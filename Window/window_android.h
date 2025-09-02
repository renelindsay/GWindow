//==========================ANDROID=============================
#ifdef VK_USE_PLATFORM_ANDROID_KHR

#ifndef WINDOW_ANDROID
#define WINDOW_ANDROID

//#define ENABLE_MULTITOUCH
//#define ENABLE_GAMEPAD
//#define ENABLE_CLIPBOARD

#include "WindowBase.h"
#include "JClass.h"
#include <cmath>

#include <iostream>
#include <string>
#include <cstring>

#ifdef ENABLE_GAMEPAD
#include "gamepads.h"
#include <algorithm>
#endif

#undef  repeat
#define repeat(COUNT) for (uint32_t i = 0; i < COUNT; ++i)
#define MIN(A,B) (((A)<(B))?(A):(B));
#define MAX(A,B) (((A)>(B))?(A):(B));

//========================================================
// clang-format off
// Convert native Android key-code to cross-platform USB HID code.
const unsigned char ANDROID_TO_HID[256] = {
  0,227,231,  0,  0,  0,  0, 39, 30, 31, 32, 33, 34, 35, 36, 37,
 38,  0,  0, 82, 81, 80, 79,  0,  0,  0,  0,  0,  0,  4,  5,  6,
  7,  8,  9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22,
 23, 24, 25, 26, 27, 28, 29, 54, 55,226,230,225,229, 43, 44,  0,
  0,  0, 40,  0, 53, 45, 46, 47, 48, 49, 51, 52, 56,  0,  0,  0,
  0,  0,118,  0,  0,  0,  0,  0,  0,  0,  0,  0, 75, 78,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0, 57, 71,  0,  0,  0,  0, 72, 74, 77, 73,  0,  0,  0,
 24, 25,  0, 58, 59, 60, 61, 62, 63, 64, 65, 66, 67, 68, 69, 83,
 98, 89, 90, 91, 92, 93, 94, 95, 96, 97, 84, 85, 86, 87, 99,  0,
 88,103,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0
};
// clang-format on
//==========================Android=============================

//------------------------ JNI Wrappers ------------------------
static void showKeyboard(bool visible, int flags=0) {
    JInputMethodManager InputMethod;
    JWindow window;
    JView decorView = window.getDecorView();
    if(visible) {
        InputMethod.showSoftInput(decorView, flags);
    } else {
        jobject token = decorView.getWindowToken();
        InputMethod.hideSoftInputFromWindow(token, flags);
    }
}

static int GetUnicodeChar(int eventType, int keyCode, int metaState) {
    JKeyEvent keyEvent(eventType, keyCode);
    return keyEvent.getUnicodeChar(metaState);
}

static std::vector<int> AInputQueue_getDeviceIds() {
    JInputManager inputManager;
    return inputManager.getInputDeviceIds();
}

struct GamepadInfo {
    std::string name;
    uint16_t  VID;
    uint16_t  PID;
    //uint8_t   BUS;
    std::string desc;

    struct Axis {
        int8_t axis;  // AMOTION_EVENT_AXIS_X
        float  min;   // 0 for trigger, -1 for thumbstick
        float  max;
        float  range;
        float  flat;
        float  fuzz;
    };
    std::vector<Axis> axes;     // Axis list and properties
    std::vector<int16_t> btns;  // KEYCODE_BUTTON_A
};

static GamepadInfo GetGamepadInfo(int deviceId) {
    JInputDevice device(deviceId);
    GamepadInfo info;
    info.name = device.getName();
    info.VID  = device.getVendorId();
    info.PID  = device.getProductId();
    info.desc = device.getDescriptor();
    //printf("NAME=%s\n", info.name.c_str());

    // list buttons
    for (int k = 96; k <= 110; ++k) if (device.hasKey(k)) info.btns.push_back(k);
    for (int k = 19; k <=  22; ++k) if (device.hasKey(k)) info.btns.push_back(k);

    // list axes
    auto list = device.getMotionRanges();
    for(int i=0; i<list.size(); ++i) {
        JMotionRange item = list.get(i);
        if((item.getSource() & AINPUT_SOURCE_JOYSTICK)==false) continue;
        if(item.getAxis() > 28) continue;
        uint inx = info.axes.size();
        auto& a = info.axes.emplace_back();
        a.axis = item.getAxis();
        a.min  = item.getMin();
        a.max  = item.getMax();
        a.range= item.getRange();
        a.flat = item.getFlat();
        a.fuzz = item.getFuzz();
        //printf("inx=%d Axis=%2d Min=% f Max=% f Range=%f Flat=%f Fuzz=%f\n", inx, a.axis, a.min, a.max, a.range, a.flat, a.flat);
    }

    return info;
}

