/*
*--------------------------------------------------------------------------
* Copyright (c) 2025 Rene Lindsay
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
*     http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
*
* Author: Rene Lindsay <rjklindsay@hotmail.com>
*
*/

//----------------------------------------------------------------------------------------------------
//  The android_main() function is the entry-point for Android, and calls the user's main() function.
//  But first, it initialises the asset manager, so that fopen can be used to read assets from the APK.
//
//  JClass is the base class for the JNI Wrappers.  (I only added functions I actually use.)
//  It allows making Java system calls directly from C++, without having to write any Java.
//  Class and Function names match their Java equivalents, except that classes start with a J.
//----------------------------------------------------------------------------------------------------


#ifndef JCLASS_H
#define JCLASS_H

#include <jni.h>
#include <string>
#include <vector>
#include <cassert>
#include <android/log.h>
#include <android_native_app_glue.h>
#include "android_fopen.h"

extern android_app* Android_App;
void android_main(struct android_app* state);

//----------------------------------------printf for Android----------------------------------------
// Uses a 256 byte buffer to allow concatenating multiple printf's onto one log line.
// The buffer gets flushed when the printf string ends in a '\n', or the buffer is full.
// Alternative with no concatenation:
//   #define printf(...)  __android_log_print(ANDROID_LOG_INFO, LOG_TAG,__VA_ARGS__)

inline struct printBuf {
    static const int SIZE = 256;
    char buf[SIZE];
    printBuf() { clear(); }
    printBuf(const char* c) {memset(buf, 0, SIZE); strncpy(buf, c, SIZE - 1);}
    printBuf& operator+=(const char* c) {strncat(buf, c, SIZE - len() - 1); if(len() >= SIZE - 1) flush(); return *this;}
    size_t len() {return strlen(buf);}
    void clear(){ memset(buf, 0, SIZE); }
    void flush() {__android_log_print(ANDROID_LOG_INFO, "Window", "%s", buf); clear();}
}printBuf;

inline int printf(const char* format, ...) {  // printf for Android
    char buf[printBuf.SIZE];
    va_list argptr;
    va_start(argptr, format);
    vsnprintf(buf, sizeof(buf), format, argptr);
    va_end(argptr);
    printBuf += buf;
    size_t len = strlen(buf);
    if ((len >= printBuf.SIZE - 1) || (buf[len - 1] == '\n')) printBuf.flush();  // flush
    if (buf[len - 1] == '\r') printBuf.clear();
    return strlen(buf);
}
//--------------------------------------------------------------------------------------------------
//--------------------------------------JClass: JNI Wrappers----------------------------------------
class JClass {
    bool attached = false;
public:
    JavaVM* jvm=0;
    JNIEnv* env=0;
    jobject activity_obj=0;
    jclass  cls=0;
    jobject obj=0;

    operator jobject() {return obj;}

    JClass() { Init(Android_App->activity); }

    void Init(ANativeActivity* activity) {
        if (env) return;
        jvm = activity->vm;
        env = activity->env;
        activity_obj = activity->clazz;
        if((jvm->GetEnv((void**)&env,JNI_VERSION_1_6)!=JNI_OK)) {  // if not attached:
            jvm->AttachCurrentThread(&env, nullptr);               // attach
            attached = true;                                       // and flag for detach
        }
    }

    virtual ~JClass() {
        if(obj) env->DeleteLocalRef(obj);
        if(cls) env->DeleteLocalRef(cls);
        if(attached) jvm->DetachCurrentThread();  // if this obj attached, then detach
        attached = false;
    }

    jobject GlobalRef() {
        return env->NewGlobalRef(obj);
    }

    jclass FindClass(const char* class_name) {  // using local ref
        cls = env->FindClass(class_name);
        if(!cls) printf("ERROR: FindClass Failed to find class:%s\n", class_name);
        return cls;
    }

    jclass GetObjectClass(jobject object) {
        cls = env->GetObjectClass(object);
        return cls;
    }

    void SetObject(jobject object) {
        obj = object;
        cls = GetObjectClass(object);
    }

