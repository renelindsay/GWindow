// =========
// GWindow.h
// =========

// How to use GWindow.h:
// #define GWINDOW_IMPLEMENTATION    // Do this in one .cpp file only.
// #include "GWindow.h"              // Do this wherever GWindow is used.

#ifndef GWINDOW_H
#define GWINDOW_H

// -------------------------------
// config.h
// -------------------------------

#pragma once

#ifdef _WIN32
    #undef  VK_USE_PLATFORM_WIN32_KHR
    #define VK_USE_PLATFORM_WIN32_KHR
#elif  __ANDROID__
    #undef  VK_USE_PLATFORM_ANDROID_KHR
    #define VK_USE_PLATFORM_ANDROID_KHR
#elif  __linux__
    #undef  VK_USE_PLATFORM_XCB_KHR
    #define VK_USE_PLATFORM_XCB_KHR
#endif

#define ENABLE_MULTITOUCH
#define ENABLE_GAMEPAD
#define ENABLE_CLIPBOARD
#define ENABLE_SHOWIMAGE
#define ENABLE_FULLSCREEN
#define ENABLE_CURSOR

// -------------------------------
// keycodes.h
// -------------------------------

/*-----------------------------------------------------------------------
*
* These are the standard, platform-independent USB HID keyboard codes,
* as defined at: http://www.freebsddiary.org/APC/usb_hid_usages.php
*
* These Keycodes are returned by the onKeyEvent, in the Window class,
* whenever a key is pressed or released.
* In order to provide consistent results across all platforms, the Window_###
* class converts the native platform-specific scancodes to these cross-platform
* USB HID codes, before returning it in the onKeyEvent.
*
* The layout of these keycodes are fixed, and the names correspond to a
* US keyboard layout. Unlike the key symbols, these keycodes do not change
* with international keyboard layout settings.
*
* eg. KEY_Z corresponds to the lower left key, which produces a 'z' character
* on US and UK qwerty keyboards, but a 'y' on German keyboards, and a ';' on
* dvorak keyboards, even though it is the same physical key.
*
* Therefore, use these keycodes for game controls, to ensure a consistent layout,
* but when text input is required, use the onTextEvent instead, to get the correct
* text symbol, according to the current configured keyboard layout settings.
*
* Contacts for feedback:
-   rjklindsay@hotmail.com (Rene Lindsay)
*/
//-----------------------------------------------------------------------

#ifndef KEYCODE_H
#define KEYCODE_H

// clang-format off
enum eKeycode {
    eKEY_NONE          = 0, // Undefined. (No event)
    eKEY_A             = 4,
    eKEY_B             = 5,
    eKEY_C             = 6,
    eKEY_D             = 7,
    eKEY_E             = 8,
    eKEY_F             = 9,
    eKEY_G             = 10,
    eKEY_H             = 11,
    eKEY_I             = 12,
    eKEY_J             = 13,
    eKEY_K             = 14,
    eKEY_L             = 15,
    eKEY_M             = 16,
    eKEY_N             = 17,
    eKEY_O             = 18,
    eKEY_P             = 19,
    eKEY_Q             = 20,
    eKEY_R             = 21,
    eKEY_S             = 22,
    eKEY_T             = 23,
    eKEY_U             = 24,
    eKEY_V             = 25,
    eKEY_W             = 26,
    eKEY_X             = 27,
    eKEY_Y             = 28,
    eKEY_Z             = 29,
    eKEY_1             = 30, // 1 and !
    eKEY_2             = 31, // 2 and @
    eKEY_3             = 32, // 3 and #
    eKEY_4             = 33, // 4 and $
    eKEY_5             = 34, // 5 and %
    eKEY_6             = 35, // 6 and ^
    eKEY_7             = 36, // 7 and &
    eKEY_8             = 37, // 8 and *
    eKEY_9             = 38, // 9 and (
    eKEY_0             = 39, // 0 and )
    eKEY_Enter         = 40, // (Return)
    eKEY_Escape        = 41,
    eKEY_Delete        = 42, // (Backspace)
    eKEY_Tab           = 43,
    eKEY_Space         = 44,
    eKEY_Minus         = 45, // - and (underscore)
    eKEY_Equals        = 46, // = and +
    eKEY_LeftBracket   = 47, // [ and {
    eKEY_RightBracket  = 48, // ] and }
    eKEY_Backslash     = 49, // \ and |
 // eKEY_NonUSHash     = 50, // # and ~
    eKEY_Semicolon     = 51, // ; and :
    eKEY_Quote         = 52, // ' and "
    eKEY_Grave         = 53,
    eKEY_Comma         = 54, // , and <
    eKEY_Period        = 55, // . and >
    eKEY_Slash         = 56, // / and ?
    eKEY_CapsLock      = 57,
    eKEY_F1            = 58,
    eKEY_F2            = 59,
    eKEY_F3            = 60,
    eKEY_F4            = 61,
    eKEY_F5            = 62,
    eKEY_F6            = 63,
    eKEY_F7            = 64,
    eKEY_F8            = 65,
    eKEY_F9            = 66,
    eKEY_F10           = 67,
    eKEY_F11           = 68,
    eKEY_F12           = 69,
    eKEY_PrintScreen   = 70,
    eKEY_ScrollLock    = 71,
    eKEY_Pause         = 72,
    eKEY_Insert        = 73,
    eKEY_Home          = 74,
    eKEY_PageUp        = 75,
    eKEY_DeleteForward = 76, // Delete key
    eKEY_End           = 77,
    eKEY_PageDown      = 78,
    eKEY_Right         = 79, // Right arrow
    eKEY_Left          = 80, // Left arrow
    eKEY_Down          = 81, // Down arrow
    eKEY_Up            = 82, // Up arrow
    eKP_NumLock        = 83,
    eKP_Divide         = 84,
    eKP_Multiply       = 85,
    eKP_Subtract       = 86,
    eKP_Add            = 87,
    eKP_Enter          = 88,
    eKP_1              = 89,
    eKP_2              = 90,
    eKP_3              = 91,
    eKP_4              = 92,
    eKP_5              = 93,
    eKP_6              = 94,
    eKP_7              = 95,
    eKP_8              = 96,
    eKP_9              = 97,
    eKP_0              = 98,
    eKP_Point          = 99, // . and Del
    eKP_Equals         = 103,
    eKEY_F13           = 104,
    eKEY_F14           = 105,
    eKEY_F15           = 106,
    eKEY_F16           = 107,
    eKEY_F17           = 108,
    eKEY_F18           = 109,
    eKEY_F19           = 110,
    eKEY_F20           = 111,
    eKEY_F21           = 112,
    eKEY_F22           = 113,
    eKEY_F23           = 114,
    eKEY_F24           = 115,
 // eKEY_Help          = 117,
    eKEY_Menu          = 118,
    eKEY_Mute          = 127,
    eKEY_VolumeUp      = 128,
    eKEY_VolumeDown    = 129,
    eKEY_LeftControl   = 224, // WARNING : Android has no Ctrl keys.
    eKEY_LeftShift     = 225,
    eKEY_LeftAlt       = 226,
    eKEY_LeftGUI       = 227,
    eKEY_RightControl  = 228,
    eKEY_RightShift    = 229, // WARNING : Win32 fails to send a WM_KEYUP message if both shift keys are pressed, and one released.
    eKEY_RightAlt      = 230,
    eKEY_RightGUI      = 231
};

enum eGamepadBtn {
    eBTN_UNKNOWN,
    eBTN_A,       // 1
    eBTN_B,       // 2
    eBTN_X,       // 3
    eBTN_Y,       // 4
    eBTN_TL,      // 5
    eBTN_TR,      // 6
    eBTN_THUMBL,  // 7
    eBTN_THUMBR,  // 8
    eDPAD_UP,     // 9
    eDPAD_DOWN,   // 10
    eDPAD_LEFT,   // 11
    eDPAD_RIGHT,  // 12
    eBTN_SELECT,  // Warning: Maps to Android's Back button. (Closes keyboard/app.)
    eBTN_START,   // 14
    eBTN_MODE     // Warning: Windows/Linux: Launch Steam. Android: Maps to Home btn. (Closes app.)
};

enum eGamepadAxis {
    eAXIS_UNKNOWN,
    eAXIS_LX,  // 1: left thumbstick  -1to1 (right is positive)
    eAXIS_LY,  // 2: left thumbstick  -1to1 (up is positive)
    eAXIS_RX,  // 3: right thumbstick -1to1 (right is positive)
    eAXIS_RY,  // 4: right thumbstick -1to1 (up is positive)
    eAXIS_TL,  // 5: left trigger      0to1
    eAXIS_TR,  // 6: right trigger     0to1
};

// clang-format on
#endif

// -------------------------------
// WindowBase.h
// -------------------------------

/*
*--------------------------------------------------------------------------
* FIFO Buffer is used in the few cases where event messages need to be buffered or swapped.
* EventType contains a union struct of all possible message types that may be returned by getEvent.
* WindowBase is the abstract base class for all the platform-specific window classes.
*--------------------------------------------------------------------------
*/


#ifndef WINDOWBASE_H
#define WINDOWBASE_H

#include <cstring>
#include <cstdint>
#include <string>


// clang-format off
typedef unsigned int uint;

enum eAction { eUP, eDOWN, eMOVE };  // keyboard / mouse / touchscreen actions
enum eCursor{eArrow, eCaret, eResizeAll, eResizeNS, eResizeEW, eResizeNESW, eResizeNWSE, eHand, eWait, eProgress, eNotAllowed};

//========================Event Message=========================
struct EventType {
    enum Tag{NONE, MOUSE, KEY, TEXT, MOVE, RESIZE, FOCUS, TOUCH, CLOSE, GPAD_CONNECT, GPAD_BUTTON, GPAD_AXIS, UNKNOWN} tag; // event type
    union {
        struct {eAction action; int16_t x; int16_t y; uint8_t btn;} mouse;       // mouse move/click
        struct {eAction action; eKeycode keycode;                 } key;         // Keyboard key state
        struct {const char* str;                                  } text;        // Text entered
        struct {int16_t x; int16_t y;                             } move;        // Window moved
        struct {uint16_t width; uint16_t height;                  } resize;      // Window resize
        struct {bool has_focus;                                   } focus;       // Window gained/lost focus
        struct {eAction action; float x; float y; uint8_t id;     } touch;       // multi-touch display
        struct {uint8_t pad; bool active;                         } gp_connect;  // Gamepad connect/disconnect
        struct {uint8_t pad; uint8_t btn;  bool down;             } gp_button;   // Gamepad button state
        struct {uint8_t pad; uint8_t axis; float val;             } gp_axis;     // Gamepad axis value
        struct {                                                  } close;       // Window is closing
    };
    operator bool() const {return (tag!=NONE);}
};
//==============================================================
//======================== FIFO Buffer =========================  // Used for event message queue
class EventFIFO {
    static const int SIZE = 64;
    int head, tail;
    EventType buf[SIZE] = {};

  public:
    EventFIFO() : head(0), tail(0) {}
    bool isEmpty() const { return head == tail; }                                   // Check if queue is empty.
    void push(EventType const& item) { ++head; buf[head %= SIZE] = item; }          // Add item to queue
    EventType pop() { if(isEmpty()) return {}; ++tail; return buf[tail %= SIZE]; }  // Returns next event, or {} if queue is empty
};
//==============================================================
//=========================MULTI-TOUCH==========================
class CMTouch {
    struct CPointer{bool active; float x; float y;};
    static const int  MAX_POINTERS = 10;  // Max 10 fingers
    uint32_t touchID [MAX_POINTERS]{};    // finger-id lookup table (PC)
    CPointer Pointers[MAX_POINTERS]{};

  public:
    int count=0;  // number of active touch-id's (Android only)
    void Clear() { memset(this, 0, sizeof(*this)); }

    // Convert desktop-style touch-id's to an android-style finger-id.
    EventType Event_by_ID(eAction action, float x, float y, uint32_t findval, uint32_t setval) {
        for (uint32_t i = 0; i < MAX_POINTERS; ++i) {  // lookup finger-id
            if (touchID[i] == findval) {
                touchID[i] = setval;
                return Event(action, x, y, i);
            }
        }
        return {EventType::UNKNOWN};
    }

    EventType Event(eAction action, float x, float y, uint8_t id) {
        if (id >= MAX_POINTERS) return {};  // Exit if too many fingers
        CPointer& P                   = Pointers[id];
        if (action != eMOVE) P.active = (action == eDOWN);
        P.x                           = x;
        P.y                           = y;
        EventType e                   = {EventType::TOUCH};
        e.touch                       = {action, x, y, id};
        return e;
    }
};
//==============================================================
//========================== Gamepad ===========================
const int MAX_GAMEPADS = 4;
struct Gamepad {
    bool  active = false;
    bool  buttons[16] = {};
    float axes[8]     = {};
};
//==============================================================
//=========================== Mouse ============================
struct Mouse {
    struct {int16_t x; int16_t y;}pos = {};                                    // mouse position
    bool btn[6] = {};                                                          // mouse btn state
};
//==============================================================
 struct native_handle;
//======================Window base class=======================
class WindowBase {
protected:
    Mouse mouse;                                                               // mouse state
    bool keystate[256] = {};                                                   // keyboard state
    Gamepad gamepad[MAX_GAMEPADS];                                             // gamepad state

    EventFIFO eventFIFO;                                                       // Event message queue buffer
    EventType mouseEvent (eAction action, int16_t x, int16_t y, uint8_t btn);  // Mouse event
    EventType keyEvent   (eAction action, uint8_t key);                        // Keyboard event
    EventType textEvent  (const char* str);                                    // Text event
    EventType moveEvent  (int16_t x, int16_t y);                               // Window moved
    EventType resizeEvent(uint16_t width, uint16_t height);                    // Window resized
    EventType focusEvent (bool has_focus);                                     // Window gained/lost focus   
    EventType gpadConnect(uint8_t pad, bool active);                           // Gamepad connect/disconnect
    EventType gpadButton (uint8_t pad, uint8_t btn, bool down);                // Gamepad button event
    EventType gpadAxis   (uint8_t pad, uint8_t axis, float val);               // Gamepad axis events
    EventType closeEvent ();                                                   // Window closing

    float display_scale = 0.f;
    bool running;
    bool has_focus;                                                            // true if window has focus
    bool is_resized;                                                           // true if window has been resized
    bool fullscreen;                                                           // true if window is fullscreen
    struct shape_t { int16_t x; int16_t y; uint16_t width; uint16_t height; } shape = {};  // window shape
    std::string clipboard;

  public:
    WindowBase() : running(false), has_focus(false), is_resized(false), fullscreen(false){}
    virtual ~WindowBase() {}
    virtual void close() { eventFIFO.push(closeEvent()); }

    //--State query functions--
    //shape_t GetShape (){return shape;}                                           // return window shape in pixels
    void  getPosition(int16_t& x, int16_t& y) { x = shape.x; y = shape.y; }        // return window position
    void  getSize(int16_t& w, int16_t& h) { w = width(); h = height(); }           // return window size
    void  getSize(int32_t& w, int32_t& h) { w = width(); h = height(); }           // return window size
    bool  getKeyState(eKeycode key) { return keystate[key]; }                      // return true if key is pressed
    bool  getBtnState(uint8_t  btn) { return (btn < 6) ? mouse.btn[btn] : 0; }     // return true if mouse btn is pressed
    void  getMousePos(int16_t& x, int16_t& y) {x = mouse.pos.x; y = mouse.pos.y;}  // return mouse x,y position
    Gamepad& getGamepad(uint8_t pad) {return gamepad[pad];}                        // return the gamepad state

    bool isRunning() { return running; }
    uint width() {return shape.width; }
    uint height(){return shape.height;}
    bool resized() { bool resize = is_resized; is_resized = false; return resize; }
    float getScale() {return (display_scale>0)? display_scale : getDisplayScale();}
    void  setScale(float val) {display_scale = val;}

    virtual float getDisplayScale() {return 1.f;}
    virtual bool isFullscreen() {return fullscreen;}

    //--Clipboard--
    virtual const char* getClipboardText() {return clipboard.c_str(); }  // Fallback implementation works only locally.
    virtual void setClipboardText(const char* str) { clipboard = str; }  // Platform implementations overrides this.

    //--Control functions--
    virtual void showKeyboard(bool enabled) {}                    // Shows the Android soft-keyboard.
    virtual void setTitle(const char* title) {}
    virtual void setPosition(uint x, uint y) {}
    virtual void setSize(uint w, uint h) {}
    virtual native_handle* getNativeHandle() const = 0;           // For creating Vulkan/OpenGL Surface
    virtual void showImage(uint32_t* buf, uint32_t width, uint32_t height) {}
    virtual void setCursor(eCursor id) {}
    virtual void setFullscreen(bool enable) {}
    void setSizeScaled(uint w, uint h) {float s=getScale(); setSize(w*s, h*s);}

    //--Event loop--
    virtual EventType getEvent(bool wait_for_event = false) = 0;  // Fetch one event from the queue.
    bool processEvents(bool wait_for_event = false);              // Dispatch all waiting events to event handlers. Returns false if window is closing.
    bool processEvent (EventType e);                              // Dispatch/inject the given event to event handlers.
    bool pollEvents() { return processEvents(false); }            // Run continuously
    bool waitEvents() { return processEvents(true ); }            // Pause app when there are no events to process
    void Run(bool wait=true){ while(processEvents(wait)){} }      // Run message loop until window is closed.

    //-- Virtual Functions as event handlers --
    virtual void onMouse(eAction action, int16_t x, int16_t y, uint8_t btn) {}  // Callback for mouse events
    virtual void onKey(eAction action, eKeycode keycode) {}                     // Callback for keyboard events (keycodes)
    virtual void onText(const char *str) {}                                     // Callback for text typed events (text)
    virtual void onMove(int16_t x, int16_t y) {}                                // Callback for window move events
    virtual void onResize(uint16_t width, uint16_t height) {}                   // Callback for window resize events
    virtual void onFocus(bool hasFocus) {}                                      // Callback for window gain/lose focus events
    virtual void onTouch(eAction action, float x, float y, uint8_t id) {}       // Callback for Multi-touch events
    virtual void onGpadConnect(uint8_t pad, bool active) {}                     // Callback for Joystick connect/disconnect
    virtual void onGpadButton(uint8_t pad, uint8_t btn, bool down) {}           // Callback for Joystick button events
    virtual void onGpadAxis(uint8_t pad, uint8_t axis, float val) {}            // Callback for Joystick axis events
    virtual void onClose() {}                                                   // Callback for window closing event
    virtual void onFrame() {}                                                   // Callback for new frame event
    //virtual void onIdleEvent() {}                                               // Callback when idle
};
//==============================================================

#endif

// -------------------------------
// gamepads.h
// -------------------------------

// Gamepad button layout lookup table for Linux and Android.
// This file was generated by gamepads.py,
// using data derived from 'gamecontrollerdb.txt'.
// - Duplicates and problematic items were discarded.
// - Controllers with missing buttons were discarded.

#if defined(__linux__) && !defined(__ANDROID__)  // Linux, but not Android
#define LINUX
#endif

#if !defined(__linux__)  // Not for Win32
#define GAMEPAD_H
#endif

#ifndef GAMEPAD_H
#define GAMEPAD_H

#include <array>
#include <cstdint>