//--------------------------------------------------------------

class Window_android : public WindowBase {
    android_app* app = 0;
    CMTouch MTouch;

    //---- Gamepad ----
#ifdef ENABLE_GAMEPAD
#define MAX_BTNS 19
#define MAX_AXIS 9
    struct GPadSlots {
        int32_t deviceID=0;
        char name[256] = {};

        struct Btns {
            uint16_t BTN=0;   // AKEYCODE event code
            int8_t  eBTN=0;   // eGamepadBtn
        }b[MAX_BTNS]={};      // buttons
        int8_t eBtn(uint16_t BTN) { for(auto& i:b) if(i.BTN==BTN) return i.eBTN; return eBTN_UNKNOWN; }  //BTN to eBTN

        struct eAxes {
            int8_t axis =-1;      // AMOTION_EVENT_AXIS_X
            float  min  = 0;      // 0 for trigger, -1 for thumbstick
            float  max  = 0;      // 1
            float  flat = 0;      // deadzone
            float  fuzz = 0;      // jitter
            bool   flip = false;  // flip the axis
            float  prev = 0;      // previous value
        }a[MAX_AXIS]={};

    }gpads[MAX_GAMEPADS];
#endif
    //-----------------

  public:
    void setTitle(const char* title){};  // TODO : Set window title?
    void setPosition(uint x, uint y){};
    void setSize(uint w, uint h){};

  private:
    void Create(const char* title="", uint width=640, uint height=480) {
        shape.width  = 0;  // width;
        shape.height = 0;  // height;
        running      = true;
        //LOGI("Creating Android-Window...\n");
        app = Android_App;

        //---Wait for window to be created AND gain focus---
        while (!has_focus) {
            int events = 0;
            struct android_poll_source* source;
            int id = ALooper_pollOnce(100, NULL, &events, (void**)&source);
            if (id == LOOPER_ID_MAIN) {
                int8_t cmd = android_app_read_cmd(app);
                android_app_pre_exec_cmd(app, cmd);
                if (app->onAppCmd) app->onAppCmd(app, cmd);
                if (cmd == APP_CMD_INIT_WINDOW) {
                    shape.width  = (uint16_t)ANativeWindow_getWidth (app->window);
                    shape.height = (uint16_t)ANativeWindow_getHeight(app->window);
                    eventFIFO.push(resizeEvent(shape.width, shape.height));        // post window-resize event
/*
                    //Get device configuration for dp scaling
                    AConfiguration* config = AConfiguration_new();
                    AConfiguration_fromAssetManager(config, app->activity->assetManager);
                    int32_t dpi = AConfiguration_getDensity(config);
                    display_scale = dpi / 160.0;
                    AConfiguration_delete(config);
*/
                }
                if (cmd == APP_CMD_GAINED_FOCUS) eventFIFO.push(focusEvent(true)); // post focus-event
                android_app_post_exec_cmd(app, cmd);
            }
        }
        ALooper_pollOnce(20, NULL, NULL, NULL);  // for keyboard
        //--------------------------------------------------
    };

  public:
    Window_android(){Create();}

    Window_android(const char* title, uint width, uint height) {
        Create(title, width, height);
    }

    virtual ~Window_android(){}

    //-------------------- GAMEPAD ---------------------
 #ifdef ENABLE_GAMEPAD
    int8_t FindGamepad(AInputEvent* a_event) {  // returns gamepad slot id or -1 if failed
        uint32_t deviceID = AInputEvent_getDeviceId(a_event);  // get deviceID from event
        return ConnectGamepad(deviceID);                       // (connect) and return slot
    }

    int ConnectGamepad(uint32_t deviceID) {  // try to connect to gamepad
        repeat(MAX_GAMEPADS) if (gpads[i].deviceID == deviceID) return i;  // if already connected, return slot
        repeat(MAX_GAMEPADS) if (gpads[i].deviceID == 0) {                 // find empty slot, connect, return slot
                auto& pad = gpads[i];
                pad.deviceID = deviceID;
                MapGamepad(i);
                eventFIFO.push(gpadConnect(i, true));
                return i;
            }
        return -1;
    }