    // ----------------- jstring to std::string -----------------
    std::string asString(jstring jStr) const {
        if (!jStr) return "";
        const char* chars = env->GetStringUTFChars(jStr, nullptr);
        std::string result(chars);
        env->ReleaseStringUTFChars(jStr, chars);
        return result;
    }
    std::string asString(jobject jStr) const {return asString((jstring)jStr);}
    //-----------------------------------------------------------
    // ----------------- std::string to jstring -----------------
    jstring newStr(const char* str) {return env->NewStringUTF(str);}
    void    delStr(jstring jstr) {env->DeleteLocalRef(jstr);}
    //-----------------------------------------------------------
    //-------------- jintArray to std::vector<int> --------------
    std::vector<int> asIntVector(jintArray intArray) const { // Convert Java int[] to std::vector<int>
        jsize length = env->GetArrayLength(intArray);
        std::vector<int> array(length);
        jint *elements = env->GetIntArrayElements(intArray, nullptr);
        for (jsize i=0; i<length; ++i) array[i] = elements[i];
        env->ReleaseIntArrayElements(intArray, elements, JNI_ABORT);
        return array;
    }
    std::vector<int> asIntVector(jobject intArray) const {return asIntVector((jintArray)intArray);}
    //-----------------------------------------------------------

    jmethodID Method(const char* name, const char* sig) {
        return env->GetMethodID(cls, name, sig);
    }

    jmethodID StaticMethod(const char* name, const char* sig) {
        return env->GetStaticMethodID(cls, name, sig);
    }

    jfieldID Field(const char* name, const char* sig) {
        return env->GetFieldID(cls, name, sig);
    }

    jfieldID StaticField(const char* name, const char* sig) {
        return env->GetStaticFieldID(cls, name, sig);
    }

    //-----------------------------------------------------------

    int IntField(const char* name, const char* sig) {
        return env->GetIntField(cls, StaticField(name, sig));
    }

    jstring StaticStrField(const char* name, const char* sig) {
        return (jstring)env->GetStaticObjectField(cls, StaticField(name, sig));
    }

    //-----------------------------------------------------------

    template<typename... Args>
    jobject CallStaticObj(const char* name, const char* sig, Args&&... args) {
        jmethodID method = StaticMethod(name, sig);
        return env->CallStaticObjectMethod(cls, method, std::forward<Args>(args)...);
    }

    template<typename... Args>
    int CallStaticInt(const char* name, const char* sig, Args&&... args) {
        jmethodID method = StaticMethod(name, sig);
        return env->CallIntMethod(cls, method, std::forward<Args>(args)...);
    }

    template<typename... Args>
    float CallStaticFloat(const char* name, const char* sig, Args&&... args) {
        jmethodID method = StaticMethod(name, sig);
        return env->CallStaticFloatMethod(cls, method, std::forward<Args>(args)...);
    }

    template<typename... Args>
    bool CallStaticBool(const char* name, const char* sig, Args&&... args) {
        jmethodID method = StaticMethod(name, sig);
        return env->CallStaticBooleanMethod(cls, method, std::forward<Args>(args)...);
    }

    //-----------------------------------------------------------

    template<typename... Args>
    void CallVoid(const char* name, const char* sig, Args&&... args) {
        jmethodID method = Method(name, sig);
        env->CallVoidMethod(obj, method, std::forward<Args>(args)...);
    }

    template<typename... Args>
    jobject CallObj(const char* name, const char* sig, Args&&... args) {
        jmethodID method = Method(name, sig);
        return env->CallObjectMethod(obj, method, std::forward<Args>(args)...);
    }

    template<typename... Args>
    int CallInt(const char* name, const char* sig, Args&&... args) {
        jmethodID method = Method(name, sig);
        return env->CallIntMethod(obj, method, std::forward<Args>(args)...);
    }

    template<typename... Args>
    float CallFloat(const char* name, const char* sig, Args&&... args) {
        jmethodID method = Method(name, sig);
        return env->CallFloatMethod(obj, method, std::forward<Args>(args)...);
    }