#ifdef LINUX
constexpr std::array gamepad_layout_list = {
//   A   B   X   Y   LB  RB  LS  RS  UP  DN  LE  RI  THUMBL  THUMBR  TRIGER  SEL START
    "b0  b1  b2  b3  b4  b5  b9  b10 h   h   h   h   a0  a1  a3  a4  a2  a5  b6  b7  ",  // 0
    "b1  b2  b0  b3  b4  b5  b10 b11 h   h   h   h   a0  a1  a2  a3  b6  b7  b8  b9  ",  // 1
    "b0  b1  b3  b4  b6  b7  b13 b14 h   h   h   h   a0  a1  a2  a3  a5  a4  b10 b11 ",  // 2
    "b1  b2  b0  b3  b4  b5  b10 b11 h   h   h   h   a0  a1  a2  a5  a3  a4  b8  b9  ",  // 3
    "b1  b0  b4  b3  b6  b7  b13 b14 h   h   h   h   a0  a1  a2  a3  b8  b9  b10 b11 ",  // 4
    "b2  b1  b3  b0  b4  b5  b10 b11 h   h   h   h   a0  a1  a2  a3  b6  b7  b8  b9  ",  // 5
    "b0  b1  b2  b3  b4  b5  b10 b11 h   h   h   h   a0  a1  a2  a3  b6  b7  b8  b9  ",  // 6
    "b0  b1  b3  b4  b6  b7  b13 b14 h   h   h   h   a0  a1  a2  a3  b8  b9  b10 b11 ",  // 7
    "b1  b0  b4  b3  b6  b7  b13 b14 h   h   h   h   a0  a1  a2  a3  a5  a4  b10 b11 ",  // 8
    "b0  b1  b2  b3  b4  b5  b7  b8  h   h   h   h   a0  a1  a2  a3  a5  a4  b9  b6  ",  // 9
    "b2  b3  b0  b1  b4  b6  b10 b11 h   h   h   h   a0  a1  a3  a2  b5  b7  b8  b9  ",  // 10
    "b1  b2  b0  b3  b4  b5  b10 b11 h   h   h   h   a0  a1  a2  a5  b6  b7  b8  b9  ",  // 11
    "b0  b1  b2  b3  b4  b5  b9  b10 h   h   h   h   a0  a1  a2  a3  a5  a4  b6  b7  ",  // 12
    "b2  b3  b0  b1  b4  b5  b8  b9  h   h   h   h   a0  a1  a3  a2  b6  b7  b10 b11 ",  // 13
    "b0  b1  b3  b4  b5  b2  b8  b9  h   h   h   h   a0  a1  a3  a4  a2  a5  b6  b7  ",  // 14
    "b14 b13 b15 b12 b10 b11 b1  b2  b4  b6  b7  b5  a0  a1  a2  a3  b8  b9  b0  b3  ",  // 15
    "b0  b1  b2  b3  b4  b5  b9  b10 b13 b14 b11 b12 a0  a1  a3  a4  a2  a5  b6  b7  ",  // 16
    "b0  b2  b1  b3  b4  b6  b10 b11 h   h   h   h   a0  a1  a2  a3  b5  b7  b8  b9  ",  // 17
    "b1  b0  b4  b3  b6  b7  b13 b14 h   h   h   h   a0  a1  a3  a4  b8  b9  b10 b11 ",  // 18
    "b0  b1  b3  b4  b6  b7  b12 b13 h   h   h   h   a0  a1  a2  a3  a5  a4  b10 b11 ",  // 19
    "b0  b1  b2  b3  b4  b5  b11 b12 h   h   h   h   a0  a1  a2  a3  b6  b7  b8  b9  ",  // 20
    "b0  b1  b3  b2  b5  b6  b12 b13 b14 b15 b16 b17 a0  a1  a2  a3  b7  b8  b9  b10 ",  // 21
    "b0  b1  b2  b3  b4  b5  b8  b9  h   h   h   h   a0  a1  a2  a5  a3  a4  b6  b7  ",  // 22
    "b2  b1  b3  b0  b6  b7  b10 b11 h   h   h   h   a0  a1  a3  a2  b4  b5  b8  b9  ",  // 23
    "b1  b0  b4  b3  b6  b7  b13 b14 h   h   h   h   a0  a1  a2  a3  a4  a5  b10 b11 ",  // 24
    "b1  b2  b0  b3  b4  b5  b10 b11 h   h   h   h   a0  a1  a3  a2  b6  b7  b8  b9  ",  // 25
    "b0  b1  b2  b3  b4  b5  b10 b11 h   h   h   h   a0  a1  a2  a3  a5  a4  b8  b9  ",  // 26
    "b0  b1  b3  b4  b6  b7  b13 b14 h   h   h   h   a0  a1  a2  a3  a6  a5  b17 b11 ",  // 27
    "b2  b1  b3  b0  b6  b7  b10 b11 h   h   h   h   a0  a1  a2  a3  b4  b5  b9  b8  ",  // 28
    "b0  b1  b2  b3  b4  b5  b7  b8  h   h   h   h   a0  a1  a2  a5  a3  a4  b14 b6  ",  // 29
    "b1  b2  b0  b3  b4  b5  b10 b11 h   h   h   h   a0  a1  a2  a4  b6  b7  b8  b9  ",  // 30
    "b1  b0  b4  b3  b6  b7  b13 b14 h   h   h   h   a0  a1  a2  a3  a5  a5  b10 b11 ",  // 31
    "b0  b1  b2  b3  b4  b5  b10 b11 h   h   h   h   a0  a1  a2  a5  a3  a4  b6  b7  ",  // 32
    "b1  b0  b3  b4  b6  b7  b13 b14 h   h   h   h   a0  a1  a2  a4  b8  b9  b10 b11 ",  // 33
    "b0  b1  b2  b3  b4  b5  b6  b7  h   h   h   h   a0  a1  a2  a3  a5  a4  b9  b8  ",  // 34
    "b0  b1  b3  b2  b4  b5  b9  b10 h   h   h   h   a0  a1  a3  a4  a2  a5  b6  b7  ",  // 35
    "b1  b2  b0  b3  b4  b5  b10 b11 h   h   h   h   a0  a1  a2  a4  a3  -a3 b8  b9  ",  // 36
    "b2  b3  b0  b1  b4  b5  b8  b9  h   h   h   h   a0  a1  a2  a3  b6  b7  b10 b11 ",  // 37
    "b2  b1  b3  b0  b6  b7  b10 b11 b12 b14 b15 b13 a0  a1  a5  a2  b4  b5  b8  b9  ",  // 38
    "b0  b1  b3  b4  b6  b7  b13 b16 h   h   h   h   a0  a1  a2  a3  b8  b9  b10 b11 ",  // 39
    "b2  b1  b3  b0  b6  b7  b9  b10 h   h   h   h   a0  a1  a3  a2  b4  b5  b8  b11 ",  // 40
    "b0  b1  b2  b3  b4  b6  b10 b11 h   h   h   h   a0  a1  a3  a2  b5  b7  b8  b9  ",  // 41
    "b0  b1  b2  b3  b4  b5  b9  b10 h   h   h   h   a0  a1  a3  a4  a2  a5  b6  b7  ",  // 42
    "b3  b4  b0  b1  b6  b7  b2  b5  h   h   h   h   a0  a1  a2  a3  b8  b9  b10 b11 ",  // 43
    "b2  b1  b3  b0  b4  b5  b10 b11 h   h   h   h   a0  a1  a3  a2  b6  b7  b8  b9  ",  // 44
    "b2  b3  b0  b1  b4  b6  b10 b11 h   h   h   h   a0  a1  a2  a3  b5  b7  b8  b9  ",  // 45
    "b1  b0  b3  b2  b9  b10 b7  b8  b11 b12 b13 b14 a0  a1  a2  a3  a4  a5  b4  b6  ",  // 46
    "b0  b1  b3  b2  b4  b5  b11 b12 b13 b14 b15 b16 a0  a1  a2  a3  b6  b7  b8  b9  ",  // 47
    "b0  b1  b2  b3  b4  b6  b12 b11 h   h   h   h   a0  a1  a3  a2  b5  b7  b8  b10 ",  // 48
    "b0  b1  b2  b3  b4  b5  b8  b9  h   h   h   h   a0  a1  a2  a3  a5  a4  __  b7  ",  // 49
    "b0  b1  b2  b3  b4  b5  b11 b12 h   h   h   h   a0  a1  a3  a2  b6  b7  b8  b9  ",  // 50
    "b0  b3  b1  b2  b4  b5  b6  b7  b8  b9  b10 b11 a0  a1  a3  a4  a2  a5  __  __  ",  // 51
    "b0  b1  b2  b3  b4  b5  b8  b9  h   h   h   h   a0  a1  a2  a3  b6  b7  b10 b11 ",  // 52
    "b0  b1  b2  b3  b4  b5  b9  b10 b14 b13 b14 b13 a0  a1  a3  a4  a2  a5  b6  b7  ",  // 53
    "b1  b0  b2  b3  b4  b5  b10 b11 h   h   h   h   a0  a1  a2  a3  b6  b7  b8  b9  ",  // 54
    "b0  b1  b3  b2  b4  b5  b11 b12 h   h   h   h   a0  a1  a3  a4  a2  a5  b8  b9  ",  // 55
    "b1  b2  b0  b3  b4  b5  b10 b11 h   h   h   h   a0  a1  a2  a5  b6  a4  b8  b9  ",  // 56
    "b2  b3  b1  b0  b4  b5  b10 b11 h   h   h   h   a0  a1  a2  a3  b6  b7  b8  b9  ",  // 57
    "b1  b2  b0  b3  b6  b7  b10 b11 h   h   h   h   a0  a1  a3  a2  b4  b5  b8  b12 ",  // 58
    "b1  b2  b0  b3  b4  b5  b10 b11 h   h   h   h   a0  a1  a3  a4  a2  b7  b8  b9  ",  // 59
    "b2  b1  b3  b0  b6  b7  b9  b10 b12 b14 b15 b13 a0  a1  a2  a3  b4  b5  b8  b11 ",  // 60
    "b2  b3  b4  b5  b6  b7  b13 b14 -a5 a5  -a4 a4  a0  a1  a2  a3  a7  a6  b10 b11 ",  // 61
    "b2  b1  b3  b0  b6  b7  b10 b11 b12 b14 b15 b13 a0  a1  a2  a3  b4  b5  b9  b8  ",  // 62
    "b0  b1  b3  b2  b4  b5  b10 b11 h   h   h   h   a0  a1  a5  a2  b6  b7  b8  b9  ",  // 63
    "b0  b2  b1  b3  b4  b6  b10 b11 h   h   h   h   a0  a1  a2  a3  b8  b9  __  __  ",  // 64
    "b0  b2  b1  b3  b4  b6  b11 b12 h   h   h   h   a0  a1  a2  a3  b5  b7  b9  b10 ",  // 65
    "b2  b1  b3  b0  b4  b5  b10 b11 h   h   h   h   a0  a1  a3  a4  b6  b7  b8  b9  ",  // 66
    "b0  b1  b2  b3  b6  b7  b10 b11 h   h   h   h   a0  a1  a3  a2  b4  b5  b8  b9  ",  // 67
    "b1  b0  b3  b2  b4  b5  b11 b12 h   h   h   h   a0  a1  a2  a3  b6  b7  b8  b9  ",  // 68
    "b0  b1  b2  b3  b4  b5  b8  b9  h   h   h   h   a0  a1  a3  a4  a2  a5  b6  b7  ",  // 69
    "b0  b1  b3  b4  b6  b7  b13 b14 h   h   h   h   a0  a1  a2  a5  a7  a6  b10 b11 ",  // 70
};