    void MapGamepad(int8_t slot) {
        auto& pad = gpads[slot];
        GamepadInfo info = GetGamepadInfo(pad.deviceID);
        strncpy(pad.name, info.name.c_str(), sizeof(pad.name)-1);
        printf("Gamepad %d found: %s (VID:%x PID:%x)\n", slot, pad.name, info.VID, info.PID);
        //printf("Descriptor GUID: %s\n", info.desc.c_str());
        for(int i=0; i<info.btns.size(); ++i) pad.b[i].BTN  = info.btns[i];        // list AKEYCODE event codes

        //                A  B  X  Y SL SR TL TR UP DN LE RI THUML THUMR TRIGR SE ST
        int8_t eBTN [] = {1, 2, 3, 4, 5, 6, 7, 8, 9,10,11,12,-1,-2,-3,-4,-5,-6,13,14};  // pos-to-eBTN
        int8_t eAXIS[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 3, 4, 5, 6, 0, 0};  // pos-to-eAxis

        auto* p_layout = get_gamepad_layout(info.VID, info.PID, 5, info.name.c_str());
        if(p_layout) {  // Gamepad found. Decode layout tokens.
            auto layout = *p_layout;
            printf("A   B   X   Y   LB  RB  LS  RS  UP  DN  LE  RI  THUMBL  THUMBR  TRIGER  SEL START\n");
            for(auto& code : layout) { printf("%02x  ", code); } printf("\n");

            for(int i=0; i<layout.size(); ++i) {
                uint8_t code = layout[i];
                if(code==0xff) continue;                         // not mapped
                uint8_t num = code &0x1F;                        // extract number
                bool flip   = code &0x80;                        // extract flip flag
                bool isBtn  =!(code&0x60);                       // No flag for button
                bool isHat  = code &0x40;                        // extract hat flag
                bool isAxis = code &0x20;                        // extract axis flag
                if(isBtn) pad.b[num].eBTN = eBTN[i];             // button event code to eGamepadBtn map
                if(isAxis && eAXIS[i]) {                         // axis event code
                    auto& ia = info.axes[num];
                    pad.a[eAXIS[i]] = {ia.axis, ia.min, ia.max, ia.flat, ia.fuzz, flip};
                }
                if(isBtn && i== 8) pad.a[7]={15,-1,1,0,0,num==16};  // hat-x flip
                if(isBtn && i==10) pad.a[8]={16,-1,1,0,0,num==18};  // hat-y flip

                //printf("i=%d num=%d isBtn=%d isHat=%d isAxis=%d filp=%d\n",i ,num, isBtn, isHat, isAxis, flip);
            }
        }
        if(!p_layout) {  // Gamepad not found.  Use heuristics.
            // Buttons: apply default layout
            // Button        A   B   C   X   Y   Z  L1  R1  L2  R2 THL THR STA SEL MOD  UP  DN  LE  RI
            // layout b#     0   1   2   3   4   5   6   7   8   9  10  11  12  13  14  15  16  17  18
            uint8_t BTN[]={ 96, 97, 98, 99,100,101,102,103,104,105,106,107,108,109,110, 19, 20, 21, 22};  // AKEYCODE event code
            int8_t  MAP[]={  1,  2,  0,  3,  4,  0,  5,  6,  0,  0,  7,  8, 14, 13, 15,  9, 10, 11, 12};  // eBTN: default layout
            for(int i=0; i<sizeof(MAP); ++i) pad.b[i] = {BTN[i], MAP[i]};  // apply default layout

            //Axis: Identify axis role, based on its min and flat values.
            auto assign = [&](GPadSlots::eAxes& e, GamepadInfo::Axis& i, bool flip=false) {
                e = {i.axis, i.min, i.max, i.flat, i.fuzz, flip};
                i.axis = -1;  //mark assigned
            };

            auto assignFirst = [&](uint8_t eAxis, auto fn) {
                int axis_count = MIN(info.axes.size(), MAX_AXIS)      // Number of axes found
                for(int i = 0; i < axis_count; ++i) {                 // for each axis
                    auto& ia=info.axes[i];                            // get axis info
                    if(ia.axis<0) continue;                           // skip if already assigned
                    if(fn(ia)) { assign(pad.a[eAxis], ia); return; }  // assign if conditions met
                }
            };

            auto& t = info.axes[0];  // thumb_Left_X
            assign(pad.a[eAXIS_LX], info.axes[0]);  // Assume axis 0 is left_thumb_X
            assign(pad.a[eAXIS_LY], info.axes[1]);  // Assume axis 1 is left_thumb_Y
            assignFirst (eAXIS_RX, [&](auto& ia){ return ia.min==t.min && ia.flat==t.flat; });  // is right thumb
            assignFirst (eAXIS_RY, [&](auto& ia){ return ia.min==t.min && ia.flat==t.flat; });  // is right thumb
            assignFirst (eAXIS_TL, [&](auto& ia){ return ia.min==0 && ia.flat>0.f; });          // is trigger
            assignFirst (eAXIS_TR, [&](auto& ia){ return ia.min==0 && ia.flat>0.f; });          // is trigger
            assignFirst (7       , [&](auto& ia){ return ia.axis==15 || ia.flat==0.f; });       // is hat
            assignFirst (8       , [&](auto& ia){ return ia.axis==16 || ia.flat==0.f; });       // is hat
        }

        auto& LY=pad.a[eAXIS_LY];  LY.flip=!LY.flip;  // up is positive
        auto& RY=pad.a[eAXIS_RY];  RY.flip=!RY.flip;  // up is positive
        for(auto& a:pad.a) {a.min*=0.8; a.max*=0.8;}  // leave room for auto-calibrate

        //printf("BTNS:\n"); for(int i=0; i<MAX_BTNS; ++i) printf("%d: %d->%d\n", i, pad.b[i].BTN, pad.b[i].eBTN);
        //printf("eAXIS:\n"); for(int i=0; i<MAX_AXIS; ++i) printf("i=%d: axis=%2d min=% f max=% f flat=%f fuzz=%f flip=%d\n", i, pad.a[i].axis, pad.a[i].min, pad.a[i].max, pad.a[i].flat, pad.a[i].fuzz, pad.a[i].flip );
    }