    template<typename... Args>
    bool CallBool(const char* name, const char* sig, Args&&... args) {
        jmethodID method = Method(name, sig);
        return env->CallBooleanMethod(obj, method, std::forward<Args>(args)...);
    }

    template<typename... Args>
    std::string CallStr(const char* name, const char* sig, Args&&... args) {
        jmethodID method = Method(name, sig);
        return asString(env->CallObjectMethod(obj, method, std::forward<Args>(args)...));
    }
/*
    template<typename... Args>
    std::vector<int> CallIntVec(const char* name, const char* sig, Args&&... args) {
        jmethodID method = Method(name, sig);
        return asIntVector(env->CallObjectMethod(obj, method, std::forward<Args>(args)...));
    }
*/

    /*
    //-------------- std::vector<int> to jintArray --------------  // not used
    jintArray asJintArray(std::vector<int> intvec) const {
        jintArray array = env->NewIntArray(intvec.size());
        if(array) env->SetIntArrayRegion(array, 0, intvec.size(), intvec.data());
        return array;
    }
    //-----------------------------------------------------------
     */
    //------------------- int to jintArray[1] -------------------
    jintArray asJintArray(int i) const {
        jintArray array = env->NewIntArray(1);
        env->SetIntArrayRegion(array, 0, 1, &i);
        return array;
    }
    //-----------------------------------------------------------
    /*
    //----------- jbooleanArray to std::vector<bool> ------------  // not used
    std::vector<bool> asBoolVector(jbooleanArray array) {
        jsize length = env->GetArrayLength(array);
        std::vector<bool> result(length);
        jboolean* elements = env->GetBooleanArrayElements(array, nullptr);
        for (jsize i = 0; i < length; ++i) {
            result[i] = elements[i];
        }
        env->ReleaseBooleanArrayElements(array, elements, JNI_ABORT);
        return result;
    }
    std::vector<bool> asBoolVector(jobject array) {return asBoolVector((jbooleanArray)array);}
    //-----------------------------------------------------------
    */
    //---------------- jbooleanArray[1] to bool -----------------
    bool asBool(jbooleanArray array) {
        jboolean result = JNI_FALSE;
        env->GetBooleanArrayRegion(array, 0, 1, &result);
        return result;
    }
    bool asBool(jobject boolArray) { return asBool((jbooleanArray)boolArray); }
    //-----------------------------------------------------------
};

//------------------------------------------------------------------------------

struct JString : public JClass {
    jstring jstr = nullptr;
    JString(const char* str) { jstr = env->NewStringUTF(str);}
    JString(std::string& str) { jstr = env->NewStringUTF(str.c_str());}
    ~JString() {if(jstr) env->DeleteLocalRef(jstr);}
    operator jstring() const {return jstr;}
    std::string toString() {return asString(jstr);}
};

//------------------------------------------------------------------------------

//------------------------------------------------------------------------------

class JActivity : public JClass {  // Context
    jclass cls = GetObjectClass(activity_obj);
public:
    JActivity(){ SetObject(activity_obj); }
    ~JActivity(){ obj = nullptr; }
    const int CONTENT_ID = 0x01020002;   // android.R.id.content
    jstring INPUT_SERVICE()        { return StaticStrField("INPUT_SERVICE", "Ljava/lang/String;"); }
    jstring INPUT_METHOD_SERVICE() { return StaticStrField("INPUT_METHOD_SERVICE", "Ljava/lang/String;"); }
    jstring CLIPBOARD_SERVICE()    { return StaticStrField("CLIPBOARD_SERVICE", "Ljava/lang/String;"); }

    jobject getSystemService(jstring jstr) { return CallObj("getSystemService", "(Ljava/lang/String;)Ljava/lang/Object;", jstr); }
    jobject getWindow()                    { return CallObj("getWindow", "()Landroid/view/Window;"); }
    //void setContextView(jobject view) { CallVoid("setContentView", "(Landroid/view/View;)V", view); }
    //jobject findViewById(int id) { return CallObj("findViewById", "(I)Landroid/view/View;", id); }
    //void runOnUiThread(jobject runnable) { CallVoid("runOnUiThread", "(Ljava/lang/Runnable;)V"); }
};