static struct gamepad_index {
    uint16_t VID;     // Vendor ID
    uint16_t PID;     // Product ID
    uint8_t  BUS;     // USB(3) or Bluetooth(5)
    uint8_t  inx;     // Layout index
} gamepad_index[] = {
    {0x0079,0x18d4,3, 0},  // USB: GPD Win 2 Controller
    {0x044f,0xb326,3, 0},  // USB: Thrustmaster GP XID
    {0x045e,0x028e,3, 0},  // USB: Be1 GC101 Xbox 360
    {0x045e,0x028e,5, 0},  // BT : Microsoft Xbox One Elite 2
    {0x045e,0x02d1,3, 0},  // USB: Microsoft Xbox One
    {0x045e,0x02dd,3, 0},  // USB: Microsoft Xbox One
    {0x045e,0x02e3,3, 0},  // USB: Microsoft Xbox One Elite
    {0x045e,0x02ea,3, 0},  // USB: Microsoft Xbox One
    {0x045e,0x02ea,6, 0},  // VRT: Xbox One S Controller
    {0x045e,0x0b00,3, 0},  // USB: Microsoft Xbox One Elite 2
    {0x045e,0x0b12,3, 0},  // USB: Microsoft Xbox Series Controller
    {0x045e,0x0b12,6, 0},  // VRT: Microsoft Xbox One
    {0x046d,0xc21d,3, 0},  // USB: Logitech F310
    {0x046d,0xc21e,3, 0},  // USB: Logitech F510
    {0x046d,0xc21f,3, 0},  // USB: Logitech F710
    {0x0738,0x4716,3, 0},  // USB: Mad Catz Xbox 360 Controller
    {0x0e6f,0x011f,3, 0},  // USB: Rock Candy
    {0x0e6f,0x0131,3, 0},  // USB: PDP EA Sports Controller
    {0x0e6f,0x0139,3, 0},  // USB: Afterglow Prismatic Controller
    {0x0e6f,0x0164,3, 0},  // USB: PDP Battlefield One
    {0x0e6f,0x0213,3, 0},  // USB: Afterglow Xbox 360 Controller
    {0x0e6f,0x02a7,3, 0},  // USB: PDP Xbox One Raven Black
    {0x0e6f,0x02b8,3, 0},  // USB: PDP Afterglow Xbox One Controller
    {0x0e6f,0x02c8,3, 0},  // USB: PDP Kingdom Hearts Controller
    {0x0e6f,0x02d8,3, 0},  // USB: PDP Xbox Series Controller
    {0x0e6f,0x02ef,3, 0},  // USB: PDP Xbox Series Kinetic Wired Controller
    {0x0e6f,0x02f1,3, 0},  // USB: PDP Xbox Atomic
    {0x0e6f,0x0301,3, 0},  // USB: Logic3 Controller
    {0x0e6f,0x0315,3, 0},  // USB: Xbox 360 Controller
    {0x0e6f,0x0401,3, 0},  // USB: Gamestop Logic3 Controller
    {0x0e6f,0x0413,3, 0},  // USB: Xbox Controller
    {0x0f0d,0x0067,3, 0},  // USB: Horipad One
    {0x0f0d,0x0150,3, 0},  // USB: Hori Fighting Commander Octa Xbox One
    {0x0f0d,0x0185,3, 0},  // USB: Hori Split Pad Fit
    {0x1038,0x1430,3, 0},  // USB: SteelSeries Stratus Duo
    {0x1038,0x1431,3, 0},  // USB: SteelSeries Stratus Duo
    {0x10f5,0x7008,6, 0},  // VRT: Turtle Beach Recon
    {0x146b,0x0609,3, 0},  // USB: Nacon Asymmetric Wireless PS4 Controller
    {0x1532,0x0a03,3, 0},  // USB: Razer Wildcat
    {0x1532,0x0a14,3, 0},  // USB: Razer Wolverine Ultimate Xbox
    {0x1689,0xfe00,3, 0},  // USB: Razer Sabertooth
    {0x1bad,0xf016,3, 0},  // USB: Mad Catz Xbox 360 Controller
    {0x1bad,0xf501,3, 0},  // USB: Hori Pad EX Turbo 2
    {0x20d6,0x2002,3, 0},  // USB: PowerA Xbox One Controller
    {0x20d6,0x2005,3, 0},  // USB: PowerA Xbox Series Controller
    {0x20d6,0x200b,3, 0},  // USB: PowerA Xbox Series Controller
    {0x20d6,0x200f,3, 0},  // USB: PowerA Xbox Series Controller
    {0x20d6,0x2802,3, 0},  // USB: PowerA Xbox One Controller
    {0x20d6,0x4001,3, 0},  // USB: PowerA Fusion Pro 2 Controller
    {0x20d6,0x4002,3, 0},  // USB: PowerA Xbox One Spectra Infinity
    {0x20d6,0x4005,3, 0},  // USB: PowerA Advantage Xbox Series Controller
    {0x24c6,0x5300,3, 0},  // USB: PowerA
    {0x24c6,0x531a,3, 0},  // USB: PowerA Mini Pro Ex
    {0x24c6,0x541a,3, 0},  // USB: PowerA Xbox One Mini Controller
    {0x24c6,0x543a,3, 0},  // USB: PowerA 1428124-01
    {0x24c6,0x581a,3, 0},  // USB: PowerA Xbox One
    {0x24c6,0x5b02,3, 0},  // USB: Thrustmaster GPX
    {0x24c6,0x5d04,3, 0},  // USB: Razer Sabertooth
    {0x24c6,0xfafe,3, 0},  // USB: Rock Candy Xbox 360 Controller
    {0x28de,0x11ff,3, 0},  // USB: Steam Virtual Gamepad
    {0x294b,0x3004,3, 0},  // USB: Snakebyte Xbox Series Controller
    {0x2dc8,0x2000,3, 0},  // USB: 8BitDo Pro 2 for Xbox
    {0x2dc8,0x2000,6, 0},  // VRT: 8BitDo Pro 2 for Xbox
    {0x2dc8,0x3106,3, 0},  // USB: 8BitDo Adapter 2
    {0x2dc8,0x310a,3, 0},  // USB: 8BitDo Ultimate 2C
    {0x2e24,0x1688,3, 0},  // USB: Hyperkin X91
    {0x2f24,0x0091,3, 0},  // USB: EasySMX ESM-9101
    {0x2f24,0x00f7,3, 0},  // USB: Mayflash Magic S Pro
    {0x3285,0x0607,3, 0},  // USB: Nacon GC-100
    {0xdead,0xbeef,6, 0},  // VRT: Hidromancer Controller
    {0x0079,0x1800,3, 1},  // USB: Mayflash Wii U Pro Adapter
    {0x0079,0x181a,3, 1},  // USB: Venom PS4 Arcade Joystick
    {0x0079,0x18d2,3, 1},  // USB: Mayflash Magic NS
    {0x044f,0xd007,3, 1},  // USB: Thrustmaster T Mini
    {0x044f,0xd009,3, 1},  // USB: Thrustmaster Run N Drive PlayStation Controller
    {0x046d,0xc216,3, 1},  // USB: Logitech Dual Action
    {0x046d,0xc218,3, 1},  // USB: Logitech RumblePad 2
    {0x046d,0xc219,3, 1},  // USB: Logitech Cordless RumblePad 2
    {0x046d,0xcad1,3, 1},  // USB: Logitech Chillstream
    {0x046d,0xcad2,3, 1},  // USB: Precision Controller
    {0x0738,0x3180,3, 1},  // USB: Mad Catz FightStick Alpha PS3
    {0x0738,0x3250,3, 1},  // USB: Mad Catz Fightpad Pro PS3
    {0x0738,0x3384,3, 1},  // USB: Mad Catz Fightstick TE S PS3
    {0x0738,0x5266,5, 1},  // BT : Mad Catz CTRLR
    {0x0738,0x8180,3, 1},  // USB: Mad Catz FightStick Alpha PS4
    {0x0c12,0x0e21,3, 1},  // USB: Brook Mars PS4 Controller
    {0x0c12,0x0e30,3, 1},  // USB: Brook Audio Fighting Board PS3
    {0x0e6f,0x011e,3, 1},  // USB: Rock Candy PS3 Controller
    {0x0e6f,0x0128,3, 1},  // USB: PDP PS3 Rock Candy Controller
    {0x0e6f,0x012f,3, 1},  // USB: PDP Wired PS3 Controller
    {0x0e6f,0x0130,3, 1},  // USB: EA Sports PS3 Controller
    {0x0e6f,0x0180,3, 1},  // USB: Faceoff Pro Nintendo Switch Controller
    {0x0e6f,0x0181,3, 1},  // USB: Faceoff Deluxe Pro Nintendo Switch Controller
    {0x0e6f,0x0184,3, 1},  // USB: Faceoff Deluxe Nintendo Switch Controller
    {0x0e6f,0x0187,3, 1},  // USB: Rock Candy Nintendo Switch Controller
    {0x0e6f,0x0188,3, 1},  // USB: Afterglow Deluxe Nintendo Switch Controller
    {0x0e6f,0x0214,3, 1},  // USB: PS3 Controller
    {0x0e8f,0x310d,3, 1},  // USB: SZMY Power 3 Turbo
    {0x0e8f,0x3114,3, 1},  // USB: SZMY Power PS3
    {0x0f0d,0x0009,3, 1},  // USB: Natec Genesis P44
    {0x0f0d,0x0011,3, 1},  // USB: Hori Real Arcade Pro 3
    {0x0f0d,0x004d,3, 1},  // USB: Hori Gem Pad 3
    {0x0f0d,0x006b,3, 1},  // USB: Hori Real Arcade Pro 4
    {0x0f0d,0x006e,3, 1},  // USB: Horipad 4 PS3
    {0x0f0d,0x0085,3, 1},  // USB: Hori Fighting Commander PS3
    {0x0f0d,0x00c1,3, 1},  // USB: Horipad Nintendo Switch Controller
    {0x11c0,0x5503,3, 1},  // USB: Acrux Gamepad
    {0x12bd,0xc003,3, 1},  // USB: Joypad Alpha Shock
    {0x146b,0x0902,3, 1},  // USB: Bigben
    {0x1532,0x0402,3, 1},  // USB: Razer Panthera PS3
    {0x1a34,0x0836,3, 1},  // USB: PS3 Controller
    {0x20bc,0x5656,3, 1},  // USB: GameSir T4w
    {0x20d6,0xa710,3, 1},  // USB: Mayflash Magic NS
    {0x20d6,0xa711,3, 1},  // USB: PowerA Core Controller
    {0x20d6,0xa712,3, 1},  // USB: PowerA Fusion Nintendo Switch Fight Pad
    {0x20d6,0xa713,3, 1},  // USB: PowerA Nintendo Switch Controller
    {0x20d6,0xa714,3, 1},  // USB: PowerA Spectra Nintendo Switch Controller
    {0x20d6,0xca6d,3, 1},  // USB: PowerA Pro Ex
    {0x2185,0x0102,3, 1},  // USB: Final Fantasy XIV Online Controller
    {0x25f0,0x83c1,3, 1},  // USB: Goodbetterbest Controller
    {0x2c22,0x2302,3, 1},  // USB: Qanba Obsidian Arcade Joystick PS3
    {0x2c22,0x2502,3, 1},  // USB: Qanba Dragon Arcade Joystick PS3
    {0x62dd,0xa715,3, 1},  // USB: PowerA Fusion Nintendo Switch Arcade Stick
    {0x62dd,0xa716,3, 1},  // USB: PowerA Fusion Pro Nintendo Switch Controller
    {0x6469,0x6469,5, 1},  // BT : idroidcon Controller
    {0x0111,0x1419,5, 2},  // BT : SteelSeries Stratus XL
    {0x0111,0x1431,5, 2},  // BT : SteelSeries Stratus Duo
    {0x03f0,0x038d,3, 2},  // USB: HyperX Clutch
    {0x045e,0x02fd,5, 2},  // BT : Xbox One Controller
    {0x045e,0x0b13,3, 2},  // USB: Xbox Series Controller
    {0x045e,0x0b13,5, 2},  // BT : Xbox Series Controller
    {0x045e,0x0b20,5, 2},  // BT : Xbox Wireless Controller
    {0x045e,0x0b22,5, 2},  // BT : Xbox One Elite 2 Controller
    {0x0502,0x1309,3, 2},  // USB: Anbernic RG P01
    {0x05ac,0x022d,5, 2},  // BT : GameSir G4s
    {0x05ac,0x061a,3, 2},  // USB: GameSir-T3 2.02
    {0x0b05,0x7905,3, 2},  // USB: ASUS ROG Kunai 3
    {0x0b05,0x7906,5, 2},  // BT : ASUS ROG Kunai 3
    {0x0f0d,0x0196,5, 2},  // BT : Horipad Steam
    {0x0f0d,0x01ab,3, 2},  // USB: Horipad Steam
    {0x11c3,0x9107,3, 2},  // USB: Be1 GC101 Controller 1.03
    {0x1532,0x0705,3, 2},  // USB: Razer Raiju Mobile
    {0x1915,0x7856,3, 2},  // USB: Uniplay U6
    {0x1949,0x0402,5, 2},  // BT : Amazon Fire Controller
    {0x20bc,0x5500,3, 2},  // USB: GameSir G3w
    {0x24c6,0x891a,5, 2},  // BT : MOGA XP5X Plus
    {0x24c6,0x891b,3, 2},  // USB: BDA MOGA XP5X Plus
    {0x24c6,0x892a,5, 2},  // BT : MOGA XP5A Plus
    {0x24c6,0x892b,3, 2},  // USB: MOGA XP5A Plus
    {0x2563,0x0526,3, 2},  // USB: Shanwan Gamepad
    {0x27f8,0x0bbf,3, 2},  // USB: Razer Kishi
    {0x2dc8,0x2101,3, 2},  // USB: 8BitDo Xbox One SN30 Pro
    {0x2dc8,0x2101,5, 2},  // BT : 8BitDo Xbox One SN30 Pro
    {0x2dc8,0x3011,3, 2},  // USB: 8BitDo Ultimate Wired
    {0x2dc8,0x3012,3, 2},  // USB: 8BitDo Ultimate Wireless
    {0x2dc8,0x3012,5, 2},  // BT : 8BitDo Ultimate
    {0x2dc8,0x3013,3, 2},  // USB: 8BitDo Ultimate Wireless
    {0x2dc8,0x3015,3, 2},  // USB: 8BitDo Ultimate C
    {0x2dc8,0x3016,3, 2},  // USB: 8BitDo Ultimate C
    {0x2dc8,0x3017,3, 2},  // USB: 8BitDo Ultimate C
    {0x2dc8,0x301b,5, 2},  // BT : 8BitDo Ultimate 2C
    {0x2dc8,0x301d,3, 2},  // USB: 8BitDo Ultimate 2C
    {0x3285,0x0305,5, 2},  // BT : Nacon MG-X Pro
    {0x3537,0x1007,3, 2},  // USB: Anbernic RG P01
    {0x3537,0x1046,5, 2},  // BT : Anbernic RG P01
    {0x358a,0x0102,3, 2},  // USB: Backbone One
    {0x358a,0x0202,3, 2},  // USB: Backbone One
    {0x358a,0x0203,3, 2},  // USB: Backbone One
    {0x358a,0x0204,3, 2},  // USB: Backbone One
    {0x0079,0x181b,3, 3},  // USB: Venom PS4 Arcade Joystick
    {0x044f,0xd00e,3, 3},  // USB: Thrustmaster eSwap Pro Controller
    {0x054c,0x05c4,5, 3},  // BT : PS4 Controller
    {0x054c,0x09cc,3, 3},  // USB: PS4 Controller
    {0x054c,0x09cc,5, 3},  // BT : PS4 Controller
    {0x054c,0x0ba0,3, 3},  // USB: PS4 Controller
    {0x054c,0x0ce6,3, 3},  // USB: PS5 Controller
    {0x054c,0x0ce6,5, 3},  // BT : PS5 Controller
    {0x054c,0x0df2,3, 3},  // USB: PS5 Controller
    {0x054c,0x0df2,5, 3},  // BT : PS5 Controller
    {0x054c,0x0e5f,3, 3},  // USB: PS5 Access Controller
    {0x0738,0x8250,3, 3},  // USB: Mad Catz Fightpad Pro PS4
    {0x0738,0x8384,3, 3},  // USB: Mad Catz Fightstick TE S PS4
    {0x0c12,0x0e10,3, 3},  // USB: Zeroplus P4
    {0x0c12,0x0e20,3, 3},  // USB: Brook Mars PS4 Controller
    {0x0c12,0x0e31,3, 3},  // USB: Brook Audio Fighting Board PS4
    {0x0c12,0x1e10,3, 3},  // USB: Zeroplus P4
    {0x0f0d,0x0066,3, 3},  // USB: Horipad 4 PS4
    {0x0f0d,0x006a,3, 3},  // USB: Hori Real Arcade Pro 4
    {0x0f0d,0x0084,3, 3},  // USB: Hori Fighting Commander
    {0x0f0d,0x00ee,3, 3},  // USB: Horipad Mini 4
    {0x146b,0x0d01,3, 3},  // USB: Revolution Pro Controller
    {0x146b,0x0d13,3, 3},  // USB: Revolution Pro Controller 3
    {0x1532,0x0401,3, 3},  // USB: Razer Panthera PS4
    {0x1532,0x1000,3, 3},  // USB: Razer Raiju
    {0x1532,0x1008,3, 3},  // USB: Razer Panthera PS4 Evo Arcade Stick
    {0x1532,0x100a,5, 3},  // BT : Razer Raiju Tournament Edition
    {0x1532,0x100b,3, 3},  // USB: Razer Wolverine PS5 Controller
    {0x1532,0x1100,3, 3},  // USB: Razer Raion PS4 Fightpad
    {0x20d6,0x792a,3, 3},  // USB: BDA PS4 Fightpad
    {0x2c22,0x2300,3, 3},  // USB: Qanba Obsidian Arcade Joystick PS4
    {0x2c22,0x2500,3, 3},  // USB: Qanba Dragon Arcade Joystick PS4
    {0x3285,0x0d17,3, 3},  // USB: Nacon Revolution 5 Pro
    {0x3285,0x0d19,3, 3},  // USB: Nacon Revolution 5 Pro
    {0x7545,0x0104,3, 3},  // USB: SZMY Power DS4 Wired Controller
    {0x1002,0x9000,3, 4},  // USB: 8BitDo FC30 Pro
    {0x2002,0x9000,3, 4},  // USB: 8BitDo NES30 Pro
    {0x2dc8,0x2862,5, 4},  // BT : 8BitDo SN30 Pro
    {0x2dc8,0x2865,5, 4},  // BT : 8BitDo N30 Pro 2
    {0x2dc8,0x3101,3, 4},  // USB: 8BitDo Receiver
    {0x2dc8,0x3102,3, 4},  // USB: 8BitDo Receiver
    {0x2dc8,0x3103,3, 4},  // USB: 8BitDo Receiver
    {0x2dc8,0x3104,3, 4},  // USB: 8BitDo Receiver
    {0x2dc8,0x3810,5, 4},  // BT : 8BitDo FC30 Pro
    {0x2dc8,0x5111,3, 4},  // USB: 8BitDo Lite SE
    {0x2dc8,0x5111,5, 4},  // BT : 8BitDo Lite SE
    {0x2dc8,0x6000,3, 4},  // USB: 8BitDo SF30 Pro
    {0x2dc8,0x6000,5, 4},  // BT : 8BitDo SF30 Pro
    {0x2dc8,0x6002,3, 4},  // USB: 8BitDo SN30 Pro Plus
    {0x2dc8,0x6101,5, 4},  // BT : 8BitDo SN30 Pro
    {0x2dc8,0x6102,5, 4},  // BT : 8BitDo SN30 Pro Plus
    {0x2dc8,0x9015,3, 4},  // USB: 8BitDo N30 Pro 2
    {0x3820,0x0009,5, 4},  // BT : 8BitDo NES30 Pro
    {0x05ac,0x055b,3, 5},  // USB: GameSir G3w
    {0x0e8f,0x0003,3, 5},  // USB: PS3 Controller
    {0x0e8f,0x0008,3, 5},  // USB: Gasia PlayStation Gamepad
    {0x0f0d,0x00aa,3, 5},  // USB: Hori Real Arcade Pro for Nintendo Switch
    {0x11c0,0x9105,3, 5},  // USB: Torid
    {0x11c1,0x9101,3, 5},  // USB: EasySMX
    {0x11c2,0x9107,3, 5},  // USB: Be1 GC101 Controller 1.03
    {0x11ff,0x3331,3, 5},  // USB: PC Controller
    {0x145f,0x01c5,3, 5},  // USB: Trust Gamepad
    {0x145f,0x0231,3, 5},  // USB: PS3 Controller
    {0x20bc,0x1264,3, 5},  // USB: Betop Controller
    {0x20e8,0x5860,3, 5},  // USB: Cideko AK08b
    {0x2563,0x0523,3, 5},  // USB: ShanWan Gamepad
    {0x2563,0x0575,3, 5},  // USB: Ipega PG 9099
    {0x25f0,0x83c3,3, 5},  // USB: GT VX2
    {0x25f0,0xc121,3, 5},  // USB: Shanwan Gioteck PS3 Controller
    {0x2f24,0x002d,3, 5},  // USB: JYS Adapter
    {0x7545,0x1122,3, 5},  // USB: SZMY Power Gamepad
    {0x057e,0x2009,5, 6},  // BT : Nintendo Switch Pro Controller
    {0x0c45,0x4320,3, 6},  // USB: XEOX SL6556 BK
    {0x0f0d,0x00f6,5, 6},  // BT : Horipad Switch Pro Controller
    {0x11c9,0x55f0,3, 6},  // USB: HJC Gamepad
    {0x1345,0x1000,3, 6},  // USB: Genius Maxfire Grandias 12
    {0x146b,0x0c01,3, 6},  // USB: Nacon GC 400ES
    {0x1a34,0x0809,3, 6},  // USB: SL6566
    {0x4f4d,0x4554,5, 6},  // BT : Mocute 053X
    {0x5347,0x6d61,5, 6},  // BT : GameStop Gamepad
    {0x694c,0x7250,5, 6},  // BT : Nintendo Switch Controller
    {0x0079,0x181c,3, 7},  // USB: Mobapad Chitu HD
    {0x04e8,0x046e,5, 7},  // BT : Mocute 053X M59
    {0x1949,0x0403,5, 7},  // BT : Ipega PG9099
    {0x20bc,0x504d,3, 7},  // USB: Beitong A1T2 BFM
    {0x20bc,0x5500,5, 7},  // BT : Betop AX1 BFM
    {0x2dc8,0x3100,3, 7},  // USB: 8BitDo Adapter
    {0x8555,0x061b,3, 7},  // USB: GameSir G4 Pro
    {0x2dc8,0x3010,3, 8},  // USB: 8BitDo Pro 2
    {0x2dc8,0x3820,5, 8},  // BT : 8BitDo NES30 Pro
    {0x2dc8,0x5112,5, 8},  // BT : 8BitDo Lite 2
    {0x2dc8,0x6006,3, 8},  // USB: 8BitDo Pro 2
    {0x2dc8,0x6006,5, 8},  // BT : 8BitDo Pro 2
    {0x2dc8,0x6007,3, 8},  // USB: 8BitDo Ultimate Wireless
    {0x2dc8,0x6100,5, 8},  // BT : 8BitDo SF30 Pro
    {0x0171,0x0419,5, 9},  // BT : Amazon Luna Controller
    {0x0b05,0x4500,5, 9},  // BT : ASUS Gamepad
    {0x20d6,0x0dad,5, 9},  // BT : Moga Pro
    {0x20d6,0x6271,5, 9},  // BT : Moga Pro 2
    {0x20d6,0x89e5,5, 9},  // BT : Moga 2
    {0x07b5,0x0312,3,10},  // USB: Mega World Logic 3 Controller
    {0x07b5,0x0315,3,10},  // USB: Impact
    {0x0f30,0x0110,3,10},  // USB: Jess Tech Dual Analog Rumble
    {0x0f30,0x0111,3,10},  // USB: Jess Tech Colour Rumble Pad
    {0x0f30,0x0112,3,10},  // USB: Saitek P380
    {0x044f,0xb323,3,11},  // USB: Thrustmaster Dual Trigger PlayStation Controller
    {0x044f,0xd008,3,11},  // USB: Thrustmaster Run N Drive PlayStation Controller
    {0x11c0,0x4001,3,11},  // USB: PS4 Controller
    {0x1345,0x3008,3,11},  // USB: NYKO CORE
    {0x2c22,0x2010,3,11},  // USB: Qanba Drone 2 Arcade Joystick PS5
    {0x1532,0x0900,3,12},  // USB: Razer Serval
    {0x1532,0x0900,5,12},  // BT : Razer Serval
    {0x18d1,0x9400,3,12},  // USB: Google Stadia Controller
    {0x18d1,0x9400,5,12},  // BT : Google Stadia Controller
    {0x05ef,0x0003,3,13},  // USB: InterAct AxisPad
    {0x06a3,0x0109,3,13},  // USB: Saitek P880
    {0x06a3,0xff0c,3,13},  // USB: Saitek P2500 Force Rumble
    {0x0c12,0x0005,3,13},  // USB: InterAct AxisPad
    {0x045e,0x0202,3,14},  // USB: Xbox Controller
    {0x045e,0x0285,3,14},  // USB: Microsoft Xbox
    {0x045e,0x0289,3,14},  // USB: Microsoft Xbox
    {0xffff,0xffff,3,14},  // USB: Xbox Controller
    {0x054c,0x0268,3,15},  // USB: PS3 Controller
    {0x054c,0x0268,5,15},  // BT : PS3 Controller
    {0x054c,0x0268,6,15},  // VRT: PS3 Controller
    {0x4c50,0x5453,5,15},  // BT : PS3 Controller
    {0x045e,0x0291,3,16},  // USB: Xbox 360 Controller
    {0x045e,0x02a1,3,16},  // USB: Xbox 360 Controller
    {0x045e,0x0719,3,16},  // USB: Xbox 360 Controller
    {0x1689,0xfd01,3,16},  // USB: Razer Onza Classic Edition
    {0x044f,0xb304,3,17},  // USB: Thrustmaster Firestorm Dual Power
    {0x044f,0xb312,3,17},  // USB: Thrustmaster Vibrating Gamepad
    {0x044f,0xb315,3,17},  // USB: Thrustmaster Dual Analog 3.2
    {0x044f,0xb320,3,17},  // USB: Thrustmaster Dual Trigger
    {0x2dc8,0x6001,3,18},  // USB: 8BitDo SN30 Pro
    {0x2dc8,0x6101,3,18},  // USB: 8BitDo SN30 Pro
    {0x2dc8,0x9012,3,18},  // USB: 8BitDo SN30 Pro
    {0x04b4,0x2411,3,19},  // USB: Flydigi Vader 2
    {0x04b4,0x2412,3,19},  // USB: Flydigi Vader 2
    {0x1915,0x0040,5,19},  // BT : Flydigi Vader 2
    {0x0001,0x0001,5,20},  // BT : Nintendo Wii Remote
    {0x0e6f,0x02a8,3,20},  // USB: PDP Xbox One Controller
    {0x0f0d,0x0086,3,20},  // USB: Hori Fighting Commander Xbox 360
    {0x057e,0x2006,6,21},  // VRT: Nintendo Switch Combined Joy-Cons
    {0x057e,0x2008,6,21},  // VRT: Nintendo Switch Combined Joy-Cons
    {0x694e,0x6e65,6,21},  // VRT: Nintendo Switch Combined Joy-Cons
    {0x2e95,0x434b,3,22},  // USB: Scuf Envision
    {0x2e95,0x434d,3,22},  // USB: Scuf Envision
    {0x2e95,0x434e,3,22},  // USB: Scuf Envision
    {0x04d9,0x0f16,3,23},  // USB: Sony PlayStation Controller Adapter
    {0x0810,0x0001,3,23},  // USB: Twin PS2 Adapter
    {0x0810,0x0003,3,23},  // USB: USB Gamepad
    {0x2dc8,0x9000,3,24},  // USB: 8BitDo FC30 Pro
    {0x2dc8,0x9001,3,24},  // USB: 8BitDo NES30 Pro
    {0x06a3,0x040b,3,25},  // USB: Saitek P990 Dual Analog
    {0x187c,0x0600,3,25},  // USB: Alienware Dual Compatible Game PlayStation Controller
    {0x04b4,0x2412,5,26},  // BT : Flydigi APEX 4
    {0x4f43,0x4e41,5,26},  // BT : VX Gaming Command Series
    {0x045e,0x02e3,5,27},  // BT : Xbox One Elite
    {0x045e,0x0b05,5,27},  // BT : Microsoft Xbox One Elite 2
    {0x0925,0x0005,3,28},  // USB: Sony PS2 pad with SmartJoy Adapter
    {0x0925,0x8866,3,28},  // USB: MP8866 Super Dual Box
    {0x0955,0x7214,3,29},  // USB: NVIDIA Controller
    {0x0955,0x7214,5,29},  // BT : NVIDIA Controller
    {0x06a3,0xf620,3,30},  // USB: Saitek PS2700 Rumble
    {0x06a3,0xf623,3,30},  // USB: Saitek Cyborg PlayStation Controller
    {0x2dc8,0x5112,3,31},  // USB: 8BitDo Lite 2
    {0x1949,0x0419,3,32},  // USB: Amazon Luna Controller
    {0x1008,0x01e5,3,33},  // USB: Anbernic Handheld
    {0x3250,0x1002,3,34},  // USB: Atari VCS Modern Controller
    {0x3250,0x1002,5,35},  // BT : Atari VCS Modern Controller
    {0x06a3,0xf622,3,36},  // USB: Cyborg V3 Rumble
    {0x056e,0x2003,3,37},  // USB: Elecom U3613M
    {0x0b43,0x0003,3,38},  // USB: EMS Production PS2 Adapter
    {0x05ac,0x057a,3,39},  // USB: GameSir G5
    {0x0e8f,0x1006,3,40},  // USB: GreenAsia Electronics Controller
    {0x0e8f,0x0012,3,41},  // USB: GreenAsia Joystick
    {0x0f0d,0x006d,3,42},  // USB: Hori EDGE 301
    {0x05fd,0x262a,3,43},  // USB: InterAct HammerHead FX
    {0x0f30,0x010b,3,44},  // USB: Jess Tech GGE909 PC Recoil
    {0x07b5,0x004f,3,45},  // USB: Mega World Logic 3 Controller
    {0x057e,0x2009,3,46},  // USB: Nintendo Switch Pro Controller
    {0x057e,0x0330,5,47},  // BT : Nintendo Wii U Pro Controller
    {0x050d,0x0803,3,48},  // USB: Nostromo n45 Dual Analog
    {0x0955,0x7210,3,49},  // USB: NVIDIA Controller
    {0x124b,0x4d01,3,50},  // USB: NYKO Airflo EX
    {0x2836,0x0001,5,51},  // BT : OUYA Controller
    {0x0583,0x2050,3,52},  // USB: Padix Rockfire PlayStation Bridge
    {0x0e6f,0x02d7,3,53},  // USB: PDP Black Camo Wired Xbox Series Controller
    {0x0e6f,0x0185,3,54},  // USB: PDP Fightpad Pro Gamecube Controller
    {0x054c,0x05c4,3,55},  // USB: PS4 Controller
    {0x2c22,0x2012,3,56},  // USB: Qanba Drone 2 Arcade Joystick PS4
    {0x0e6f,0x1113,3,57},  // USB: Saffun Controller
    {0x06a3,0x040c,3,58},  // USB: Saitek P2900
    {0x06a3,0xf518,3,59},  // USB: Saitek P3200 Rumble
    {0x6666,0x0667,3,60},  // USB: Sony PlayStation Adapter
    {0x28de,0x1201,3,61},  // USB: Steam Controller
    {0x6666,0x8804,3,62},  // USB: Super Joy Box 5 Pro
    {0x22ba,0x0107,3,63},  // USB: Technology Innovation PS2 Adapter
    {0x044f,0xb303,3,64},  // USB: Thrustmaster Firestorm Dual Analog 2
    {0x044f,0xb300,3,65},  // USB: Thrustmaster Firestorm Dual Power
    {0x0079,0x0006,3,66},  // USB: USB gamepad
    {0x05ac,0x3232,5,67},  // BT : VR Box Controller
    {0x045e,0x0b0a,3,68},  // USB: Xbox One Controller
    {0x045e,0x02e0,5,69},  // BT : Xbox One Controller
    {0x2717,0x3144,5,70},  // BT : XiaoMi Controller
};