    void MonitorGamepads() {  // poll for gamepad connect/disconnect
        // Run no more than once per second
        static clock_t last_time = clock();
        clock_t curr_time = clock();
        if (curr_time - last_time < CLOCKS_PER_SEC) return;
        last_time = curr_time;

        // Check if device list changed
        auto list = AInputQueue_getDeviceIds();
        static std::vector<int> prev_list;
        if (list == prev_list) return;  // exit if nothing changed
        prev_list = list;
        //for(int item : list) printf("%d ", item); printf("\n");

        // Check for disconnects
        for(int i=0; i<MAX_GAMEPADS; ++i) {
            int32_t id = gpads[i].deviceID;
            if(id) if (std::find(list.begin(), list.end(), id) == list.end()) {
                gpads[i] = GPadSlots{};  // clear gamepad slot
                eventFIFO.push(gpadConnect(i,false));
            }
        }
        // Check for new connects
        for(auto item:list) {
              JInputDevice device(item);
              int sources = device.getSources();
              bool isGamepad = ((sources & AINPUT_SOURCE_GAMEPAD) == AINPUT_SOURCE_GAMEPAD) &&
                               ((sources & AINPUT_SOURCE_JOYSTICK) == AINPUT_SOURCE_JOYSTICK);
              if(isGamepad) ConnectGamepad(item);
        }
    }

    EventType GetGPadButtonEvent(AInputEvent* a_event) {
        //ASSERT(AInputEvent_getType(a_event)==AINPUT_EVENT_TYPE_KEY, "Not a key press event.");
        int8_t id = FindGamepad(a_event);            // Get gamepad ID for this event
        if(id==-1) return {};

        int32_t keycode  = AKeyEvent_getKeyCode(a_event);
        if (AKeyEvent_getRepeatCount(a_event) > 0) return {};  // ignore keyboard repeat events
        if((keycode>=19 && keycode<=22) || (keycode>=96 && keycode<=110)) {        // valid btn ranges
            bool down = (AKeyEvent_getAction(a_event) == AKEY_EVENT_ACTION_DOWN);  // btn is pressed
            //printf("keycode:%d (0x%02x) %d\n",keycode,keycode, down);
            auto& pad = gpads[id];
            int8_t eBTN = pad.eBtn(keycode);                        // keycode to eBTN
            if(eBTN>0) eventFIFO.push(gpadButton(id, eBTN, down));  // is button:  eBTN event
            if(eBTN<0) eventFIFO.push(gpadAxis  (id,-eBTN, down));  // is trigger: aAXIS event
        }
        return eventFIFO.pop();
    }