class JInputManager : public JClass {
    jclass cls = FindClass("android/hardware/input/InputManager");
public:
    JInputManager() { JActivity a; obj = a.getSystemService(a.INPUT_SERVICE()); }
    std::vector<int> getInputDeviceIds() { return asIntVector(CallObj("getInputDeviceIds", "()[I")); }
    //std::vector<int> getInputDeviceIds() { return CallIntVec("getInputDeviceIds", "()[I"); }
};

//------------------------------------------------------------------------------
// CLIPBOARD
class JCharSequence : public JClass {
    jclass cls = FindClass("java/lang/CharSequence");
public:
    JCharSequence(jobject cs_obj) { obj = cs_obj; }
    std::string toString() { return CallStr("toString", "()Ljava/lang/String;"); }
};

class JClipDataItem : public JClass {
    jclass cls = FindClass("android/content/ClipData$Item");
public:
    JClipDataItem(jobject item_obj) { obj = item_obj; }
    JCharSequence getText() { return JCharSequence(CallObj("getText", "()Ljava/lang/CharSequence;")); }
};

class JClipData : public JClass {
    jclass cls = FindClass("android/content/ClipData");
public:
    JClipData(jobject cd_obj) { obj = cd_obj; }
    JClipData(jstring label, jstring text) { newPlainText(label, text); }
    JClipDataItem getItemAt(int index) { return JClipDataItem(CallObj("getItemAt", "(I)Landroid/content/ClipData$Item;", index));}
    void newPlainText(jstring label, jstring text) { assert(!obj); obj = CallStaticObj("newPlainText", "(Ljava/lang/CharSequence;Ljava/lang/CharSequence;)Landroid/content/ClipData;", label, text); }
    void newPlainText(const char* label, const char* text) { newPlainText(newStr(label), newStr(text)); }
};

class JClipboardManager : public JClass {
public:
    JClipboardManager() {
        JActivity a;
        obj = a.getSystemService(a.CLIPBOARD_SERVICE());
        cls = GetObjectClass(obj);
    }
    void setPrimaryClip(JClipData& clipData) { CallVoid("setPrimaryClip", "(Landroid/content/ClipData;)V", clipData.obj); }
    JClipData getPrimaryClip() { return JClipData(CallObj("getPrimaryClip", "()Landroid/content/ClipData;")); }
    JCharSequence getText() { return JCharSequence(CallObj("getText", "()Ljava/lang/CharSequence;")); }  // deprecated
};
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
// for ShowKeyboard
class JInputMethodManager : public JClass {
    jclass cls = FindClass("android/view/inputmethod/InputMethodManager");
public:
    JInputMethodManager() { JActivity a; obj = a.getSystemService(a.INPUT_METHOD_SERVICE()); }
    jboolean showSoftInput(jobject decorView, int flags)         { return CallBool("showSoftInput", "(Landroid/view/View;I)Z", decorView, flags); }
    jboolean hideSoftInputFromWindow(jobject iBinder, int flags) { return CallBool("hideSoftInputFromWindow", "(Landroid/os/IBinder;I)Z", iBinder, flags); }
};

class JView : public JClass {
    jclass cls = FindClass("android/view/View");
public:
    JView(jobject viewobj) { obj = viewobj; }
    jobject getWindowToken() { return CallObj("getWindowToken", "()Landroid/os/IBinder;"); }
    //void setSystemUiVisibility(int flags) { CallVoid("setSystemUiVisibility", "(I)V", flags); }
    //int  getSystemUiVisibility() { return CallInt("getSystemUiVisibility", "()I"); }
};

class JWindow : public JClass {
    jclass cls = FindClass("android/view/Window");
public:
    JWindow() {JActivity a; obj = a.getWindow();}
    JView getDecorView() { return JView(CallObj("getDecorView", "()Landroid/view/View;")); }
};