// For gamepads with no VID:PID, index by string name instead.
static struct gamepad_by_name {
    char name[17];       // Name string
    uint8_t  inx;        // Layout index
} gamepad_by_name[] = {
    {"Lic Pro Controll", 6},  // Lic Pro Controller
    {"Nintendo Wireles", 6},  // Nintendo Wireless Gamepad
    {"Wireless Gamepad", 6},  // Wireless Gamepad
};
#endif

#ifdef __ANDROID__
constexpr std::array gamepad_layout_list = {
//   A   B   X   Y   LB  RB  LS  RS  UP  DN  LE  RI  THUMBL  THUMBR  TRIGER  SEL START
    "b0  b1  b3  b4  b6  b7  b10 b11 b15 b16 b17 b18 a0  a1  a2  a3  a4  a5  b8  b12 ",  // 0
    "b1  b0  b4  b3  b6  b7  b10 b11 b15 b16 b17 b18 a0  a1  a2  a3  b2  b5  b8  b12 ",  // 1
    "b1  b0  b4  b3  b6  b7  b10 b11 b15 b16 b17 b18 a0  a1  a2  a3  a4  a5  b13 b12 ",  // 2
    "b0  b1  b3  b4  b6  b7  b10 b11 b15 b16 b17 b18 a0  a1  a2  a3  a5  a4  b13 b12 ",  // 3
    "b1  b0  b4  b3  b6  b7  b10 b11 b15 b16 b17 b18 a0  a1  a2  a3  a5  a4  b13 b12 ",  // 4
    "b0  b1  b3  b4  b6  b7  b10 b11 b15 b16 b17 b18 a0  a1  a3  a4  a2  a5  b8  b12 ",  // 5
    "b1  b0  b4  b3  b6  b7  b10 b11 b15 b16 b17 b18 a0  a1  a2  a3  b8  b9  b13 b12 ",  // 6
    "b0  b1  b3  b4  b6  b7  b10 b11 b15 b16 b17 b18 a0  a1  a2  a5  a3  a4  b13 b12 ",  // 7
    "b1  b2  b0  b3  b4  b5  b13 b12 b15 b16 b17 b18 a0  a1  a2  a3  b6  b7  b8  b9  ",  // 8
    "b0  b3  b1  b4  b6  b8  b10 b11 h   h   h   h   a0  a1  a2  a3  a4  a5  b13 b12 ",  // 9
    "b1  b0  b3  b2  b4  b5  b8  b9  b15 b16 b17 b18 a0  a1  a3  a4  a2  a5  b6  b7  ",  // 10
    "b1  b0  b3  b4  b6  b7  b10 b11 b15 b16 b17 b18 a0  a1  a2  a4  b2  b5  b8  b12 ",  // 11
    "b1  b0  b4  b3  b6  b7  b10 b11 b15 b16 b18 b17 a0  a1  a2  a3  a5  a4  b13 b12 ",  // 12
    "b0  b1  b2  b3  b4  b5  b13 b12 b15 b16 b17 b18 a0  a1  a2  a3  b6  b7  b8  b9  ",  // 13
    "b1  b2  b0  b3  b4  b5  b13 b12 b15 b16 b17 b18 a0  a1  a2  a5  a3  a4  b8  b9  ",  // 14
    "b0  b1  b3  b4  b6  b7  b10 b11 b15 b16 b17 b18 a0  a1  a2  a3  b2  b5  b8  b12 ",  // 15
    "b0  b1  b2  b3  b4  b5  b8  b9  b15 b16 b17 b18 a0  a1  a3  a4  a2  a5  b6  b7  ",  // 16
    "b0  b1  b3  b4  b6  b7  b10 b11 b15 b16 b17 b18 a0  a1  a2  a3  a5  a6  b13 b12 ",  // 17
    "b0  b1  b3  b4  b6  b7  b10 b11 b15 b16 b17 b18 a0  a1  a2  a5  a7  a6  b13 b12 ",  // 18
};

static struct gamepad_index {
    uint16_t VID;     // Vendor ID
    uint16_t PID;     // Product ID
    uint8_t  BUS;     // USB(3) or Bluetooth(5)
    uint8_t  inx;     // Layout index
} gamepad_index[] = {
    {0x045e,0x028e,5, 0},  // BT : Xbox 360 Controller
    {0x045e,0x02ea,5, 0},  // BT : Xbox One S Controller
    {0x045e,0x02fd,5, 0},  // BT : Xbox One Controller
    {0x045e,0x0b13,5, 0},  // BT : Xbox Series Controller
    {0x04b4,0x2411,5, 0},  // BT : Flydigi Vader 2
    {0x054c,0x0268,5, 0},  // BT : PS3 Controller
    {0x054c,0x05c4,5, 0},  // BT : PS4 Controller
    {0x054c,0x09cc,5, 0},  // BT : PS4 Controller
    {0x0955,0x7203,5, 0},  // BT : NVIDIA Controller
    {0x0955,0x7210,5, 0},  // BT : NVIDIA Controller
    {0x1532,0x0705,5, 0},  // BT : Razer Raiju Mobile
    {0x1532,0x0707,5, 0},  // BT : Razer Raiju Mobile
    {0x1532,0x0900,5, 0},  // BT : Razer Serval
    {0x27f8,0x0bbf,5, 0},  // BT : Razer Kishi
    {0x2002,0x9000,5, 2},  // BT : 8BitDo NES30 Pro
    {0x2820,0x0009,5, 2},  // BT : 8BitDo SNES30
    {0x2dc8,0x2865,5, 2},  // BT : 8BitDo N30 Pro 2
    {0x2dc8,0x6100,5, 2},  // BT : 8BitDo SF30 Pro
    {0x3820,0x0009,5, 2},  // BT : 8BitDo NES30 Pro
    {0x20bc,0x5500,5, 3},  // BT : GameSir G3w
    {0x2dc8,0x6001,5, 4},  // BT : 8BitDo SN30 Pro
    {0x2dc8,0x9015,5, 4},  // BT : 8BitDo N30 Pro 2
    {0x045e,0x0b00,5, 5},  // BT : Xbox One Elite 2 Controller
    {0x045e,0x0b12,5, 5},  // BT : Xbox Series Controller
    {0x2dc8,0x6000,5, 6},  // BT : 8BitDo SF30 Pro
    {0x2dc8,0x6002,5, 6},  // BT : 8BitDo SN30 Pro Plus
    {0x02d6,0x89e5,5, 7},  // BT : GPD XD Plus
    {0x0955,0x7214,5, 7},  // BT : NVIDIA Controller
    {0x057e,0x2009,5,13},  // BT : Nintendo Switch Pro Controller
    {0x054c,0x0ce6,5,14},  // BT : PS5 Controller
    {0x045e,0x02e0,5,16},  // BT : Xbox One Controller
    {0x045e,0x0b05,5,17},  // BT : Xbox One Elite 2 Controller
    {0x2717,0x3144,5,18},  // BT : XiaoMi Controller
};

// For gamepads with no VID:PID, index by string name instead.
static struct gamepad_by_name {
    char name[17];       // Name string
    uint8_t  inx;        // Layout index
} gamepad_by_name[] = {
    {"Lic Pro Controll", 13},  // Lic Pro Controller
    {"Nintendo Wireles", 13},  // Nintendo Wireless Gamepad
    {"Wireless Gamepad", 13},  // Wireless Gamepad
    {"Google LLC Stadi",  0},  // Google Stadia Controller
    {"Logitech Gamepad",  0},  // Logitech F710
    {"Luna Gamepad",      0},  // Luna Controller
    {"MOBAPAD Pro-HD",    0},  // Mobapad Chitu HD
    {"MOCUTE-053X-M51-",  0},  // Mocute 053X
    {"NVIDIA Corporati",  0},  // NVIDIA Controller
    {"Sony Computer En",  0},  // 8BitDo SN30 Pro Plus
    {"Sony Interactive",  0},  // Sony PlayStation Controller
    {"Sony PLAYSTATION",  0},  // PS3 Controller
    {"Wireless Control",  0},  // Sony PlayStation Controller
    {"Xbox 360 Wireles",  0},  // Xbox 360 Controller
    {"Xbox Wireless Co",  0},  // Xbox One Controller
    {"8BitDo 8BitDo Li",  1},  // 8BitDo Lite
    {"8BitDo 8BitDo Pr",  1},  // 8BitDo Pro 2
    {"8BitDo 8BitDo Re",  1},  // 8BitDo Receiver
    {"8BitDo Lite 2",     1},  // 8BitDo Lite 2
    {"8BitDo Lite SE",    1},  // 8BitDo Lite SE
    {"8BitDo Pro 2",      1},  // 8BitDo Pro 2
    {"8BitDo SN30 Pro+",  1},  // 8BitDo SN30 Pro Plus
    {"Generic X-Box pa",  3},  // Xbox Controller
    {"Microsoft X-Box ",  3},  // Xbox Controller
    {"X-Box Controller",  3},  // Retroid Pocket
    {"Retroid Pocket C",  4},  // Retroid Pocket
    {"Amazon Game Cont",  5},  // Luna Controller
    {"HORI CO.,LTD  PA",  8},  // Hori Gem Pad 3
    {"Performance Desi",  8},  // PDP PS3 Rock Candy Controller
    {"8BitDo NGC Modki",  9},  // 8BitDo GameCube
    {"8Bitdo SF30 Pro",  10},  // 8BitDo SF30 Pro
    {"Linux 4.19.172 w", 11},  // Anbernic Handheld
    {"Odin Controller",  12},  // AYN Odin
    {"TGZ Controller",   15},  // TGZ Controller
};
#endif

//------ Encode gamepad layout to binary at compile-time ------
constexpr std::array<uint8_t, 20> encodeLine(const char* line) {
    std::array<uint8_t, 20> data = {};
    for(auto& d:data) d=0xff;

    int i=0;
    char c = *line;
    while(*line && i<data.size()) {
        uint8_t flags = 0;
        while(c==' ') {          c=*++line;}  // white-space
        if(c=='-') {flags|=0x80; c=*++line;}  // flip axis
        if(c=='b') {flags&=0x1F; c=*++line;}  // is button
        if(c=='a') {flags|=0x20; c=*++line;}  // is axis
        if(c=='h') {flags|=0x40; c=*++line;}  // is HAT
        uint8_t num=0;
        while(c>='0' && c<='9') {
            num=num*10+(c-'0');
            c=*++line;
        }
        data[i++]=num|flags;
    }
    return data;
}

template<std::size_t N>
constexpr auto encodeLines(const std::array<const char*, N>& lines) {
    std::array<std::array<uint8_t, 20>, N> result = {};
    for (std::size_t i = 0; i < N; ++i) {
        result[i] = encodeLine(lines[i]);
    }
    return result;
}

constexpr auto gamepad_layouts = encodeLines(gamepad_layout_list);
//-------------------------------------------------------------

//-------------------Return gamepad layout---------------------
static const std::array<uint8_t, 20>* get_gamepad_layout(uint16_t VID, uint16_t PID, uint8_t BUS, const char* name) {
    const std::array<uint8_t, 20>* layout = nullptr;
    for (const auto& entry : gamepad_index)
        if (entry.VID == VID && entry.PID == PID && entry.BUS == BUS)
            return &gamepad_layouts[entry.inx];
    for (const auto& entry : gamepad_by_name)
        if (strstr(name, entry.name))
            return &gamepad_layouts[entry.inx];
    return nullptr;
}
//-------------------------------------------------------------

#endif

// -------------------------------
// window_win32.h
// -------------------------------

//#define VK_USE_PLATFORM_WIN32_KHR
//#define GWINDOW_IMPLEMENTATION

//==========================Win32===============================
#ifdef VK_USE_PLATFORM_WIN32_KHR

#ifndef WINDOW_WIN32
#define WINDOW_WIN32

//#define ENABLE_MULTITOUCH //1kb
//#define ENABLE_GAMEPAD    //2kb  (requires xinput)
//#define ENABLE_CLIPBOARD  //1kb
//#define ENABLE_SHOWIMAGE
//#define ENABLE_FULLSCREEN
#define ENABLE_CURSOR

#include <windowsx.h>  // Mouse
#include <assert.h>
#include <ShellScalingApi.h> // for GetDpiForWindow
#include <Xinput.h>          // for Gamepad

#define MIN(a,b) ((a<b)?(a):(b))
#define MAX(a,b) ((a>b)?(a):(b))

//for MINGW
#undef  _WIN32_WINNT
#define _WIN32_WINNT 0x0A00  // Windows 10
#undef  WINVER
#define WINVER 0x0A00

// Convert native Win32 keyboard scancode to cross-platform USB HID code.
const unsigned char WIN32_TO_HID[256] = {
      0,  0,  0,  0,  0,  0,  0,  0, 42, 43,  0,  0,  0, 40,  0,  0,    // 16
    225,224,226, 72, 57,  0,  0,  0,  0,  0,  0, 41,  0,  0,  0,  0,    // 32
     44, 75, 78, 77, 74, 80, 82, 79, 81,  0,  0,  0, 70, 73, 76,  0,    // 48
     39, 30, 31, 32, 33, 34, 35, 36, 37, 38,  0,  0,  0,  0,  0,  0,    // 64
      0,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13, 14, 15, 16, 17, 18,    // 80
     19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29,  0,  0,  0,  0,  0,    // 96
     98, 89, 90, 91, 92, 93, 94, 95, 96, 97, 85, 87,  0, 86, 99, 84,    //112
     58, 59, 60, 61, 62, 63, 64, 65, 66, 67, 68, 69,104,105,106,107,    //128
    108,109,110,111,112,113,114,115,  0,  0,  0,  0,  0,  0,  0,  0,    //144
     83, 71,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,    //160
    225,229,224,228,226,230,  0,  0,  0,  0,  0,  0,  0,127,128,129,    //176    L/R shift/ctrl/alt  mute/vol+/vol-
      0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 51, 46, 54, 45, 55, 56,    //192
     53,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,    //208
      0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 47, 49, 48, 52,  0,    //224
      0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,    //240
      0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0     //256
};

struct native_handle {
    HINSTANCE hInstance;
    HWND hWnd;
};

//=============================Win32============================
class Window_win32 : public WindowBase {
    HINSTANCE hInstance;
    HWND hWnd;
    RECT rect;            // Save window rect while in fullscreen mode
    DWORD style;          // Save window stype while in fullscreen mode
    CMTouch MTouch;       // Multi-Touch device
    HBITMAP DIB = 0;      // For showImage().  Holds image to display.
    HCURSOR cursors[12];  // For mouse cursors

    void Create(const char* title="Window", uint width=640, uint height=480);
    void DetectGamepads();
    void ReadGamepadEvents();
    void EnableDPIAware();
public:
    void setTitle(const char* title);
    void setPosition(uint x, uint y);
    void setSize(uint w, uint h);

public:
    Window_win32(){Create();};
    Window_win32(const char* title, uint width, uint height);
    virtual ~Window_win32();
    EventType getEvent(bool wait_for_event = false);
    native_handle* getNativeHandle() const {return (native_handle*)&hInstance;};
    float getDisplayScale();
#ifdef ENABLE_SHOWIMAGE
    void showImage(uint32_t* buf, uint32_t width, uint32_t height);
#endif
#ifdef ENABLE_CURSOR
    void setCursor(eCursor id);
#endif
#ifdef ENABLE_CLIPBOARD
    void setClipboardText(const char* text) override;
    const char* getClipboardText() override;
#endif
#ifdef ENABLE_FULLSCREEN
    void setFullscreen(bool enable);
#endif

};
//==============================================================
#endif

#ifdef GWINDOW_IMPLEMENTATION

//=====================Win32 IMPLEMENTATION=====================
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

Window_win32::Window_win32(const char* title, uint width, uint height) {
    Create(title, width, height);
}

void Window_win32::Create(const char* title, uint width, uint height) {
    shape.width  = width;
    shape.height = height;
    running      = true;
    //printf("Creating Win32 Window...\n");

    EnableDPIAware();
    hInstance = GetModuleHandle(NULL);

    // Initialize the window class structure:
    WNDCLASSEX win_class;
    win_class.cbSize        = sizeof(WNDCLASSEX);
    win_class.style         = CS_HREDRAW | CS_VREDRAW;
    win_class.lpfnWndProc   = WndProc;
    win_class.cbClsExtra    = 0;
    win_class.cbWndExtra    = 0;
    win_class.hInstance     = hInstance;
    win_class.hIcon         = LoadIcon(NULL, IDI_APPLICATION);
#ifdef ENABLE_CURSOR
    win_class.hCursor       = LoadCursor(NULL, IDC_ARROW);
#endif
    win_class.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
    win_class.lpszMenuName  = NULL;
    win_class.lpszClassName = title;
    win_class.hIconSm       = LoadIcon(NULL, IDI_WINLOGO);
    // Register window class:
    ATOM atom = RegisterClassEx(&win_class);
    assert(atom && "Failed to register the window class.");

    // Create window with the registered class:
    RECT wr = {0, 0, (LONG)width, (LONG)height};
    AdjustWindowRect(&wr, WS_OVERLAPPEDWINDOW, FALSE);
    hWnd = CreateWindowEx(0,
                          title,                                          // class name
                          title,                                          // app name
                          WS_VISIBLE | WS_SYSMENU | WS_OVERLAPPEDWINDOW,  // window style
                          100, 100,                                       // x/y coords
                          wr.right - wr.left,                             // width
                          wr.bottom - wr.top,                             // height
                          NULL,                                           // handle to parent
                          NULL,                                           // handle to menu
                          hInstance,                                    // hInstance
                          NULL);                                          // no extra parameters
    assert(hWnd && "Failed to create a window.");

#ifdef ENABLE_CURSOR
    cursors[0] = LoadCursor(NULL, IDC_ARROW);       // Arrow
    cursors[1] = LoadCursor(NULL, IDC_IBEAM);       // Caret (Text Input)
    cursors[2] = LoadCursor(NULL, IDC_SIZEALL);     // Resize All
    cursors[3] = LoadCursor(NULL, IDC_SIZENS);      // Resize NS
    cursors[4] = LoadCursor(NULL, IDC_SIZEWE);      // Resize EW
    cursors[5] = LoadCursor(NULL, IDC_SIZENESW);    // Resize NESW
    cursors[6] = LoadCursor(NULL, IDC_SIZENWSE);    // Resize NWSE
    cursors[7] = LoadCursor(NULL, IDC_HAND);        // Hand
    cursors[8] = LoadCursor(NULL, IDC_WAIT);        // Wait
    cursors[9] = LoadCursor(NULL, IDC_APPSTARTING); // Progress
    cursors[10]= LoadCursor(NULL, IDC_NO);          // Not Allowed
#endif

    eventFIFO.push(resizeEvent(width, height));
}