 #define eAXIS_HATX 7
 #define eAXIS_HATY 8

    EventType GetGPadAxisEvent(AInputEvent* a_event) {
        //ASSERT(AInputEvent_getType(a_event)==AINPUT_EVENT_TYPE_MOTION, "Not a motion event.");
        int8_t id = FindGamepad(a_event);            // Get gamepad ID for this event
        if(id==-1) return {};
        GPadSlots& gpad = gpads[id];

        //---AXIS---
        auto isFuzz = [](float val, auto& a) -> bool { // detect fuzz events
            float delta = fabs(a.prev - val);
            if(delta<a.fuzz) return true;
            a.prev = val;
            return false;
        };

        auto flatzone = [&](float val, auto& a) -> float {
            float mag = std::max(fabs(val)-a.flat, 0.f);
            if(mag<=0.f) return 0.f;
            float sign = (val<0.f)?-1.f:1.f;
            return sign * (mag / (1.f-a.flat)) / a.max;
        };

        auto calibrate = [&](float val, auto& a) {  // auto-calibrate min/max range
            float mag = fabs(val);
            if(mag>a.max) {a.max = mag; a.min = -mag;}
        };

        auto axisEvent = [&](eGamepadAxis eAxis) {
            auto& a = gpad.a[eAxis];                                           // Get axis info
            if(a.axis<0) return;                                               // skip if not mapped
            float flip=a.flip?-1:1;                                            // flip the axis
            float val = AMotionEvent_getAxisValue(a_event, a.axis, 0) * flip;  // query current axis value
            calibrate(val, a);                                                 // adjust min/max
            val = flatzone(val, a);                                            // apply deadzone
            if(isFuzz(val, a)) return;                                         // skip if value has not changed
            eventFIFO.push(gpadAxis(id, eAxis, val));                          // push event
        };

        axisEvent(eAXIS_LX);  // left thumb
        axisEvent(eAXIS_LY);
        axisEvent(eAXIS_RX);  // right thumb
        axisEvent(eAXIS_RY);
        axisEvent(eAXIS_TL);  // trigger
        axisEvent(eAXIS_TR);
        //---------

        //---HAT---
        Gamepad& pad = gamepad[id];
        auto Hat = [&](int val, int btnNeg, int btnPos) { // convert hat axis values to button events
            if((val!=-1) && ( pad.buttons[btnNeg])) eventFIFO.push(gpadButton(id, btnNeg, 0));
            if((val!= 1) && ( pad.buttons[btnPos])) eventFIFO.push(gpadButton(id, btnPos, 0));
            if((val==-1) && (!pad.buttons[btnNeg])) eventFIFO.push(gpadButton(id, btnNeg, 1));
            if((val== 1) && (!pad.buttons[btnPos])) eventFIFO.push(gpadButton(id, btnPos, 1));
        };

        auto hatVal = [&](int8_t eAxis) -> float {
            auto& a = gpad.a[eAxis];                                    // Get axis info
            float flip=a.flip?-1:1;                                     // flip the axis
            float val = AMotionEvent_getAxisValue(a_event, a.axis, 0);  // query current axis value
            return val * flip;
        };

        float hatx = hatVal(eAXIS_HATX);
        float haty = hatVal(eAXIS_HATY);
        Hat(hatx, eDPAD_LEFT, eDPAD_RIGHT);
        Hat(haty, eDPAD_UP,   eDPAD_DOWN);
        //---------

        return eventFIFO.pop();
    }
 #else  // ENABLE_GAMEPAD
    void MonitorGamepads() {}
    EventType GetGPadAxisEvent(AInputEvent* a_event){return {};}
    EventType GetGPadButtonEvent(AInputEvent* a_event){return {};}
#endif // ENABLE_GAMEPAD
    //--------------------------------------------------
    //-------------------- KEYBOARD --------------------
    EventType GetKeyboardEvent(AInputEvent* a_event) {  // KEYBOARD
        int32_t a_action = AKeyEvent_getAction(a_event);
        int32_t keycode  = AKeyEvent_getKeyCode(a_event);
        uint8_t hidcode  = ANDROID_TO_HID[keycode];
        // printf("key action:%d keycode=%d\n",a_action,keycode);
        if(!hidcode) return {};  // unknown key

        switch (a_action) {
            case AKEY_EVENT_ACTION_DOWN: {
                static char buf[5] = {};
                int metaState = AKeyEvent_getMetaState(a_event);
                int unicode = GetUnicodeChar(AKEY_EVENT_ACTION_DOWN, keycode, metaState);
                if(unicode) {
                    std::string utf8text = UnicodeToUTF8(unicode);
                    memcpy(buf, utf8text.c_str(), 4);  // copy to static buf so it doesn't go out of scope
                    eventFIFO.push(textEvent(buf));    // text typed event  (store in FIFO for next run)
                }
                return keyEvent(eDOWN, hidcode);       // key pressed event (returned on this run)
            }
            case AKEY_EVENT_ACTION_UP: {
                return keyEvent(eUP, hidcode); // key released event
            }
            case AKEY_EVENT_ACTION_MULTIPLE: {
                // TODO: Implement IME and auto-correct string input,
                //  (When google fixes the getCharacters bug.)
                //return textEvent("IME/AutoCorrect not supported");
            }
        }
        return {};
    }
    //--------------------------------------------------
    //------------------ TOUCHSCREEN -------------------

