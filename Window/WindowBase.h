/*
*--------------------------------------------------------------------------
* FIFO Buffer is used in the few cases where event messages need to be buffered or swapped.
* EventType contains a union struct of all possible message types that may be returned by GetEvent.
* WindowBase is the abstract base class for all the platform-specific window classes.
*--------------------------------------------------------------------------
*/


#ifndef WINDOWBASE_H
#define WINDOWBASE_H

#include <cstring>
#include <cstdint>
#include <string>

#include "keycodes.h"

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
//======================Window base class=======================
class WindowBase {
protected:
    Mouse mouse;                                                               // mouse state
    bool keystate[256] = {};                                                   // keyboard state
    Gamepad gamepad[MAX_GAMEPADS];                                             // gamepad state

    EventFIFO eventFIFO;                                                       // Event message queue buffer
    EventType MouseEvent (eAction action, int16_t x, int16_t y, uint8_t btn);  // Mouse event
    EventType KeyEvent   (eAction action, uint8_t key);                        // Keyboard event
    EventType TextEvent  (const char* str);                                    // Text event
    EventType MoveEvent  (int16_t x, int16_t y);                               // Window moved
    EventType ResizeEvent(uint16_t width, uint16_t height);                    // Window resized
    EventType FocusEvent (bool has_focus);                                     // Window gained/lost focus   
    EventType GPadConnect(uint8_t pad, bool active);                           // Gamepad connect/disconnect
    EventType GPadButton (uint8_t pad, uint8_t btn, bool down);                // Gamepad button event
    EventType GPadAxis   (uint8_t pad, uint8_t axis, float val);               // Gamepad axis events
    EventType CloseEvent ();                                                   // Window closing

    float display_scale = 0.f;
    bool running;
    bool has_focus;                                                            // true if window has focus
    bool resized;                                                              // true if window has been resized
    bool fullscreen;                                                           // true if window is fullscreen
    struct shape_t { int16_t x; int16_t y; uint16_t width; uint16_t height; } shape = {};  // window shape
    std::string clipboard;

  public:
    WindowBase() : running(false), has_focus(false), resized(false), fullscreen(false){}
    virtual ~WindowBase() {}
    virtual void Close() { eventFIFO.push(CloseEvent()); }

    //--State query functions--
    //shape_t GetShape (){return shape;}                                             // return window shape in pixels
    void  GetPosition(int16_t& x, int16_t& y) { x = shape.x; y = shape.y; }                  // return window position
    void  GetSize(int16_t& width, int16_t& height) { width = Width(); height = Height(); }   // return window size
    void  GetSize(int32_t& width, int32_t& height) { width = Width(); height = Height(); }   // return window size
    bool  GetKeyState(eKeycode key) { return keystate[key]; }                      // return true if key is pressed
    bool  GetBtnState(uint8_t  btn) { return (btn < 6) ? mouse.btn[btn] : 0; }     // return true if mouse btn is pressed
    void  GetMousePos(int16_t& x, int16_t& y) {x = mouse.pos.x; y = mouse.pos.y;}  // return mouse x,y position
    Gamepad& GetGamepad(uint8_t pad) {return gamepad[pad];}                        // return the gamepad state

    bool IsRunning() { return running; }
    uint Width() {return shape.width; }
    uint Height(){return shape.height;}
    bool Resized() { bool resize = resized; resized = false; return resize; }
    float GetScale() {return (display_scale>0)? display_scale : GetDisplayScale();}
    void  SetScale(float val) {display_scale = val;}

    virtual float GetDisplayScale() {return 1.f;}
    virtual bool IsFullscreen() {return fullscreen;}

    //--Clipboard--
    virtual const char* GetClipboardText() {return clipboard.c_str(); }  // Fallback implementation works only locally.
    virtual void SetClipboardText(const char* str) { clipboard = str; }  // Platform implementations overrides this.

    //--Control functions--
    virtual void ShowKeyboard(bool enabled) {}                    // Shows the Android soft-keyboard.
    virtual void SetTitle(const char* title) {}
    virtual void SetPosition(uint x, uint y) {}
    virtual void SetSize(uint w, uint h) {}
    virtual const void* GetNativeHandle() const = 0;              // For creating Vulkan/OpenGL Surface
    virtual void ShowImage(uint32_t* buf, uint32_t width, uint32_t height) {}
    virtual void SetCursor(eCursor id) {}
    virtual void SetFullscreen(bool enable) {}
    void SetSizeScaled(uint w, uint h) {float s=GetScale(); SetSize(w*s, h*s);}

    //--Event loop--
    virtual EventType GetEvent(bool wait_for_event = false) = 0;  // Fetch one event from the queue.
    bool ProcessEvents(bool wait_for_event = false);              // Dispatch all waiting events to event handlers. Returns false if window is closing.
    bool ProcessEvent (EventType e);                              // Dispatch/inject the given event to event handlers.
    bool PollEvents() { return ProcessEvents(false); }            // Run continuously
    bool WaitEvents() { return ProcessEvents(true ); }            // Pause app when there are no events to process
    // void Run(){ while(ProcessEvents()){} }                     // Run message loop until window is closed.

    //-- Virtual Functions as event handlers --
    virtual void OnMouseEvent(eAction action, int16_t x, int16_t y, uint8_t btn) {}  // Callback for mouse events
    virtual void OnKeyEvent(eAction action, eKeycode keycode) {}                     // Callback for keyboard events (keycodes)
    virtual void OnTextEvent(const char *str) {}                                     // Callback for text typed events (text)
    virtual void OnMoveEvent(int16_t x, int16_t y) {}                                // Callback for window move events
    virtual void OnResizeEvent(uint16_t width, uint16_t height) {}                   // Callback for window resize events
    virtual void OnFocusEvent(bool hasFocus) {}                                      // Callback for window gain/lose focus events
    virtual void OnTouchEvent(eAction action, float x, float y, uint8_t id) {}       // Callback for Multi-touch events
    virtual void OnGPadConnect(uint8_t pad, bool active){}                           // Callback for Joystick connect/disconnect
    virtual void OnGPadButton(uint8_t pad, uint8_t btn, bool down){}                 // Callback for Joystick button events
    virtual void OnGPadAxis(uint8_t pad, uint8_t axis, float val){}                  // Callback for Joystick axis events
    virtual void OnCloseEvent() {}                                                   // Callback for window closing event
};
//==============================================================

#endif