Window_win32::~Window_win32() { DestroyWindow(hWnd); }

void Window_win32::setTitle(const char* title) { SetWindowText(hWnd, title); }

void Window_win32::setPosition(uint x, uint y) {
    SetWindowPos(hWnd, NULL, x, y, 0, 0, SWP_NOZORDER | SWP_NOACTIVATE | SWP_NOSIZE);
    if (x != shape.x || y != shape.y) eventFIFO.push(moveEvent(x, y));  // Trigger window moved event
}

void Window_win32::setSize(uint w, uint h) {
    RECT wr = {0, 0, (LONG)w, (LONG)h};
    AdjustWindowRect(&wr, WS_OVERLAPPEDWINDOW, FALSE);  // Add border size to create desired client area size
    int total_width = wr.right - wr.left;
    int total_height = wr.bottom - wr.top;
    SetWindowPos(hWnd, NULL, 0, 0, total_width, total_height, SWP_NOZORDER | SWP_NOACTIVATE | SWP_NOMOVE);
    if ((w != shape.width) | (h != shape.height)) eventFIFO.push(resizeEvent(w, h));  // Trigger resize event
}

#define WM_RESHAPE (WM_USER + 0)
#define WM_ACTIVE  (WM_USER + 1)

EventType Window_win32::getEvent(bool wait_for_event) {
    // EventType event;
    if (!eventFIFO.isEmpty()) return eventFIFO.pop();

    if (running) {
        MSG msg = {};
        if(wait_for_event) running = (GetMessage(&msg, NULL, 16, 0) > 0);           // Blocking mode
        else if(!PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) return {EventType::NONE};  // Non-blocking mode

        TranslateMessage(&msg);
        int16_t x = GET_X_LPARAM(msg.lParam);
        int16_t y = GET_Y_LPARAM(msg.lParam);

        //--Convert Shift / Ctrl / Alt key messages to LeftShift / RightShift / LeftCtrl / RightCtrl / LeftAlt / RightAlt--
        if (msg.message == WM_KEYDOWN || msg.message == WM_KEYUP) {
            if (msg.wParam == VK_CONTROL) msg.wParam = (msg.lParam & (1 << 24)) ? VK_RCONTROL : VK_LCONTROL;
            if (msg.wParam == VK_SHIFT) {
                if (!!(::GetKeyState(VK_LSHIFT) & 128) != getKeyState(eKEY_LeftShift )) PostMessage(hWnd, msg.message, VK_LSHIFT, 0);
                if (!!(::GetKeyState(VK_RSHIFT) & 128) != getKeyState(eKEY_RightShift)) PostMessage(hWnd, msg.message, VK_RSHIFT, 0);
                return {EventType::NONE};
            }
        } else if (msg.message == WM_SYSKEYDOWN || msg.message == WM_SYSKEYUP) {
            if (msg.wParam == VK_MENU) msg.wParam = (msg.lParam & (1 << 24)) ? VK_RMENU : VK_LMENU;
        }
        //-----------------------------------------------------------------------------------------------------------------

        static char buf[4] = {};
        uint8_t bestBtn = getBtnState(1) ? 1 : getBtnState(2) ? 2 : getBtnState(3) ? 3 : 0;
        switch (msg.message) {
            //--Mouse events--
            case WM_MOUSEMOVE  : return mouseEvent(eMOVE, x, y, bestBtn);
            case WM_LBUTTONDOWN: return mouseEvent(eDOWN, x, y, 1);
            case WM_MBUTTONDOWN: return mouseEvent(eDOWN, x, y, 2);
            case WM_RBUTTONDOWN: return mouseEvent(eDOWN, x, y, 3);
            case WM_LBUTTONUP  : return mouseEvent(eUP  , x, y, 1);
            case WM_MBUTTONUP  : return mouseEvent(eUP  , x, y, 2);
            case WM_RBUTTONUP  : return mouseEvent(eUP  , x, y, 3);
            //--Mouse wheel events--
            case WM_MOUSEWHEEL: {
                uint8_t wheel = (GET_WHEEL_DELTA_WPARAM(msg.wParam) > 0) ? 4 : 5;
                POINT point = {x, y};
                ScreenToClient(msg.hwnd, &point);
                return {EventType::MOUSE, {eDOWN, (int16_t)point.x, (int16_t)point.y, wheel}};
            }
            //--Keyboard events--
            case WM_KEYDOWN   : return keyEvent(eDOWN, WIN32_TO_HID[msg.wParam]);
            case WM_KEYUP     : return keyEvent(eUP  , WIN32_TO_HID[msg.wParam]);
            case WM_SYSKEYDOWN: {MSG discard; GetMessage(&discard, NULL, 0, 0);     // Alt-key triggers a WM_MOUSEMOVE message... Discard it.
                                return keyEvent(eDOWN, WIN32_TO_HID[msg.wParam]); } // +alt key
            case WM_SYSKEYUP  : return keyEvent(eUP  , WIN32_TO_HID[msg.wParam]);   // +alt key

            //--Char event--
            case WM_CHAR: { strncpy_s(buf, (const char*)&msg.wParam, 4);  return textEvent(buf); }  // return UTF8 code of key pressed
            //--Window events--
            case WM_ACTIVE: { return focusEvent(msg.wParam != WA_INACTIVE); }

            case WM_RESHAPE: {
                if (!has_focus) {
                    PostMessage(hWnd, WM_RESHAPE, msg.wParam, msg.lParam);  // Repost this event to the queue
                    return focusEvent(true);                                // Activate window before reshape
                }

                RECT r;
                GetClientRect(hWnd, &r);
                uint16_t w = (uint16_t)(r.right - r.left);
                uint16_t h = (uint16_t)(r.bottom - r.top);
                if (w != shape.width || h != shape.height) return resizeEvent(w, h);  // window resized

                GetWindowRect(hWnd, &r);
                int16_t x = (int16_t)r.left;
                int16_t y = (int16_t)r.top;
                if (x != shape.x || y != shape.y) return moveEvent(x, y);  // window moved
                break;
            }
            case WM_CLOSE: {
                if(msg.hwnd == hWnd) {
                    //printf("WM_CLOSE\n");
                    if(DIB) {DeleteObject(DIB); DIB=0;}
                    return closeEvent();
                }
                break;
            }
            case WM_PAINT: {
                if(DIB) {
                    PAINTSTRUCT ps{};
                    HDC hDC = BeginPaint(hWnd, &ps);
                    HDC hMemDC = CreateCompatibleDC(hDC);
                    HBITMAP hOldBitmap = (HBITMAP)SelectObject(hMemDC, DIB);
                    int w = ps.rcPaint.right;
                    int h = ps.rcPaint.bottom;
                    //float s = getScale();
                    //StretchBlt(hDC, 0, 0, w*s, h*s, hMemDC, 0, 0, w, h, SRCCOPY);
                    BitBlt(hDC, 0, 0, w, h, hMemDC, 0, 0, SRCCOPY);
                    SelectObject(hMemDC, hOldBitmap);
                    DeleteDC(hMemDC);
                    EndPaint(hWnd, &ps);
                }
                break;
            }

#ifdef ENABLE_MULTITOUCH

#define WM_POINTERUPDATE 0x0245
#define WM_POINTERDOWN   0x0246
#define WM_POINTERUP     0x0247

            case WM_POINTERUPDATE:
            case WM_POINTERDOWN:
            case WM_POINTERUP: {
                POINTER_INFO pointerInfo;
                if (GetPointerInfo(GET_POINTERID_WPARAM(msg.wParam), &pointerInfo)) {
                    uint  id = pointerInfo.pointerId;
                    POINT pt = pointerInfo.ptPixelLocation;
                    ScreenToClient(hWnd, &pt);
                    switch (msg.message) {
                        case WM_POINTERDOWN  : return MTouch.Event_by_ID(eDOWN, x, y,  0, id);  // touch down event
                        case WM_POINTERUPDATE: return MTouch.Event_by_ID(eMOVE, x, y, id, id);  // touch move event
                        case WM_POINTERUP    : return MTouch.Event_by_ID(eUP  , x, y, id,  0);  // touch up event
                    }
                }
            }
#endif
        }
        DispatchMessage(&msg);
    }

    ReadGamepadEvents();
    return {EventType::NONE};
}

static bool inClientArea = false;
// MS-Windows event handling function:
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
        case WM_CLOSE:
            PostMessage(hWnd, WM_CLOSE, 0, 0);  // for onCloseEvent
            return 0;
        case WM_DESTROY:
            //LOGI("WM_DESTROY\n");
            PostQuitMessage(0);
            return 0;
        case WM_PAINT:
             //LOGI("WM_PAINT\n");
            return 0;
        case WM_GETMINMAXINFO: {  // set window's minimum size
            MINMAXINFO* pmmi = (MINMAXINFO*)lParam;
            pmmi->ptMinTrackSize.x = 8;
            pmmi->ptMinTrackSize.y = 8;
            return 0;
        }
        case WM_NCHITTEST: {
            POINT pt;
            pt.x = GET_X_LPARAM(lParam);
            pt.y = GET_Y_LPARAM(lParam);
            ScreenToClient(hWnd, &pt);

            RECT rect;
            GetClientRect(hWnd, &rect);

            // Flag if mouse is in client area (for mouse cursor)
            inClientArea = ((pt.x > rect.left)&&(pt.x < rect.right)
                          &&(pt.y > rect.top )&&(pt.y < rect.bottom));
            break;
        }

//        case WM_IME_CHAR: 
//            wprintf(L"WM_IME_CHAR : %c\n", (wchar_t)wParam);
//            return 0;

        case WM_SIZE         : { PostMessage(hWnd, WM_RESHAPE, 0, 0);          break; }
        case WM_EXITSIZEMOVE : { PostMessage(hWnd, WM_RESHAPE, 0, 0);          break; }
        case WM_ACTIVATE     : { PostMessage(hWnd, WM_ACTIVE, wParam, lParam); break; }
        default: break;
    }
    return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

float Window_win32::getDisplayScale() {
    int dpi = GetDpiForWindow(hWnd);
    //printf("dpi = %d\n", dpi);
    return dpi/96.f;
}
#ifdef ENABLE_SHOWIMAGE
void Window_win32::showImage(uint32_t* buf, uint32_t width, uint32_t height) {  // using GDI only
    if(DIB) {DeleteObject(DIB); DIB=0;}  // delete previous bitmap
    BITMAPINFO bmi{};
    bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    bmi.bmiHeader.biWidth = width;
    bmi.bmiHeader.biHeight = -height;  // Negative height for top-down DIB
    bmi.bmiHeader.biPlanes = 1;
    bmi.bmiHeader.biBitCount = 32;     // RGBA format
    bmi.bmiHeader.biCompression = BI_RGB;

    HDC hdc = GetDC(0);
    DIB = CreateDIBitmap(hdc, &bmi.bmiHeader, CBM_INIT, buf, &bmi, DIB_RGB_COLORS);
    ReleaseDC(NULL, hdc);
    InvalidateRect(hWnd, NULL, false);
}
#endif

#ifdef ENABLE_CURSOR
void Window_win32::setCursor(eCursor id) {      // Override mouse cursor,
    if(inClientArea) ::SetCursor(cursors[id]);  // but not on window edges
}
#endif

//---- Gamepads ----
#ifdef ENABLE_GAMEPAD
void Window_win32::DetectGamepads() {
    static DWORD last_time = 0;
    DWORD curr_time = GetTickCount();
    if(curr_time - last_time < 1000) return; // Only check once per second
    last_time = curr_time;

    for (DWORD i = 0; i < MAX_GAMEPADS; i++) {
        XINPUT_STATE state;
        Gamepad& pad = gamepad[i];
        bool active = (XInputGetState(i, &state) == ERROR_SUCCESS);
        if(active != pad.active) eventFIFO.push(gpadConnect(i, active));
    }
}

void Window_win32::ReadGamepadEvents() {
    DetectGamepads();
    for (DWORD i = 0; i < MAX_GAMEPADS; i++) {
        Gamepad& pad = gamepad[i];
        if(!pad.active) continue;
        XINPUT_STATE state;
        if (XInputGetState(i, &state) != ERROR_SUCCESS) continue;

        auto btnCheck = [&](eGamepadBtn btn, bool isDown) {
            if(pad.buttons[btn] == isDown) return;
            eventFIFO.push(gpadButton(i, btn, isDown));
        };

        WORD buttons = state.Gamepad.wButtons;
        btnCheck(eBTN_A,      buttons & XINPUT_GAMEPAD_A);
        btnCheck(eBTN_B,      buttons & XINPUT_GAMEPAD_B);
        btnCheck(eBTN_X,      buttons & XINPUT_GAMEPAD_X);
        btnCheck(eBTN_Y,      buttons & XINPUT_GAMEPAD_Y);
        btnCheck(eBTN_TL,     buttons & XINPUT_GAMEPAD_LEFT_SHOULDER);
        btnCheck(eBTN_TR,     buttons & XINPUT_GAMEPAD_RIGHT_SHOULDER);
        btnCheck(eBTN_THUMBL, buttons & XINPUT_GAMEPAD_LEFT_THUMB);
        btnCheck(eBTN_THUMBR, buttons & XINPUT_GAMEPAD_RIGHT_THUMB);
        btnCheck(eBTN_SELECT, buttons & XINPUT_GAMEPAD_BACK);
        btnCheck(eBTN_START,  buttons & XINPUT_GAMEPAD_START);
      //btnCheck(eBTN_MODE,   buttons & XINPUT_GAMEPAD_GUIDE);
        btnCheck(eDPAD_UP,    buttons & XINPUT_GAMEPAD_DPAD_UP);
        btnCheck(eDPAD_DOWN,  buttons & XINPUT_GAMEPAD_DPAD_DOWN);
        btnCheck(eDPAD_LEFT,  buttons & XINPUT_GAMEPAD_DPAD_LEFT);
        btnCheck(eDPAD_RIGHT, buttons & XINPUT_GAMEPAD_DPAD_RIGHT);

        auto axisCheck = [&](eGamepadAxis axis, float val) {
            if(pad.axes[axis] == val) return;
            eventFIFO.push(gpadAxis(i, axis, val));
        };

        axisCheck(eAXIS_LX, state.Gamepad.sThumbLX / 32767.f);
        axisCheck(eAXIS_LY, state.Gamepad.sThumbLY / 32767.f);
        axisCheck(eAXIS_RX, state.Gamepad.sThumbRX / 32767.f);
        axisCheck(eAXIS_RY, state.Gamepad.sThumbRY / 32767.f);
        axisCheck(eAXIS_TL, state.Gamepad.bLeftTrigger / 255.f);
        axisCheck(eAXIS_TR, state.Gamepad.bRightTrigger / 255.f);
    }
}
#else
    void Window_win32::DetectGamepads() {}
    void Window_win32::ReadGamepadEvents(){}
#endif
//------------------

//---- Clipboard ----
#ifdef ENABLE_CLIPBOARD
    void Window_win32::setClipboardText(const char* text) {
        if (!OpenClipboard(nullptr)) return;
        EmptyClipboard();

        size_t len = strlen(text) + 1;
        HGLOBAL hMem = GlobalAlloc(GMEM_MOVEABLE, len);
        if (!hMem) { CloseClipboard(); return; }

        memcpy(GlobalLock(hMem), text, len);
        GlobalUnlock(hMem);
        SetClipboardData(CF_TEXT, hMem);
        CloseClipboard();
    }

    const char* Window_win32::getClipboardText() {
        if (!OpenClipboard(nullptr)) return nullptr;

        HANDLE hData = GetClipboardData(CF_TEXT);
        if (!hData) { CloseClipboard(); return nullptr; }

        char* pszText = static_cast<char*>(GlobalLock(hData));
        if (pszText) {
            clipboard = pszText;
            GlobalUnlock(hData);
        }
        CloseClipboard();
        return clipboard.c_str();
    }

#endif //ENABLE_CLIPBOARD
//-------------------

#ifdef ENABLE_FULLSCREEN
    void Window_win32::setFullscreen(bool enable) {
        if(enable==fullscreen) return;
        fullscreen = enable;

        if(enable) {
            // Save windowed rect and style
            style = GetWindowLong(hWnd, GWL_STYLE);
            GetWindowRect(hWnd, &rect);

            // Remove borders and title bar
            SetWindowLong(hWnd, GWL_STYLE, style & ~(WS_OVERLAPPEDWINDOW));

            // Get monitor dimensions
            HMONITOR hMonitor = MonitorFromWindow(hWnd, MONITOR_DEFAULTTONEAREST);
            MONITORINFO mi = { sizeof(mi) };
            if (GetMonitorInfo(hMonitor, &mi)) {
                float scale = getDisplayScale();
                int x = int(mi.rcMonitor.left / scale);
                int y = int(mi.rcMonitor.top / scale);
                int w = int((mi.rcMonitor.right - mi.rcMonitor.left) / scale);
                int h = int((mi.rcMonitor.bottom - mi.rcMonitor.top) / scale);
                SetWindowPos(hWnd, HWND_TOP, x, y, w, h, SWP_NOOWNERZORDER | SWP_FRAMECHANGED);
            }
        }else{
            // Restore windowed style and size
            SetWindowLong(hWnd, GWL_STYLE, style);
            int x = rect.left;
            int y = rect.top;
            int w = rect.right - rect.left;
            int h = rect.bottom - rect.top;
            SetWindowPos(hWnd, HWND_NOTOPMOST, x, y, w, h, SWP_NOOWNERZORDER | SWP_FRAMECHANGED);
        }
    }
#endif  // ENABLE_FULLSCREEN

void Window_win32::EnableDPIAware() {
    auto user32 = LoadLibraryA("user32.dll");  // Windows 10+
    if (user32) {
        using SetDpiAwarenessContext_t = BOOL(WINAPI*)(DPI_AWARENESS_CONTEXT);
        auto setContext = reinterpret_cast<SetDpiAwarenessContext_t>(
            GetProcAddress(user32, "SetProcessDpiAwarenessContext"));
        if (setContext) {
            setContext(DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE_V2);
            return;
        }
    }
    SetProcessDPIAware();  // Fallback for Vista / Windows 7
}

#endif  // GWINDOW_IMPLEMENTATION

#endif  // VK_USE_PLATFORM_WIN32_KHR
//==============================================================

// -------------------------------
// window_xcb.h
// -------------------------------

//#define VK_USE_PLATFORM_XCB_KHR
//#define GWINDOW_IMPLEMENTATION

//============================XCB===============================
#ifdef VK_USE_PLATFORM_XCB_KHR

#ifndef WINDOW_XCB
#define WINDOW_XCB

//#define ENABLE_MULTITOUCH  // requires libxi-dev
//#define ENABLE_GAMEPAD     // requires libevdev-dev (8kb)
//#define ENABLE_CLIPBOARD   // requires libxcb-icccm4-dev + libxcb1-dev
//#define ENABLE_SHOWIMAGE   // requires libxcb-image0-dev + libxcb1-dev
//#define ENABLE_CURSOR      // requires libxcb-cursor-dev + libxcb1-dev + libxcb-cursor0
//#define ENABLE_FULLSCREEN  // requires libxcb1-dev

//-------------------------------------------------
//#include <xcb/xcb.h>            // XCB only
//#include <X11/Xlib.h>           // XLib only
#include <X11/Xlib-xcb.h>         // Xlib + XCB
#include <xkbcommon/xkbcommon.h>  // Keyboard   libxkbcommon-dev
#include <X11/Xresource.h>        // DPI scale
#include <stdlib.h>               // atof
#include <assert.h>
#ifdef ENABLE_SHOWIMAGE
#include <xcb/xcb_image.h>        // showImage  libxcb-image0-dev
#endif
#ifdef ENABLE_CURSOR
#include <xcb/xcb_cursor.h>       // mouse cursor icons
#endif
#ifdef ENABLE_GAMEPAD
#include <libevdev/libevdev.h>    // libevdev-dev
#include <fcntl.h>                // gamepad open
#include <unistd.h>               // gamepad read
#include <sys/inotify.h>          // gamepad inotify
#include <dirent.h>               // For scanning the /dev/input/ directory
#include <sys/ioctl.h>
#include <linux/input.h>
#endif
#ifdef ENABLE_FULLSCREEN
#include <xcb/xcb.h>
#endif
#ifdef ENABLE_CLIPBOARD
#include <xcb/xcb_icccm.h>
#endif
//-------------------------------------------------