//------------------------------------------------------------------------------
// for GetUnicodeChar
class JKeyEvent : public JClass {
    jclass cls = FindClass("android/view/KeyEvent");
    jmethodID constructor = Method("<init>", "(II)V");
public:
    JKeyEvent(int eventType, int keyCode) {
        obj = env->NewObject(cls, constructor, eventType, keyCode);
    }
    int getUnicodeChar(int metaState) {return CallInt("getUnicodeChar", "(I)I", metaState);}
    //std::string getCharacters() {return CallStr("getCharacters", "()Ljava/lang/String;");}  // fails. (deprecated in API 29)
};

static std::string UnicodeToUTF8(int unicode) {
    std::string utf8;
    if (unicode < 0x80) { // 1-byte ASCII (0xxxxxxx)
        utf8 += static_cast<char>(unicode);
    }
    else if (unicode < 0x800) { // 2-byte sequence (110xxxxx 10xxxxxx)
        utf8 += static_cast<char>(0xC0 | (unicode >> 6));
        utf8 += static_cast<char>(0x80 | (unicode & 0x3F));
    }
    else if (unicode < 0x10000) { // 3-byte sequence (1110xxxx 10xxxxxx 10xxxxxx)
        utf8 += static_cast<char>(0xE0 | (unicode >> 12));
        utf8 += static_cast<char>(0x80 | ((unicode >> 6) & 0x3F));
        utf8 += static_cast<char>(0x80 | (unicode & 0x3F));
    }
    else if (unicode < 0x110000) { // 4-byte sequence (11110xxx 10xxxxxx 10xxxxxx 10xxxxxx)
        utf8 += static_cast<char>(0xF0 | (unicode >> 18));
        utf8 += static_cast<char>(0x80 | ((unicode >> 12) & 0x3F));
        utf8 += static_cast<char>(0x80 | ((unicode >> 6) & 0x3F));
        utf8 += static_cast<char>(0x80 | (unicode & 0x3F));
    }
    return utf8;
}

//------------------------------------------------------------------------------
// Gamepad layout info
class JMotionRange : public JClass {
    jclass cls = FindClass("android/view/InputDevice$MotionRange");
public:
    JMotionRange(jobject motionRangeObj) { obj = motionRangeObj; }
    int getAxis()        { return CallInt("getAxis", "()I"); }
    int getSource()      { return CallInt("getSource", "()I"); }
    float getMin()       { return CallFloat("getMin", "()F"); }
    float getMax()       { return CallFloat("getMax", "()F"); }
    float getRange()     { return CallFloat("getRange", "()F"); }
    float getFlat()      { return CallFloat("getFlat", "()F"); }
    float getFuzz()      { return CallFloat("getFuzz", "()F"); }
    float getResolution(){ return CallFloat("getResolution", "()F"); }
};

template<typename T>
class JList : public JClass {
    jclass cls = FindClass("java/util/List");
public:
    JList(jobject listObj) { obj = listObj; }
    jint size() { return CallInt("size", "()I"); }
    T get(int i){ {return CallObj("get", "(I)Ljava/lang/Object;", i);} }
};

class JInputDevice : public JClass {
    jclass cls = FindClass("android/view/InputDevice");
public:
    JInputDevice(int deviceId) { obj = CallStaticObj("getDevice", "(I)Landroid/view/InputDevice;", deviceId); }
    std::string getName()                 { return CallStr("getName", "()Ljava/lang/String;"); }
    int getVendorId()                     { return CallInt("getVendorId", "()I"); }
    int getProductId()                    { return CallInt("getProductId", "()I"); }
    std::string getDescriptor()           { return CallStr("getDescriptor", "()Ljava/lang/String;"); }
    JMotionRange getMotionRange(int axis) { return {CallObj("getMotionRange", "(I)Landroid/view/InputDevice$MotionRange;", axis)}; }
    JList<JMotionRange> getMotionRanges() { return {CallObj("getMotionRanges", "()Ljava/util/List;")}; }
    bool hasKey(int keyCode)              { return asBool(CallObj("hasKeys", "([I)[Z", asJintArray(keyCode)));}
    int getSources()                      { return CallInt("getSources", "()I"); }
};

//------------------------------------------------------------------------------

#endif