    EventType GetTouchscreenEvent(AInputEvent* a_event) {
        EventType event = {};
#ifdef ENABLE_MULTITOUCH
        int32_t a_action = AMotionEvent_getAction(a_event);
        int action       = (a_action & 255); // get action-code from bottom 8 bits
        MTouch.count     = (int)AMotionEvent_getPointerCount(a_event);
        if (action == AMOTION_EVENT_ACTION_MOVE) {  //touch drag events
            for(uint i = 0; i<MTouch.count; ++i) {
                uint8_t finger_id = (uint8_t)AMotionEvent_getPointerId(a_event, i);
                float x           = AMotionEvent_getX(a_event, i);
                float y           = AMotionEvent_getY(a_event, i);
                if(i==0) event    = MTouch.Event(eMOVE, x, y, finger_id);   // return first finger directly
                else eventFIFO.push(MTouch.Event(eMOVE, x, y, finger_id));  // queue additional finger events
            }
        } else {  // touch up/down events
            size_t inx        = (size_t)(a_action >> 8); // get index from top 24 bits
            uint8_t finger_id = (uint8_t)AMotionEvent_getPointerId(a_event, inx);
            float x           = AMotionEvent_getX(a_event, inx);
            float y           = AMotionEvent_getY(a_event, inx);
            switch (action) {
                case AMOTION_EVENT_ACTION_POINTER_DOWN:
                case AMOTION_EVENT_ACTION_DOWN      :  event = MTouch.Event(eDOWN, x, y, finger_id);  break;
                case AMOTION_EVENT_ACTION_POINTER_UP:
                case AMOTION_EVENT_ACTION_UP        :  event = MTouch.Event(eUP  , x, y, finger_id);  break;
                case AMOTION_EVENT_ACTION_CANCEL    :  MTouch.Clear();                                break;
                default:break;
            }
        }
        //-------------------------Emulate mouse from touch events--------------------------
        // if(event.tag==EventType::TOUCH && event.touch.id==0){  //if one-finger touch
        //     eventFIFO.push(mouseEvent(event.touch.action, event.touch.x, event.touch.y, 1));
        // }
        //----------------------------------------------------------------------------------
#endif // ENABLE_MULTITOUCH
        return event;
    }
    //--------------------------------------------------
    //--------------------- MOUSE ----------------------
    EventType GetMouseEvent(AInputEvent* a_event) {
        EventType event = {};
        int32_t a_action = AMotionEvent_getAction(a_event);
        int action = (a_action & 255); // get action-code from bottom 8 bits

        int16_t x = (int16_t)AMotionEvent_getX(a_event, 0);
        int16_t y = (int16_t)AMotionEvent_getY(a_event, 0);
        bool moved = (x!=mouse.pos.x || y!=mouse.pos.y);
        if(!moved && action==AMOTION_EVENT_ACTION_HOVER_MOVE) return event;  // eliminate fake move events

        // Get button state (bitmask: 0x1 = left, 0x2 = right, 0x4 = middle)
        int32_t buttons = AMotionEvent_getButtonState(a_event);
        uint8_t bestBtn = getBtnState(1) ? 1 : getBtnState(2) ? 2 : getBtnState(3) ? 3 : 0;

        uint8_t btn = 0;  // get button that changed
        if(mouse.btn[3] != (buttons & AMOTION_EVENT_BUTTON_SECONDARY)) btn = 3;
        if(mouse.btn[2] != (buttons & AMOTION_EVENT_BUTTON_TERTIARY)) btn = 2;
        if(mouse.btn[1] != (buttons & AMOTION_EVENT_BUTTON_PRIMARY)) btn = 1;

        switch (action) {
            case AMOTION_EVENT_ACTION_BUTTON_PRESS   : event = mouseEvent(eDOWN, x, y, btn);     break;
            case AMOTION_EVENT_ACTION_MOVE           : event = mouseEvent(eMOVE, x, y, bestBtn); break;
            case AMOTION_EVENT_ACTION_HOVER_MOVE     : event = mouseEvent(eMOVE, x, y, 0  );     break;
            case AMOTION_EVENT_ACTION_BUTTON_RELEASE : event = mouseEvent(eUP,   x, y, btn);     break;
            case AMOTION_EVENT_ACTION_SCROLL: {
                float vscroll = AMotionEvent_getAxisValue(a_event, AMOTION_EVENT_AXIS_VSCROLL, 0);
                uint8_t wheel = (vscroll > 0) ? 4 : 5;
                event = mouseEvent(eDOWN, x, y, wheel);
                break;
            }
            default: break;
        }
        return event;
    }
    //--------------------------------------------------
    //--------------- Main event handler ---------------
    EventType getEvent(bool wait_for_event = false) {
        EventType event = {};
        if (!eventFIFO.isEmpty()) return eventFIFO.pop();  // pop message from message queue buffer

        int events = 0;
        struct android_poll_source* poll_source;
        int timeoutMillis = wait_for_event ? -1 : 0; // Blocking or non-blocking mode
        int id = ALooper_pollOnce(timeoutMillis, NULL, &events, (void**)&poll_source);

        // if(id>=0) printf("id=%d events=%d, source=%d",id,(int)events, source[0]);
        // if(source) source->process(app, source);

        if (id == LOOPER_ID_MAIN) {
            int8_t cmd = android_app_read_cmd(app);
            if (cmd == APP_CMD_TERM_WINDOW) return event;  // prevent crash when mouse connects
            android_app_pre_exec_cmd(app, cmd);
            if (app->onAppCmd != nullptr) app->onAppCmd(app, cmd);
            switch (cmd) {
                case APP_CMD_GAINED_FOCUS: event = focusEvent(true);  break;
                case APP_CMD_LOST_FOCUS  : event = focusEvent(false); break;
                default: break;
            }
            android_app_post_exec_cmd(app, cmd);
            return event;
        } else if (id == LOOPER_ID_INPUT) {
            AInputEvent* a_event = nullptr;
            while (AInputQueue_getEvent(app->inputQueue, &a_event) >= 0) {
                //LOGV("Event: source=0x%4x type=%d\n", AInputEvent_getSource(a_event),AInputEvent_getType(a_event));
                if (AInputQueue_preDispatchEvent(app->inputQueue, a_event)) { continue; }
                int32_t handled = 0;
                if (app->onInputEvent) handled = app->onInputEvent(app, a_event);

                int32_t source = AInputEvent_getSource(a_event);
                //bool isClassButton   = (source & AINPUT_SOURCE_CLASS_BUTTON);
                //bool isClassPointer  = (source & AINPUT_SOURCE_CLASS_POINTER);
                //bool isClassJoystick = (source & AINPUT_SOURCE_CLASS_JOYSTICK);

                source&=AINPUT_SOURCE_ANY;
                bool isKeyboard  = (source & AINPUT_SOURCE_KEYBOARD);     // class button
                bool isGamepad   = (source & AINPUT_SOURCE_GAMEPAD);      // class button
                bool isJoystick  = (source & AINPUT_SOURCE_JOYSTICK);     // class joystick
                bool isTouch     = (source & AINPUT_SOURCE_TOUCHSCREEN);  // class pointer
                bool isMouse     = (source & AINPUT_SOURCE_MOUSE);        // class pointer
                bool isStylus    = (source & AINPUT_SOURCE_STYLUS);       // class pointer
                if(isStylus) isMouse = true;  // treat stylus as mouse. TODO: Add Stylus support
                //printf("source=0x%4x %s%s%s%s%s\n", source, isTouch?"T":".", isMouse?"M":".", isKeyboard?"K":".", isGamepad?"G":".", isJoystick?"J":".");

                if(!event && isKeyboard) {event = GetKeyboardEvent   (a_event);}
                if(!event && isGamepad)  {event = GetGPadButtonEvent (a_event);}
                if(!event && isJoystick) {event = GetGPadAxisEvent   (a_event);}
                if(!event && isTouch)    {event = GetTouchscreenEvent(a_event);}
                if(!event && isMouse)    {event = GetMouseEvent      (a_event);}
                if(!event) event = eventFIFO.pop();

                //handled |= event;  // if an event was created, mark it as handled
                handled = 1;       // on second thought, mark it as handled anyway
                AInputQueue_finishEvent(app->inputQueue, a_event, handled);
                return event;
            }
        }  // else if (id == LOOPER_ID_USER) { printf("LOOPER_ID_USER\n");}

        MonitorGamepads();
        if (app->destroyRequested) return closeEvent();  // Check if we are exiting.
        return {};
    };
    //--------------------------------------------------