#ifdef ENABLE_MULTITOUCH
#include <X11/extensions/XInput2.h>  // MultiTouch
typedef uint16_t xcb_input_device_id_t;
typedef uint32_t xcb_input_fp1616_t;
// clang-format off
typedef struct xcb_input_touch_begin_event_t {  // from xinput.h in XCB 1.12 (current version is 1.11)
    uint8_t                   response_type;
    uint8_t                   extension;
    uint16_t                  sequence;
    uint32_t                  length;
    uint16_t                  event_type;
    xcb_input_device_id_t     deviceid;
    xcb_timestamp_t           time;
    uint32_t                  detail;
    xcb_window_t              root;
    xcb_window_t              event;
    xcb_window_t              child;
    uint32_t                  full_sequence;
    xcb_input_fp1616_t        root_x;
    xcb_input_fp1616_t        root_y;
    xcb_input_fp1616_t        event_x;
    xcb_input_fp1616_t        event_y;
    uint16_t                  buttons_len;
    uint16_t                  valuators_len;
    xcb_input_device_id_t     sourceid;
    // uint8_t                   pad0[2];
    // uint32_t                  flags;
    // xcb_input_modifier_info_t mods;
    // xcb_input_group_info_t    group;
} xcb_input_touch_begin_event_t;
#endif

// clang-format off
// Convert native EVDEV key-code to cross-platform USB HID code.
const unsigned char EVDEV_TO_HID[256] = {
  0,  0,  0,  0,  0,  0,  0,  0,  0, 41, 30, 31, 32, 33, 34, 35,
 36, 37, 38, 39, 45, 46, 42, 43, 20, 26,  8, 21, 23, 28, 24, 12,
 18, 19, 47, 48, 40,224,  4, 22,  7,  9, 10, 11, 13, 14, 15, 51,
 52, 53,225, 49, 29, 27,  6, 25,  5, 17, 16, 54, 55, 56,229, 85,
226, 44, 57, 58, 59, 60, 61, 62, 63, 64, 65, 66, 67, 83, 71, 95,
 96, 97, 86, 92, 93, 94, 87, 89, 90, 91, 98, 99,  0,  0,100, 68,
 69,  0,  0,  0,  0,  0,  0,  0, 88,228, 84, 70,230,  0, 74, 82,
 75, 80, 79, 77, 81, 78, 73, 76,  0,127,128,129,  0,103,  0, 72,
  0,  0,  0,  0,  0,227,231,118,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,104,
105,106,107,108,109,110,111,112,113,114,115,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0
};

struct native_handle {
    Display* display;
    xcb_connection_t* xcb_connection;
    xcb_screen_t* xcb_screen;
    xcb_window_t xcb_window;
};

//=============================XCB==============================
class Window_xcb : public WindowBase {
    Display* display;                  // for XLib
    xcb_connection_t* xcb_connection;  // for XCB
    xcb_screen_t* xcb_screen;
    xcb_window_t xcb_window;
    xcb_atom_t atom_wm_delete_window = XCB_NONE;
    //----xcb_image ----
    xcb_gcontext_t gc=0;
    xcb_pixmap_t   pixmap=0;
    //------------------
    //---xkb Keyboard---
    xkb_context* k_ctx;  // context for xkbcommon keyboard input
    xkb_keymap* k_keymap;
    xkb_state* k_state;
    //------------------
    //---Touch Device---
    CMTouch MTouch;
    int xi_opcode;  // 131
    int xi_devid;   // 2
    //------------------
    //----- Cursor -----
#ifdef ENABLE_CURSOR
    xcb_cursor_context_t *cursor_ctx;
    xcb_cursor_t cursors[12];
#endif
    //------------------
    //---- Gamepad ----
#ifdef ENABLE_GAMEPAD
#define MAX_BTNS 16
#define MAX_AXIS 16

    int inotify_fd = -1;          // gamepad inotify descriptor
    int watch_fd   = -1;          // gamepad watch descriptor
    struct Evdev {                // gamepad handle and axis ranges
        int fd = -1;
        libevdev* dev = nullptr;
        char name[256] = {};      // gamepad model name
        char path[256] = {};      // eg. /dev/input/event240

        struct Btns{
            uint16_t BTN;         // BTN event code
            int8_t  eBTN;         // eGamepadBtn
        }b[MAX_BTNS]={};          // buttons

        struct Axes{
            uint16_t AXIS;        // ABS event code
            int8_t  eAXIS;        // eGamepadAxis
            int  min =0;          // Axis range min value
            int  max =0;          // Axis range max value
            int  fuzz=0;          // Noise level
            int  flat=0;          // Dead zone
            bool flip=false;      // Flip this axis
            int  prev=0;          // previous value
        }a[MAX_AXIS]={};          // axes

    } evdev[MAX_GAMEPADS];

    void DetectGamepads();                                   // Detect connected gamepads
    bool ConnectGamepad(const char* path);                   // eg. /dev/input/event240
    void DisconnectGamepad(uint8_t id);                      // Disconnect gamepad by id (0-3)
    void MapGamepad(uint8_t id);                             // Map gamepad btn/axis layout
    void SetGamepadLEDs(uint8_t id, uint8_t state);          // pad-id(0-3), led-bitmask(0-15)
    void ReadGamepadEvents();                                // Process all gamepad events
    void GamepadBtnEvent(uint8_t id, input_event event);     // Button events
    void GamepadAxisEvent(uint8_t id, input_event event);    // Axis events
#endif
    //------------------

    bool InitTouch();                                        // Returns false if no touch-device was found.
    EventType TranslateEvent(xcb_generic_event_t* x_event);  // Convert x_event to Window event
    void Create(const char* title="Window", uint width=640, uint height=480);
    xcb_atom_t GetAtom(const char* name, bool only_if_exists = false);

  public:
    void setTitle(const char* title);
    void setPosition(uint x, uint y);
    void setSize(uint w, uint h);
    //void CreateSurface(VkInstance instance);

    Window_xcb() {Create();}
    Window_xcb(const char* title, uint width, uint height);
    virtual ~Window_xcb();
    EventType getEvent(bool wait_for_event = false);
    //bool CanPresent(VkPhysicalDevice phy, uint32_t queue_family);  // check if this window can present this queue type
    native_handle* getNativeHandle() const {return (native_handle*)&display;}
    float getDisplayScale();
#ifdef ENABLE_SHOWIMAGE
    void showImage(uint32_t* buf, uint32_t width, uint32_t height);
#endif
    void setCursor(eCursor id);
#ifdef ENABLE_FULLSCREEN
    void setFullscreen(bool enable);
    bool isFullscreen();
#endif

#ifdef ENABLE_CLIPBOARD
private:
    xcb_atom_t atom_CLIPBOARD;   // "CLIPBOARD"
    xcb_atom_t atom_UTF8_STRING; // "UTF8_STRING"
    xcb_atom_t atom_TARGETS;     // "TARGETS"
    xcb_atom_t atom_PROPERTY;    // any name, often "XSEL_DATA"
    xcb_atom_t atom_STRING;      // fallback

    void InitClipboard();
    bool RequestClipboard();
public:
    virtual const char* getClipboardText();
    virtual void setClipboardText(const char* str);
#else
    void InitClipboard(){};
#endif



};
//==============================================================
#endif

#ifdef GWINDOW_IMPLEMENTATION

//=======================XCB IMPLEMENTATION=====================

Window_xcb::Window_xcb(const char* title, uint width, uint height) {
    Create(title, width, height);
}

xcb_atom_t Window_xcb::GetAtom(const char* name, bool only_if_exists) {
    xcb_intern_atom_cookie_t cookie = xcb_intern_atom(xcb_connection, only_if_exists, strlen(name), name);
    xcb_intern_atom_reply_t* reply = xcb_intern_atom_reply(xcb_connection, cookie, nullptr);
    if (!reply) return XCB_NONE;
    xcb_atom_t atom = reply->atom;
    free(reply);
    return atom;
}

void Window_xcb::Create(const char* title, uint width, uint height) {
    shape.width  = width;
    shape.height = height;
    running      = true;

    //printf("Creating XCB-Window...\n");

    // --Init Connection-- XCB only
    // int scr;
    // xcb_connection = xcb_connect(NULL, &scr);
    // assert(xcb_connection && "XCB failed to connect to the X server.");
    // const xcb_setup_t*   setup = xcb_get_setup(xcb_connection);
    // xcb_screen_iterator_t iter = xcb_setup_roots_iterator(setup);
    // while(scr-- > 0) xcb_screen_next(&iter);
    // xcb_screen = iter.data;
    //-------------------

    //----XLib + XCB----
    XInitThreads(); // Required by Vulkan, when using XLib. (Vulkan spec section: 30.2.6 Xlib Platform)
    display = XOpenDisplay(NULL);                 assert(display && "Failed to open Display");        // for XLIB functions
    xcb_connection = XGetXCBConnection(display);  assert(display && "Failed to open XCB connection"); // for XCB functions
    const xcb_setup_t* setup = xcb_get_setup(xcb_connection);
    setup = xcb_get_setup(xcb_connection);
    xcb_screen = (xcb_setup_roots_iterator(setup)).data;
    XSetEventQueueOwner(display, XCBOwnsEventQueue);
    //------------------

    uint32_t value_mask = XCB_CW_BACK_PIXEL | XCB_CW_EVENT_MASK;
    uint32_t value_list[2];
    value_list[0] = xcb_screen->black_pixel;
    value_list[1] = XCB_EVENT_MASK_KEY_PRESS |          // 1
                    XCB_EVENT_MASK_KEY_RELEASE |        // 2
                    XCB_EVENT_MASK_BUTTON_PRESS |       // 4
                    XCB_EVENT_MASK_BUTTON_RELEASE |     // 8
                    XCB_EVENT_MASK_POINTER_MOTION |     // 64       motion with no mouse button held
                    XCB_EVENT_MASK_BUTTON_MOTION  |     // 8192     motion with one or more mouse buttons held
                  //XCB_EVENT_MASK_KEYMAP_STATE |       // 16384
                  //XCB_EVENT_MASK_EXPOSURE |           // 32768    Make showImage persistant
                  //XCB_EVENT_MASK_VISIBILITY_CHANGE,   // 65536,
                    XCB_EVENT_MASK_STRUCTURE_NOTIFY |   // 131072   Window move/resize events
                  //XCB_EVENT_MASK_RESIZE_REDIRECT |    // 262144
                    XCB_EVENT_MASK_FOCUS_CHANGE;        // 2097152  Window focus

#ifdef ENABLE_SHOWIMAGE
    gc     = xcb_generate_id(xcb_connection);
    pixmap = xcb_generate_id(xcb_connection);
#endif

    xcb_window = xcb_generate_id(xcb_connection);
    xcb_create_window(xcb_connection, XCB_COPY_FROM_PARENT, xcb_window, xcb_screen->root, 0, 0, width, height, 0,
                      XCB_WINDOW_CLASS_INPUT_OUTPUT, xcb_screen->root_visual, value_mask, value_list);

    xcb_atom_t wm_protocols     = GetAtom("WM_PROTOCOLS", true);
    xcb_atom_t wm_delete_window = GetAtom("WM_DELETE_WINDOW");
    xcb_change_property(xcb_connection, XCB_PROP_MODE_REPLACE, xcb_window, wm_protocols, 4, 32, 1, &wm_delete_window);
    atom_wm_delete_window = wm_delete_window;

    //---Keyboard input---
    k_ctx = xkb_context_new(XKB_CONTEXT_NO_FLAGS);
    // xkb_rule_names names = {NULL,"pc105","is","dvorak","terminate:ctrl_alt_bksp"};
    // keymap = xkb_keymap_new_from_names(k_ctx, &names,XKB_KEYMAP_COMPILE_NO_FLAGS);
    k_keymap = xkb_keymap_new_from_names(k_ctx, NULL, XKB_KEYMAP_COMPILE_NO_FLAGS);  // use current keyboard settings
    k_state  = xkb_state_new(k_keymap);
    //--------------------
    InitTouch();  
    InitClipboard();
    //--------------------

    setTitle(title);
    eventFIFO.push(resizeEvent(width, height));  // resizeEvent BEFORE focus, for consistency with win32 and android

    //---- Mouse Cursor ----
#ifdef ENABLE_CURSOR
    xcb_cursor_context_new(xcb_connection, xcb_setup_roots_iterator(setup).data, &cursor_ctx);
    cursors[eCursor::eArrow]      = xcb_cursor_load_cursor(cursor_ctx, "left_ptr");
    cursors[eCursor::eCaret]      = xcb_cursor_load_cursor(cursor_ctx, "xterm");
    cursors[eCursor::eResizeAll]  = xcb_cursor_load_cursor(cursor_ctx, "fleur");
    cursors[eCursor::eResizeNS]   = xcb_cursor_load_cursor(cursor_ctx, "sb_v_double_arrow");
    cursors[eCursor::eResizeEW]   = xcb_cursor_load_cursor(cursor_ctx, "sb_h_double_arrow");
    cursors[eCursor::eResizeNESW] = xcb_cursor_load_cursor(cursor_ctx, "top_right_corner");
    cursors[eCursor::eResizeNWSE] = xcb_cursor_load_cursor(cursor_ctx, "top_left_corner");
    cursors[eCursor::eHand]       = xcb_cursor_load_cursor(cursor_ctx, "hand2");
    cursors[eCursor::eWait]       = xcb_cursor_load_cursor(cursor_ctx, "wait");
    cursors[eCursor::eProgress]   = xcb_cursor_load_cursor(cursor_ctx, "progress");
    cursors[eCursor::eNotAllowed] = xcb_cursor_load_cursor(cursor_ctx, "circle");
#endif
    //----------------------

    //----Map the window----
    xcb_map_window(xcb_connection, xcb_window);
    xcb_flush(xcb_connection);

    // Wait for the window to be mapped (so resize works correctly)
    xcb_generic_event_t *event;
    while ((event = xcb_wait_for_event(xcb_connection))) {
        bool mapped = ((event->response_type & ~0x80) == XCB_MAP_NOTIFY);
        free(event);
        if(mapped) break;
    }
    //---------------------- 
}

Window_xcb::~Window_xcb() {

#ifdef ENABLE_SHOWIMAGE
    if(gc)     xcb_free_gc    (xcb_connection, gc);
    if(pixmap) xcb_free_pixmap(xcb_connection, pixmap);
#endif
#ifdef ENABLE_CURSOR
    int cnt = sizeof(cursors) / sizeof(cursors[0]);
    for(int i=0; i<cnt; ++i) xcb_free_cursor(xcb_connection, cursors[i]);
    xcb_cursor_context_free(cursor_ctx);
#endif
#ifdef ENABLE_GAMEPAD
    for (int i = 0; i < MAX_GAMEPADS; ++i) { DisconnectGamepad(i); }
    if (watch_fd   != -1) { inotify_rm_watch(inotify_fd, watch_fd); watch_fd=-1;}
    if (inotify_fd != -1) { ::close(inotify_fd); inotify_fd=-1; }
#endif
    xcb_disconnect(xcb_connection);
    XFree(k_ctx);  // xkb keyboard
}

void Window_xcb::setTitle(const char* title) {
    xcb_change_property(xcb_connection, XCB_PROP_MODE_REPLACE, xcb_window, XCB_ATOM_WM_NAME,  // set window title
                        XCB_ATOM_STRING, 8, strlen(title), title);
    xcb_change_property(xcb_connection, XCB_PROP_MODE_REPLACE, xcb_window, XCB_ATOM_WM_ICON_NAME,  // set icon title
                        XCB_ATOM_STRING, 8, strlen(title), title);
    xcb_map_window(xcb_connection, xcb_window);
    xcb_flush(xcb_connection);
}

void Window_xcb::setPosition(uint x, uint y) {
    uint values[] = {x, y};
    xcb_configure_window(xcb_connection, xcb_window, XCB_CONFIG_WINDOW_X | XCB_CONFIG_WINDOW_Y, values);
    xcb_flush(xcb_connection);
}

void Window_xcb::setSize(uint w, uint h) {
    uint values[] = {w, h};
    xcb_configure_window(xcb_connection, xcb_window, XCB_CONFIG_WINDOW_WIDTH | XCB_CONFIG_WINDOW_HEIGHT, values);
    xcb_flush(xcb_connection);
}
/*
void Window_xcb::CreateSurface(VkInstance instance) {
    if (surface) return;
    this->instance = instance;
    VkXcbSurfaceCreateInfoKHR xcb_createInfo;
    xcb_createInfo.sType      = VK_STRUCTURE_TYPE_XCB_SURFACE_CREATE_INFO_KHR;
    xcb_createInfo.pNext      = NULL;
    xcb_createInfo.flags      = 0;
    xcb_createInfo.connection = xcb_connection;
    xcb_createInfo.window     = xcb_window;
    VKERRCHECK(vkCreateXcbSurfaceKHR(instance, &xcb_createInfo, NULL, &surface));
    LOGI("Vulkan Surface created\n");
}
*/
//---------------------------------------------------------------------------
bool Window_xcb::InitTouch() {
#ifdef ENABLE_MULTITOUCH
    int ev, err;
    if (!XQueryExtension(display, "XInputExtension", &xi_opcode, &ev, &err)) {
        printf("WARNING: XInputExtension not available.\n");
        return false;
    }

    // check the version of XInput
    int major = 2;
    int minor = 3;
    if (XIQueryVersion(display, &major, &minor) != Success) {
        printf("WARNING: No XI2 support. (%d.%d only)\n", major, minor);
        return false;
    }

    {  // select device
        int cnt;
        XIDeviceInfo* di = XIQueryDevice(display, XIAllDevices, &cnt);
        for (int i = 0; i < cnt; ++i) {
            XIDeviceInfo* dev = &di[i];
            for (int j = 0; j < dev->num_classes; ++j) {
                XITouchClassInfo* tcinfo = (XITouchClassInfo*)(dev->classes[j]);
                if (tcinfo->type != XITouchClass) {
                    xi_devid = dev->deviceid;
                    goto endloop;
                }
            }
        }
    endloop:
        XIFreeDeviceInfo(di);
    }

    {  // select which events to listen to
        unsigned char buf[3] = {};
        XIEventMask mask     = {};
        mask.deviceid        = xi_devid;
        mask.mask_len        = XIMaskLen(XI_TouchEnd);
        mask.mask            = buf;
        XISetMask(mask.mask, XI_TouchBegin);
        XISetMask(mask.mask, XI_TouchUpdate);
        XISetMask(mask.mask, XI_TouchEnd);
        XISelectEvents(display, xcb_window, &mask, 1);
    }
    return true;
#else
    return false;
#endif
}
//---------------------------------------------------------------------------

