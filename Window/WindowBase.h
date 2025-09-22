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
    virtual const void* getNativeHandle() const = 0;              // For creating Vulkan/OpenGL Surface
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