    //--Show / Hide keyboard--
    void showKeyboard(bool enabled) {
        ::showKeyboard(enabled);
        //LOGI("%s keyboard", enabled ? "Show" : "Hide");
    }

    virtual const void* getNativeHandle() const {return app->window;};

    float getDisplayScale() {
        //Get device configuration for dp scaling
        AConfiguration* config = AConfiguration_new();
        AConfiguration_fromAssetManager(config, app->activity->assetManager);
        int32_t dpi = AConfiguration_getDensity(config);
        float scale = dpi / 160.0;
        AConfiguration_delete(config);
        //printf("scale=%f\n", scale);
        return scale;
    }

#ifdef ENABLE_SHOWIMAGE
    virtual void showImage(uint32_t* buf, uint32_t width, uint32_t height) {
        auto& wnd = app->window;
        int w = ANativeWindow_getWidth(wnd);
        int h = ANativeWindow_getHeight(wnd);
        //int s = getScale();
        int s = 1.0;
        //printf("w=%d h=%d  w2=%d h2=%d\n", w,h, width, height);

        ANativeWindow_Buffer outbuf;
        //ANativeWindow_setBuffersGeometry(wnd, width, height, WINDOW_FORMAT_RGBA_8888);
        ANativeWindow_setBuffersGeometry(wnd, w/s, h/s, WINDOW_FORMAT_RGBA_8888);

        ANativeWindow_acquire(wnd);
        ARect bounds{0, 0, (int)w, (int)h};
        ANativeWindow_lock(wnd, &outbuf, &bounds);
        //printf("bounds: left=%d top=%d right=%d bottom=%d\n", bounds.left, bounds.top, bounds.right, bounds.bottom);
        w = bounds.right - bounds.left;
        h = bounds.bottom - bounds.top;
        uint min_w = MIN(w, width)   // draw min of window-w and image-w
        uint min_h = MIN(h, height)  // draw min of window-h and image-h
        int stride = outbuf.stride;  // Actual buffer stride for memory alignment

        for(int y = 0; y<min_h; ++y) {
            uint32_t* src = buf + y * width;
            uint32_t* dst = ((uint32_t*)outbuf.bits) + y*stride;
            memcpy(dst, src, min_w*4);
        }
        ANativeWindow_unlockAndPost(wnd);
        ANativeWindow_release(wnd);
    }
#endif
#ifdef ENABLE_CLIPBOARD
#define EXIT_IF_NULL(var) if(!var) return nullptr;
    const char* getClipboardText() {
        JClipboardManager clipman;
        JClipData clip = clipman.getPrimaryClip();   EXIT_IF_NULL(clip.obj)
        JClipDataItem item = clip.getItemAt(0);      EXIT_IF_NULL(item.obj)
        JCharSequence cs = item.getText();           EXIT_IF_NULL(cs.obj)
        clipboard = cs.toString();
        return clipboard.c_str();
    }

    void setClipboardText(const char* str) {
        JClipboardManager clipman;
        JString label("");
        JString text(str);
        JClipData clip(label, text);
        clipman.setPrimaryClip(clip);
    }
#undef EXIT_IF_NULL
#endif

};

#endif

#endif  // VK_USE_PLATFORM_ANDROID_KHR
//==============================================================