EventType Window_xcb::TranslateEvent(xcb_generic_event_t* x_event) {
    static char buf[4] = {};                                            // store char for text event
    xcb_button_press_event_t& e = *(xcb_button_press_event_t*)x_event;  // xcb_motion_notify_event_t
    int16_t mx = e.event_x;
    int16_t my = e.event_y;
    uint8_t btn= e.detail;
    uint8_t bestBtn = getBtnState(1) ? 1 : getBtnState(2) ? 2 : getBtnState(3) ? 3 : 0;  // If multiple buttons pressed, pick left one.
    switch(x_event->response_type & ~0x80) {
        case XCB_MOTION_NOTIFY : return mouseEvent(eMOVE, mx, my, bestBtn); // mouse move
        case XCB_BUTTON_PRESS  : return mouseEvent(eDOWN, mx, my, btn);     // mouse btn press
        case XCB_BUTTON_RELEASE: return mouseEvent(eUP  , mx, my, btn);     // mouse btn release
        case XCB_KEY_PRESS:{
            //printf("btn %d", btn);
            uint8_t keycode = EVDEV_TO_HID[btn];                    // On Stratus XL gamepad, 2 buttons trigger keyboard events
            if(!keycode) {                                          // remap key to gamepad btn
                if(btn==166) return gpadButton(0, eBTN_SELECT, 1);  // Steelseries Stratus XL: select button (XF86Back)
                if(btn==180) return gpadButton(0, eBTN_MODE, 1);    // Steelseries Stratus XL: mode button   (XF86HomePage)
            }
            xkb_state_key_get_utf8(k_state,btn,buf,sizeof(buf));
            xkb_state_update_key(k_state,btn,XKB_KEY_DOWN);
            if(buf[0]) eventFIFO.push(textEvent(buf));              // text typed event (store in FIFO for next run)
            return keyEvent(eDOWN, keycode);                        // key pressed event
        }
        case XCB_KEY_RELEASE: {
            xkb_state_update_key(k_state, btn, XKB_KEY_UP);
            uint8_t keycode = EVDEV_TO_HID[btn];
            if(!keycode) {                                          // remap key to gamepad btn
                if(btn==166) return gpadButton(0, eBTN_SELECT, 0);  // Steelseries Stratus XL
                if(btn==180) return gpadButton(0, eBTN_MODE, 0);    // Steelseries Stratus XL
            }
            return keyEvent(eUP, keycode);                          // key released event
        }
        case XCB_CLIENT_MESSAGE: {                                  // window close event
            if ((*(xcb_client_message_event_t*)x_event).data.data32[0] == atom_wm_delete_window) {
                //printf("Closing Window\n");
                return closeEvent();
            }
            break;
        }
        case XCB_CONFIGURE_NOTIFY: {                                // Window Reshape (move or resize)
            auto& e = *(xcb_configure_notify_event_t*)x_event;
            //bool se = (e.response_type & 128);                    // True if message was sent with "SendEvent"
            if (e.width != shape.width || e.height != shape.height) return resizeEvent(e.width, e.height); // window resized
            else if (e.x != shape.x || e.y != shape.y)              return moveEvent(e.x, e.y);            // window moved
            break;
        }
        case XCB_FOCUS_IN  : if (!has_focus) return focusEvent(true);   // window gained focus
        case XCB_FOCUS_OUT : if ( has_focus) return focusEvent(false);  // window lost focus

        case XCB_GE_GENERIC: {                                            // Multi touch screen events
#ifdef ENABLE_MULTITOUCH
            xcb_input_touch_begin_event_t& te = *(xcb_input_touch_begin_event_t*)x_event;
            if(te.extension == xi_opcode) {  // check if this event is from the touch device
                float x = te.event_x / 65536.f;
                float y = te.event_y / 65536.f;
                uint id = te.detail;

                switch(te.event_type){
                    case XI_TouchBegin : return MTouch.Event_by_ID(eDOWN, x, y,  0, id); // touch down event
                    case XI_TouchUpdate: return MTouch.Event_by_ID(eMOVE, x, y, id, id); // touch move event
                    case XI_TouchEnd   : return MTouch.Event_by_ID(eUP  , x, y, id,  0); // touch up event
                    default : break;
                }
            }
#endif
            return {EventType::UNKNOWN};
        }  // XCB_GE_GENERIC

#ifdef ENABLE_SHOWIMAGE
        case XCB_EXPOSE: {  // for showImage
             xcb_expose_event_t& e = *(xcb_expose_event_t*)x_event;
             xcb_copy_area(xcb_connection,pixmap,xcb_window,gc,e.x,e.y,e.x,e.y,e.width,e.height);
             xcb_flush(xcb_connection);
        }break;
#endif

#ifdef ENABLE_CLIPBOARD
        case XCB_SELECTION_NOTIFY: {  // get clipboard text
            xcb_selection_notify_event_t* sel = (xcb_selection_notify_event_t*)x_event;
            if (sel->property == XCB_NONE) { clipboard = ""; break; }

            xcb_get_property_cookie_t prop_cookie =
                xcb_get_property(xcb_connection, 0, xcb_window, atom_PROPERTY,
                                          XCB_GET_PROPERTY_TYPE_ANY, 0, 1024);
            xcb_get_property_reply_t* prop_reply =
                xcb_get_property_reply(xcb_connection, prop_cookie, nullptr);
            if (prop_reply) {
                int len = xcb_get_property_value_length(prop_reply);
                const char* val = (const char*)xcb_get_property_value(prop_reply);
                clipboard.assign(val, len);
                free(prop_reply);
            }
        }break;

        case XCB_SELECTION_REQUEST: {
            xcb_selection_request_event_t* req = (xcb_selection_request_event_t*)x_event;

            xcb_selection_notify_event_t notify = {};
            notify.response_type = XCB_SELECTION_NOTIFY;
            notify.sequence  = 0;
            notify.time      = req->time;
            notify.requestor = req->requestor;
            notify.selection = req->selection;
            notify.target    = req->target;
            notify.property  = req->property;

            if (req->target == atom_TARGETS) {
                xcb_atom_t targets[] = { atom_UTF8_STRING, atom_STRING, atom_TARGETS };
                xcb_change_property(
                    xcb_connection,
                    XCB_PROP_MODE_REPLACE,
                    req->requestor,
                    req->property,
                    XCB_ATOM_ATOM,
                    32,
                    sizeof(targets) / sizeof(targets[0]),
                    targets
                );
            } else
            if (req->target == atom_UTF8_STRING || req->target == atom_STRING) {
                xcb_atom_t type = (req->target == atom_UTF8_STRING) ? atom_UTF8_STRING : atom_STRING;
                xcb_change_property(
                    xcb_connection,
                    XCB_PROP_MODE_REPLACE,
                    req->requestor,
                    req->property,
                    type,
                    8,  // 8 bits per char
                    clipboard.size(),
                    clipboard.c_str()
                );
            } else {
                notify.property = XCB_NONE;  // Unsupported target
            }

            xcb_send_event(xcb_connection, false, req->requestor, XCB_EVENT_MASK_NO_EVENT, (const char*)&notify);
            xcb_flush(xcb_connection);
        }break;
#endif
        default:
            // printf("EVENT: %d\n",(x_event->response_type & ~0x80));  //get event numerical value
            break;
    }  // switch
    return {EventType::NONE};
}

EventType Window_xcb::getEvent(bool wait_for_event) {
#ifdef ENABLE_GAMEPAD
    ReadGamepadEvents();
#endif
    if (!eventFIFO.isEmpty()) return eventFIFO.pop();  // pop message from message queue buffer
    xcb_generic_event_t* x_event;
    if (wait_for_event) x_event = xcb_wait_for_event(xcb_connection);  // Blocking mode
    else                x_event = xcb_poll_for_event(xcb_connection);  // Non-blocking mode
    while(x_event) {
        EventType event = TranslateEvent(x_event);
        XFree(x_event);
        if (event.tag == EventType::UNKNOWN) {
            x_event = xcb_poll_for_event(xcb_connection);  // Discard unknown events (Intel Mesa drivers spams event 35)
        } else return event;
    }
    return {EventType::NONE};
}

float Window_xcb::getDisplayScale() {
    float dpi = 0.f;
    XrmValue value;
    char *type = NULL;
    char *resourceString = XResourceManagerString(display);
    XrmInitialize();
    XrmDatabase db = XrmGetStringDatabase(resourceString);
    if (resourceString) {
        //printf("Entire DB:\n%s\n", resourceString);
        if (XrmGetResource(db, "Xft.dpi", "String", &type, &value) == True) {
            if (value.addr) {
                dpi = atof(value.addr);
            }
        }
    }
    //printf("Monitor DPI: %f\n", dpi);
    //display_scale = dpi / 96.f;
    return dpi / 96.f;
}

#ifdef ENABLE_SHOWIMAGE
void Window_xcb::showImage(uint32_t* buf, uint32_t width, uint32_t height) {  // Shows image for 1 frame.
    xcb_connection_t* c = xcb_connection;
    xcb_image_format_t format = XCB_IMAGE_FORMAT_Z_PIXMAP;
    int depth  = xcb_screen->root_depth;

    xcb_create_pixmap(c,depth,pixmap,xcb_window,width,height);
    xcb_create_gc(c,gc,pixmap,0,NULL);

    xcb_image_t* image = xcb_image_create_native(c,width,height,format,depth,0,0,(uint8_t*)buf);
    xcb_image_put(c, pixmap, gc, image, 0, 0, 0);
    xcb_image_destroy(image);
    xcb_copy_area(c,pixmap,xcb_window,gc,0,0,0,0,width,height);
    xcb_flush(xcb_connection);

    xcb_free_gc(c, gc);
    xcb_free_pixmap(c, pixmap);
}
#endif


void Window_xcb::setCursor(eCursor id) {
#ifdef ENABLE_CURSOR
    xcb_change_window_attributes(xcb_connection, xcb_window, XCB_CW_CURSOR, &cursors[id]);
#endif
}

#ifdef ENABLE_FULLSCREEN
void Window_xcb::setFullscreen(bool enable) {
    xcb_atom_t a_wm_state = GetAtom("_NET_WM_STATE");
    xcb_atom_t a_fullscreen = GetAtom("_NET_WM_STATE_FULLSCREEN");

    if (a_wm_state == XCB_NONE || a_fullscreen == XCB_NONE) return;

    xcb_client_message_event_t ev = {};
    ev.response_type = XCB_CLIENT_MESSAGE;
    ev.window = xcb_window;
    ev.type = a_wm_state;
    ev.format = 32;
    ev.data.data32[0] = enable ? 1 : 0;  // _NET_WM_STATE_ADD : _NET_WM_STATE_REMOVE
    ev.data.data32[1] = a_fullscreen;
    ev.data.data32[2] = 0;
    ev.data.data32[3] = 1;  // Normal source indication (application)
    ev.data.data32[4] = 0;

    xcb_send_event(xcb_connection, 0,
                   xcb_setup_roots_iterator(xcb_get_setup(xcb_connection)).data->root,
                   XCB_EVENT_MASK_SUBSTRUCTURE_REDIRECT | XCB_EVENT_MASK_SUBSTRUCTURE_NOTIFY,
                   reinterpret_cast<const char*>(&ev));
    xcb_flush(xcb_connection);
}

bool Window_xcb::isFullscreen() {
    xcb_atom_t net_wm_state = GetAtom("_NET_WM_STATE");
    xcb_atom_t fs_atom = GetAtom("_NET_WM_STATE_FULLSCREEN");

    xcb_get_property_cookie_t prop_cookie = xcb_get_property(
        xcb_connection,
        0,
        xcb_window,
        net_wm_state,
        XCB_ATOM_ATOM,
        0,  // offset
        1024  // length
    );

    xcb_get_property_reply_t* prop_reply = xcb_get_property_reply(xcb_connection, prop_cookie, nullptr);
    if (!prop_reply) return false;

    xcb_atom_t* atoms = (xcb_atom_t*)xcb_get_property_value(prop_reply);
    int len = xcb_get_property_value_length(prop_reply) / sizeof(xcb_atom_t);

    bool is_fs = false;
    for (int i = 0; i < len; ++i) {
        if (atoms[i] == fs_atom) {
            is_fs = true;
            break;
        }
    }

    free(prop_reply);
    return is_fs;
}
#endif

//---Gamepad---
#ifdef ENABLE_GAMEPAD
void Window_xcb::DetectGamepads() {
    if (inotify_fd == -1) {  // inotify not started yet
        // On first run, scan for already connected gamepads
        DIR* dir = opendir("/dev/input/");
        if(dir) {
            dirent* entry;
            while ((entry = readdir(dir))) {
                if (strncmp(entry->d_name, "event", 5) == 0) { // Look for "event#"
                    char path[512]{};
                    snprintf(path, sizeof(path), "/dev/input/%s", entry->d_name);
                    ConnectGamepad(path);
                }
            }
        }
        // Watch for new device connections using inotify
        inotify_fd = inotify_init1(IN_NONBLOCK);
        if (inotify_fd < 0) { perror("inotify_init1"); return; }
        watch_fd = inotify_add_watch(inotify_fd, "/dev/input/", IN_CREATE);
    }
    // Process inotify events
    char buffer[1024];
    int len = read(inotify_fd, buffer, sizeof(buffer));
    if (len > 0) {
        for (char* ptr = buffer; ptr < buffer + len;) {
            struct inotify_event* event = (struct inotify_event*)ptr;
            ptr += sizeof(struct inotify_event) + event->len;
            if (event->mask & (IN_CREATE)) {
                if (strncmp(event->name, "event", 5) == 0) {  // Look for "eventX"
                    char path[256]{};
                    snprintf(path, sizeof(path), "/dev/input/%s", event->name);
                    usleep(60000);         // Allow time for the device to appear
                    ConnectGamepad(path);  // Try to connect as Gamepad
                }
            }
        }
    }
}

void setGamepadLED(const char* devicePath, int LED_id, bool state) {
    int fd = open(devicePath, O_WRONLY);
    if(fd<0) return;
    struct input_event event;
    event.type = EV_LED;
    event.code = LED_id;
    event.value = state ? 1 : 0;
    ssize_t s=write(fd, &event, sizeof(event));
    close(fd);
}

void Window_xcb::SetGamepadLEDs(uint8_t id, uint8_t state) {
    setGamepadLED(evdev[id].path, 0, !!(state&1));
    setGamepadLED(evdev[id].path, 1, !!(state&2));
    setGamepadLED(evdev[id].path, 2, !!(state&4));
    setGamepadLED(evdev[id].path, 3, !!(state&8));
}

bool Window_xcb::ConnectGamepad(const char* path) {
    int fd = open(path, O_RDONLY | O_NONBLOCK);
    if(fd<0) return false;
    libevdev* dev = nullptr;
    if (libevdev_new_from_fd(fd, &dev) >= 0) {
        if(libevdev_has_event_type(dev, EV_ABS)
        && libevdev_has_event_type(dev, EV_KEY)
        && libevdev_has_event_code(dev, EV_KEY, BTN_SOUTH)
        && libevdev_has_event_code(dev, EV_KEY, BTN_NORTH)
        && libevdev_has_event_code(dev, EV_KEY, BTN_EAST )
        && libevdev_has_event_code(dev, EV_KEY, BTN_WEST )
        && libevdev_has_event_code(dev, EV_ABS, ABS_X)
        && libevdev_has_event_code(dev, EV_ABS, ABS_Y)) {  // make sure this is a gamepad
            for (int i = 0; i < MAX_GAMEPADS; ++i) {       // find a free slot
                Evdev& ev = evdev[i];
                if(ev.fd < 0) {
                    ev.fd = fd;
                    ev.dev= dev;
                    strncpy(ev.name, libevdev_get_name(dev), sizeof(ev.name)-1);  // query gamepad name
                    strncpy(ev.path, path, sizeof(ev.path)-1);                    // get gamepad event file path
                    //printf("Gamepad %d found: %s at %s\n", i, ev.name, path);
                    MapGamepad(i);           // Detect gamepad button layout
                    SetGamepadLEDs(i,1<<i);  // Set Gamepad LEDs to indicate which slot its in.
                    eventFIFO.push(gpadConnect(i, true));
                    return true;
                }
            }
        } else {libevdev_free(dev); ::close(fd);}
    } else ::close(fd);
    return false;
}

void Window_xcb::DisconnectGamepad(uint8_t id) {
    Evdev& ev = evdev[id];
    if(ev.fd==-1) return;
    eventFIFO.push(gpadConnect(id, false));
    //SetGamepadLEDs(id, 0);  // Does not restore blinking :(
    libevdev_free(ev.dev);
    ::close(ev.fd);
    memset(&ev, 0, sizeof(ev));
    ev.fd = -1;
    ev.dev = 0;
    ev.name[0] = '\0';
    ev.path[0] = '\0';
    //printf("Gamepad %d disconnected.\n", id);
}

void Window_xcb::MapGamepad(uint8_t id) {
    Evdev& pad = evdev[id];
    auto dev = pad.dev;
    const char* name = libevdev_get_name(dev);
    uint BUS=libevdev_get_id_bustype(dev);
    uint VID=libevdev_get_id_vendor(dev);
    uint PID=libevdev_get_id_product(dev);
    printf("Gamepad %d: \"%s\"\n",id , name);
    //printf("bus:%#x vendor:%#x product:%#x\n",bus, VID, PID);
    //-----------------------------------------------------------------
    int hatx_inx=0;  // HAT0X axis-index
    int haty_inx=0;  // HAT0Y axis-index
    int axis_cnt=0;  // not used

    // get the button event code list
    for (int code=0,b=0; code<KEY_MAX && b<MAX_BTNS; code++)
        if(libevdev_has_event_code(dev, EV_KEY, code)) pad.b[b++].BTN=code;

    // get axis event code and limits
    for (int code=0,a=0; code<KEY_MAX && a<MAX_AXIS; code++) {
        if(libevdev_has_event_code(dev, EV_ABS, code)) {
            if(code==ABS_HAT0X) hatx_inx=a;  // save the hatx index for later
            if(code==ABS_HAT0Y) haty_inx=a;  // save the haty index for later
            auto& axis = pad.a[a++];
            axis.AXIS = code;
            axis.min  = libevdev_get_abs_minimum(dev, code);  // axis min value
            axis.max  = libevdev_get_abs_maximum(dev, code);  // axis max value
            axis.fuzz = libevdev_get_abs_fuzz   (dev, code);  // noise level
            axis.flat = libevdev_get_abs_flat   (dev, code);  // dead zone
        }
        axis_cnt=a;
    }
    //for(auto& line : gamepad_layouts){for(uint8_t val : line) printf("%02X ", val); printf("\n");}

    auto* p_layout = get_gamepad_layout(VID, PID, BUS, name);  // Find layout by gamepad ID

    if(p_layout) {  // Gamepad found. Decode layout tokens.
        int8_t eBTN [] = {1,2,3,4,5,6,7,8,9,10,11,12,-1,-2,-3,-4,-5,-6,13,14};  // pos-to-eBTN
        int8_t eAXIS[] = {0,0,0,0,0,0,0,0,0, 0, 0, 0, 1, 2, 3, 4, 5, 6, 0, 0};  // pos-to-eAxis
        auto layout = *p_layout;
        for(int i=0; i<layout.size(); ++i) {
            uint8_t code = layout[i];
            //printf("%02x ",code);
            if(code==0xff) continue;                         // not mapped
            uint8_t num = code &0x1F;                        // extract number
            bool flip   = code &0x80;                        // extract flip flag
            bool isBtn  =!(code&0x60);                       // No flag for button
            bool isHat  = code &0x40;                        // extract hat flag
            bool isAxis = code &0x20;                        // extract axis flag
            if(isBtn)  pad.b[num].eBTN  = eBTN[i];           // button event code to eGamepadBtn map
            if(isAxis) pad.a[num].eAXIS = eAXIS[i];          // axis event code to eGamepadAxis map
            if(isAxis) pad.a[num].flip  = flip;              // flip axis
            if(isHat && i==8)  pad.a[hatx_inx].flip = flip;  // flip HAT0X
            if(isHat && i==10) pad.a[haty_inx].flip = flip;  // flip HAT0Y
            //printf("i=%d num=%d isBtn=%d isHat=%d isAxis=%d filp=%d\n",i ,num, isBtn, isHat, isAxis, flip);
        }
    } else {  // Gamepad not listed.  Use heuristics to guess layout.
        bool hasHAT=!!(hatx_inx & haty_inx);
        bool hasTrigger=false;

        auto& a = pad.a;
        a[0].eAXIS = eAXIS_LX;                                                                                 // left stick X (always first listed axis)
        a[1].eAXIS = eAXIS_LY;                                                                                 // left stick Y (always second listed axis)
        for(auto& ax : a) if(ax.AXIS==ABS_HAT0X) {ax.eAXIS=7; hasHAT=true; break;}                             // Hat X (always ABS_HAT0X, or a button)
        for(auto& ax : a) if(ax.AXIS==ABS_HAT0Y) {ax.eAXIS=8; hasHAT=true; break;}                             // Hat Y (always ABS_HAT0Y, or a button)
        for(auto& ax : a) if(!ax.eAXIS && ax.min==a[0].min && ax.max==a[0].max) {ax.eAXIS=eAXIS_RX; break;}    // right stick X (min/max should match left stick)
        for(auto& ax : a) if(!ax.eAXIS && ax.min==a[0].min && ax.max==a[0].max) {ax.eAXIS=eAXIS_RY; break;}    // right stick Y
        for(auto& ax : a) if(!ax.eAXIS && ax.min==0 && ax.max>1) {ax.eAXIS=eAXIS_TL; hasTrigger=true; break;}  // left trigger (Usually has min=0)
        for(auto& ax : a) if(!ax.eAXIS && ax.min==0 && ax.max>1) {ax.eAXIS=eAXIS_TR; hasTrigger=true; break;}  // right trigger
        //for(int i=0; i<axis_cnt; ++i) { auto& ax = a[i]; printf("i:%d  AXIS=%2d eAXIS=%2d  min=%5d  max=%5d  fuzz=%5d  flat=%5d  flip=%d\n", i, ax.AXIS, ax.eAXIS, ax.min, ax.max, ax.fuzz, ax.flat, ax.flip);}

        bool HID_style      = ( hasHAT &&  hasTrigger);  // HID and XInput compliant (XBox)
        bool Nintendo_style = ( hasHAT && !hasTrigger);  // Nintendo uses button triggers
        bool Sony_style     = (!hasHAT &&  hasTrigger);  // Sony uses DPad instead of HAT

        auto& btns = pad.b;
        if(HID_style) {
            for(auto& b : btns) {
                if(b.BTN==BTN_A)       b.eBTN=eBTN_A;
                if(b.BTN==BTN_B)       b.eBTN=eBTN_B;
                if(b.BTN==BTN_X)       b.eBTN=eBTN_X;
                if(b.BTN==BTN_Y)       b.eBTN=eBTN_Y;
                if(b.BTN==BTN_TL)      b.eBTN=eBTN_TL;
                if(b.BTN==BTN_TR)      b.eBTN=eBTN_TR;
                if(b.BTN==BTN_THUMBL)  b.eBTN=eBTN_THUMBL;
                if(b.BTN==BTN_THUMBR)  b.eBTN=eBTN_THUMBR;
                if(b.BTN==BTN_SELECT)  b.eBTN=eBTN_SELECT;
                if(b.BTN==BTN_START)   b.eBTN=eBTN_START;
            }
        }

        if(Nintendo_style) {
            for(auto& b : btns) {
                if(b.BTN==0x130) b.eBTN=eBTN_A;
                if(b.BTN==0x131) b.eBTN=eBTN_B;
                if(b.BTN==0x132) b.eBTN=eBTN_X;
                if(b.BTN==0x133) b.eBTN=eBTN_Y;
                if(b.BTN==0x134) b.eBTN=eBTN_TL;
                if(b.BTN==0x135) b.eBTN=eBTN_TR;
                if(b.BTN==0x136) b.eBTN=-eAXIS_TL;  // button trigger
                if(b.BTN==0x137) b.eBTN=-eAXIS_TR;  // button trigger
                if(b.BTN==0x13a) b.eBTN=eBTN_THUMBL;
                if(b.BTN==0x13b) b.eBTN=eBTN_THUMBR;
                if(b.BTN==0x138) b.eBTN=eBTN_SELECT;
                if(b.BTN==0x139) b.eBTN=eBTN_START;
            }
        }

        if(Sony_style) {  // untested
            for(auto& b : btns) {
                if(b.BTN==0x130) b.eBTN=eBTN_A;
                if(b.BTN==0x131) b.eBTN=eBTN_B;
                if(b.BTN==0x132) b.eBTN=eBTN_X;
                if(b.BTN==0x133) b.eBTN=eBTN_Y;
                if(b.BTN==0x134) b.eBTN=eBTN_TL;
                if(b.BTN==0x135) b.eBTN=eBTN_TR;
                if(b.BTN==0x13d) b.eBTN=eBTN_THUMBL;
                if(b.BTN==0x13e) b.eBTN=eBTN_THUMBR;
                if(b.BTN==0x138) b.eBTN=eBTN_SELECT;
                if(b.BTN==0x139) b.eBTN=eBTN_START;
            }
        }

        for(auto& b : btns) {  // DPad buttons (Sony?)
            if(b.BTN==BTN_DPAD_UP)    b.eBTN=eDPAD_UP;
            if(b.BTN==BTN_DPAD_DOWN)  b.eBTN=eDPAD_DOWN;
            if(b.BTN==BTN_DPAD_LEFT)  b.eBTN=eDPAD_LEFT;
            if(b.BTN==BTN_DPAD_RIGHT) b.eBTN=eDPAD_RIGHT;
        }

        //for(int i=0; i<MAX_BTNS; ++i) {auto& b = pad.map.b[i]; printf("BTN=%d eBTN=%d\n", b.BTN, b.eBTN);}
    }
}

void Window_xcb::ReadGamepadEvents() {
    DetectGamepads();
    for (int i=0; i<MAX_GAMEPADS; ++i) {
        Evdev&   ev  = evdev[i];
        Gamepad& pad = gamepad[i];
        if (!pad.active) continue;

       int rc=0;
        struct input_event event;
        while ((rc=libevdev_next_event(ev.dev, LIBEVDEV_READ_FLAG_NORMAL, &event)) == 0) {
            if (event.type == EV_KEY) { GamepadBtnEvent (i, event); } else // Button press/release
            if (event.type == EV_ABS) { GamepadAxisEvent(i, event); }      // Analog axes and hat buttons
        }
        if(rc==-ENODEV) DisconnectGamepad(i);
    }
}

void Window_xcb::GamepadBtnEvent(uint8_t id, input_event event) {
    auto& ev = evdev[id];
    uint keycode = event.code;
    //printf("keycode=%d (0x%3x) %d\n", keycode, keycode, event.value);
    if(event.value>1) return;  // ignore repeats (0=up 1=down 2=repeat)
    for(auto& b : ev.b) if(keycode==b.BTN) {
        if(b.eBTN>0) eventFIFO.push(gpadButton(id, b.eBTN, event.value));
        if(b.eBTN<0) eventFIFO.push(gpadAxis  (id,-b.eBTN, event.value));
    }
}

void Window_xcb::GamepadAxisEvent(uint8_t id, input_event event) {
    Gamepad& pad = gamepad[id];
    Evdev&   ev  = evdev[id];

    //------------------------------------------------------------------------------
    auto find_axis = [&](uint axiscode) -> Evdev::Axes& {
        for(auto& a : ev.a) if(axiscode==a.AXIS) return a;
        return ev.a[0];
    };

    auto Hat = [&](int val, int btnNeg, int btnPos) { // convert hat axis values to button events
        if((val!=-1) && ( pad.buttons[btnNeg])) eventFIFO.push(gpadButton(id, btnNeg, 0));
        if((val!= 1) && ( pad.buttons[btnPos])) eventFIFO.push(gpadButton(id, btnPos, 0));
        if((val==-1) && (!pad.buttons[btnNeg])) eventFIFO.push(gpadButton(id, btnNeg, 1));
        if((val== 1) && (!pad.buttons[btnPos])) eventFIFO.push(gpadButton(id, btnPos, 1));
    };

    auto isFuzz = [](int value, auto& a) -> bool { // detect fuzz events
        int delta = abs(a.prev - value);
        if(delta<a.fuzz) return true;
        a.prev = value;
        return false;
    };

    auto Trigger = [](int value, auto& a) -> float { // Apply dead-zone, normalize
        int val   = std::max(value-a.min-a.flat,0);
        int range = std::max(a.max-a.min-a.flat,1);
        return val / (float)range;
    };

    auto Thumb = [](int value, auto& a) -> float { // Apply dead-zone, normalize
        int center = (a.min + a.max) / 2;
        int centered = value - center;
        int sign  = centered<0 ? -1:1;
        int val   = std::max(std::abs(centered) - a.flat, 0);
        int range = a.max - center - a.flat;
        return (val / (float)range) * sign;
    };
    //------------------------------------------------------------------------------

    auto& a = find_axis(event.code);
    int val = a.flip ? -event.value : event.value;
    if(event.code == ABS_HAT0X) {Hat(val, eDPAD_LEFT, eDPAD_RIGHT); return;}
    if(event.code == ABS_HAT0Y) {Hat(val, eDPAD_UP,   eDPAD_DOWN);  return;}
    if(event.code > 10) return;         // Ignore HAT1+

    if(isFuzz(event.value, a)) return;  // defuzz
    bool isTrigger = (a.eAXIS==eAXIS_TL || a.eAXIS==eAXIS_TR);
    float fval = isTrigger? Trigger(event.value, a)
                          : Thumb  (event.value, a);

    if(pad.axes[a.eAXIS] == fval) return;  // deadzone
    if(a.eAXIS==eAXIS_LY || a.eAXIS==eAXIS_RY) fval=-fval; // flip y axis
    if(a.flip) fval=-fval;
    eventFIFO.push(gpadAxis(id, a.eAXIS, fval));
}

/*
// eg. SetGamepadRumble(0, 20000, 0);
void Window_xcb::SetGamepadRumble(int index, uint16_t weak, uint16_t strong) {  // TODO
    if (index < 0 || index >= MAX_GAMEPADS || gamepads[index].fd < 0) return;

    struct ff_effect effect = {};
    effect.type = FF_RUMBLE;
    effect.id = -1;
    effect.u.rumble.strong_magnitude = strong;
    effect.u.rumble.weak_magnitude = weak;
    if (ioctl(gamepads[index].fd, EVIOCSFF, &effect) < 0) return;

    struct input_event play = {};
    play.type = EV_FF;
    play.code = effect.id;
    play.value = 1;

    write(gamepads[index].fd, &play, sizeof(play));  // Start rumble
    usleep(500000);                                  // Let it run for 500ms
    ioctl(gamepads[index].fd, EVIOCRMFF, effect.id); // Remove the effect
}
*/

#endif

#ifdef ENABLE_CLIPBOARD
    void Window_xcb::InitClipboard() {
        atom_CLIPBOARD   = GetAtom("CLIPBOARD");
        atom_UTF8_STRING = GetAtom("UTF8_STRING");
        atom_STRING      = GetAtom("STRING");
        atom_PROPERTY    = GetAtom("XSEL_DATA");
    }

    bool Window_xcb::RequestClipboard() {
        xcb_convert_selection(xcb_connection, xcb_window,
            atom_CLIPBOARD,    // selection
            atom_UTF8_STRING,  // target
            atom_PROPERTY,     // property to receive data
            XCB_CURRENT_TIME);
        xcb_flush(xcb_connection);
        return true;
    }


    const char* Window_xcb::getClipboardText() {
        RequestClipboard();  // triggers XCB_SELECTION_NOTIFY event
        xcb_generic_event_t* event = xcb_wait_for_event(xcb_connection);
        TranslateEvent(event);
        return clipboard.c_str();
    }

    void Window_xcb::setClipboardText(const char* str) {
        clipboard = str;
        xcb_set_selection_owner(xcb_connection, xcb_window, atom_CLIPBOARD, XCB_CURRENT_TIME);
        xcb_flush(xcb_connection);
    }
#endif

//-------------

#endif  // GWINDOW_IMPLEMENTATION

#endif  // VK_USE_PLATFORM_XCB_KHR
//==============================================================

// -------------------------------
// android/android_fopen.h
// -------------------------------

#ifdef VK_USE_PLATFORM_ANDROID_KHR
// From http://www.50ply.com/blog/2013/01/19/loading-compressed-android-assets-with-file-pointer/
//
// This class hijacks android fopen, to read from the APK's assets folder.
// This avoids having to use AAssetManager explicitly, so libs like stb_image.h,
// that uses fopen, will work as is, without modification.
// To prevent breaking normal android fopen behavior, the following precautions were taken:
// If fopen is used with an absolute path (starting with '/') normal fopen is used. (for obb)
// If fopen is used to write, of if the file exists in Internal storage, normal fopen is used.
// Else, if reading from a relative path, fopen is redirected to read from the APK's assets folder.
// Optionally, you can start the path with 'assets/', to better match the desktop build's path.

#ifndef ANDROID_FOPEN_H
#define ANDROID_FOPEN_H

#define _DEFAULT_SOURCE 1  // Enables funopen()

#include <cstdio>
#include <cstring>
#include <cerrno>
#include <android/asset_manager.h>

class Android_fopen {
    static int read(void* cookie, char* buf, int size) {
        return AAsset_read((AAsset*)cookie, buf, size);
    }

    static int write(void* cookie, const char* buf, int size) {
        return EACCES; // can't provide write access to the apk
    }

    static fpos_t seek(void* cookie, fpos_t offset, int whence) {
        return AAsset_seek((AAsset*)cookie, offset, whence);
    }

    static int close(void* cookie) {
        AAsset_close((AAsset*)cookie);
        return 0;
    }

public:
    // must be established by someone else...
    static inline AAssetManager* asset_manager = nullptr;

    static FILE* fopen(const char* fname, const char* mode) {
        // Use regular fopen for absolute paths
        if (fname[0] == '/') return std::fopen(fname, mode);

        // Use regular fopen if file exists OR writing is attempted
        FILE* file = std::fopen(fname, mode);
        if (file || mode[0] != 'r') return file;

        // Finally, read from the APK instead, via AAssetManager
        if (!strncmp(fname, "./",     2)) fname+=2;  // skip the ./ prefix, if present
        if (!strncmp(fname,"assets/", 7)) fname+=7;  // skip the assets/ prefix, if present
        AAsset* asset = AAssetManager_open(asset_manager, fname, 0);
        if(!asset) return nullptr;
        return funopen(asset, read, write, seek, close);
    }
};

static void android_fopen_set_asset_manager(AAssetManager* manager) {
    Android_fopen::asset_manager = manager;
}

/* Hijack fopen and route it through the android asset system,
 * so that we can read from the APK's assets folder */
#define fopen(name, mode) Android_fopen::fopen(name, mode)

#endif

#endif // VK_USE_PLATFORM_ANDROID_KHR

// -------------------------------
// android/JClass.h
// -------------------------------

#ifdef VK_USE_PLATFORM_ANDROID_KHR
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
#endif // VK_USE_PLATFORM_ANDROID_KHR

// -------------------------------
// window_android.h
// -------------------------------

//==========================ANDROID=============================
#ifdef VK_USE_PLATFORM_ANDROID_KHR

#ifndef WINDOW_ANDROID
#define WINDOW_ANDROID

//#define ENABLE_MULTITOUCH
//#define ENABLE_GAMEPAD
//#define ENABLE_CLIPBOARD

#include <cmath>

#include <iostream>
#include <string>
#include <cstring>

#ifdef ENABLE_GAMEPAD
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

struct native_handle {
    ANativeWindow* window;
};

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

    native_handle* getNativeHandle() const {return (native_handle*)&(app->window);}

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

// -------------------------------
// Window.h
// -------------------------------

/*
*--------------------------------------------------------------------------
*
*  The CWindow class creates a window and main event processing loop.
*  It provides functions for querying the current state of the window, keyboard,
*  and mouse. Also, events may be processed via either polling or callbacks.
*
*  For polling, use the "getEvent" function to return one event at a time,
*  and process, using a case statement.  For an example, see the "processEvents" implementation.
*
*  For callbacks, use the "processEvents" function to dispatch all queued events to their
*  appropriate event handlers.  To create event handlers, derive your class from Window,
*  and override the virtual event handler functions. (See WindowBase.h)
*
*--------------------------------------------------------------------------
*/

// NOTE: GWindow.h MUST be #included BEFORE stdio.h, for printf to work correctly on Android.
// TODO: Add Wayland support


#ifndef CWINDOW_H
#define CWINDOW_H


#if defined(VK_USE_PLATFORM_XCB_KHR)
    typedef Window_xcb GWindow;
#elif defined(VK_USE_PLATFORM_WIN32_KHR)
    typedef Window_win32 GWindow;
#elif defined(VK_USE_PLATFORM_ANDROID_KHR)
    typedef Window_android GWindow;
#endif

#endif

#endif // GWINDOW_H

#ifdef GWINDOW_IMPLEMENTATION

// -------------------------------
// WindowBase.cpp
// -------------------------------

/*
*--------------------------------------------------------------------------
* Platform-specific event handlers call these functions to store input-device state,
* and package the event parameters into a platform-independent "EventType" struct.
*--------------------------------------------------------------------------
*/


//--Events--
EventType WindowBase::mouseEvent(eAction action, int16_t x, int16_t y, uint8_t btn) {
    mouse.pos = {x, y};
    if (action != eMOVE) mouse.btn[btn] = (action == eDOWN);  // Keep track of button state
    EventType e = {EventType::MOUSE, {action, x, y, btn}};
    return e;
}

EventType WindowBase::keyEvent(eAction action, uint8_t key) {
    keystate[key] = (action == eDOWN);
    EventType e   = {EventType::KEY};
    e.key         = {action, (eKeycode)key};
    return e;
}

EventType WindowBase::textEvent(const char* str) {
    EventType e = {EventType::TEXT};
    e.text.str  = str;
    return e;
}

EventType WindowBase::moveEvent(int16_t x, int16_t y) {
    shape.x     = x;
    shape.y     = y;
    EventType e = {EventType::MOVE};
    e.move      = {x, y};
    return e;
}

EventType WindowBase::resizeEvent(uint16_t width, uint16_t height) {
    this->is_resized = true;
    shape.width  = width;
    shape.height = height;
    //float scale = getScale();
    EventType e  = {EventType::RESIZE};
    e.resize     = {width, height};
    //e.resize.width = width / scale;
    //e.resize.height= height/ scale;
    return e;
}

EventType WindowBase::focusEvent(bool has_focus) {
    this->has_focus   = has_focus;
    EventType e       = {EventType::FOCUS};
    e.focus.has_focus = has_focus;
    return e;
}

EventType WindowBase::gpadConnect(uint8_t pad, bool active) {
    gamepad[pad].active = active;
    EventType e = {EventType::GPAD_CONNECT};
    e.gp_connect.pad = pad;
    e.gp_connect.active = active;
    return e;
}

EventType WindowBase::gpadButton(uint8_t pad, uint8_t btn, bool down) {
    gamepad[pad].buttons[btn] = down;
    EventType e = {EventType::GPAD_BUTTON};
    e.gp_button.pad = pad;
    e.gp_button.down = down;
    e.gp_button.btn = btn;
    return e;
}

EventType WindowBase::gpadAxis(uint8_t pad, uint8_t axis, float val) {
    gamepad[pad].axes[axis] = val;
    EventType e = {EventType::GPAD_AXIS};
    e.gp_axis.pad  = pad;
    e.gp_axis.axis = axis;
    e.gp_axis.val  = val;
    return e;
}

EventType WindowBase::closeEvent() {
    running = false;
    return {EventType::CLOSE};
}
//----------

bool WindowBase::processEvents(bool wait_for_event) {
    EventType e = getEvent(wait_for_event);
    //if(e.tag == EventType::NONE) onIdleEvent();
    while (e.tag != EventType::NONE) {
        running = processEvent(e);  // Call event handlers
        if(!running) return false;
        e = getEvent();
    }
    onFrame();
    return running;
}

bool WindowBase::processEvent(EventType e) {
    switch (e.tag) {
       case EventType::MOUSE       : onMouse      (e.mouse.action, e.mouse.x, e.mouse.y, e.mouse.btn);  break;
       case EventType::KEY         : onKey        (e.key.action, e.key.keycode);                        break;
       case EventType::TEXT        : onText       (e.text.str);                                         break;
       case EventType::MOVE        : onMove       (e.move.x, e.move.y);                                 break;
       case EventType::RESIZE      : onResize     (e.resize.width, e.resize.height);                    break;
       case EventType::FOCUS       : onFocus      (e.focus.has_focus);                                  break;
       case EventType::TOUCH       : onTouch      (e.touch.action, e.touch.x, e.touch.y, e.touch.id);   break;
       case EventType::GPAD_CONNECT: onGpadConnect(e.gp_connect.pad, e.gp_connect.active);              break;
       case EventType::GPAD_BUTTON : onGpadButton (e.gp_button.pad, e.gp_button.btn, e.gp_button.down); break;
       case EventType::GPAD_AXIS   : onGpadAxis   (e.gp_axis.pad, e.gp_axis.axis, e.gp_axis.val);       break;
       case EventType::CLOSE       : onClose      (); return false;
       default: break;
    }
    return true;
}



// -------------------------------
// android/entry.cpp
// -------------------------------

#ifdef VK_USE_PLATFORM_ANDROID_KHR

android_app* Android_App = nullptr;  // Android native-activity state
int main(int argc, char *argv[]);    // Forward declaration of main function

//--------------------------------------Application Entry Point-------------------------------------
void android_main(struct android_app* state) {
    Android_App = state;                                             // Pass android app state to window_android.cpp
    android_fopen_set_asset_manager(state->activity->assetManager);  // Re-direct fopen to read assets from our APK.
    main(0, nullptr);                                                // call main()
    ANativeActivity_finish(state->activity);                         // exit
}
//--------------------------------------------------------------------------------------------------
#endif // VK_USE_PLATFORM_ANDROID_KHR

#endif // GWINDOW_IMPLEMENTATION
