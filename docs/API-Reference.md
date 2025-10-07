# API Reference — GWindow

This document describes the **public API** of the **GWindow** library, structs and enums.

---

## 🔑 Core Types

### `enum eAction`

Represents input actions:

* `eUP   = 0` — key/button released
* `eDOWN = 1` — key/button pressed
* `eMOVE = 2` — pointer moved

---

### `enum eKeycode`

Represents keyboard keys (USB HID codes):

* `eKEY_A = 4` — A key
* `eKEY_B = 5` — B key
* `eKEY_C = 6` — C key
* ... (See `keyboard.h` for full list.)

---

### `enum eGamepadBtn`

Represents gamepad buttons:

* `eBTN_UNKNOWN = 0`  — (Unknown button)
* `eBTN_A       = 1`  — A
* `eBTN_B       = 2`  — B
* `eBTN_X       = 3`  — X
* `eBTN_Y       = 4`  — Y
* `eBTN_TL      = 5`  — Sholder Left
* `eBTN_TR      = 6`  — Sholder Right
* `eBTN_THUMBL  = 7`  — Thumb Left
* `eBTN_THUMBR  = 8`  — Thumb Right
* `eDPAD_UP     = 9`  — DPad up
* `eDPAD_DOWN   = 10` — DPad down
* `eDPAD_LEFT   = 11` — DPad left
* `eDPAD_RIGHT  = 12` — DPad right
* `eBTN_SELECT  = 13` — (reserved for system use)
* `eBTN_START   = 14` — Start button
* `eBTN_MODE    = 15` — (reserved for system use)

---

### `enum eGamepadAxis`

Represents gamepad analog thumbsticks and triggers:

* `eAXIS_UNKNOWN = 0` — (Unknown button)
* `eAXIS_LX      = 1` — left thumbstick  -1to1 (right is positive)
* `eAXIS_LY      = 2` — left thumbstick  -1to1 (up is positive)
* `eAXIS_RX      = 3` — right thumbstick -1to1 (right is positive)
* `eAXIS_RY      = 4` — right thumbstick -1to1 (up is positive)
* `eAXIS_TL      = 5` — left trigger      0to1
* `eAXIS_TR      = 6` — right trigger     0to1

---

### `enum eCursor`

Mouse cursor shapes supported by `setCursor()`:

* `eArrow`, `eCaret`, `eResizeAll`, `eResizeNS`, `eResizeEW`,
* `eResizeNESW`, `eResizeNWSE`, `eHand`, `eWait`, `eProgress`, `eNotAllowed`

Examples: `eArrow` (default arrow), `eCaret` (text selection), `eHand` (clickable link).

---

### `struct EventType`

The event object returned by `getEvent()` and passed through the event loop.

```cpp
struct EventType {
    enum Tag { NONE, MOUSE, KEY, TEXT, MOVE, RESIZE, FOCUS, TOUCH,
               CLOSE, GPAD_CONNECT, GPAD_BUTTON, GPAD_AXIS, UNKNOWN } tag;
    union {
        struct { eAction action; int16_t x; int16_t y; uint8_t btn; } mouse;
        struct { eAction action; eKeycode keycode; } key;
        struct { const char* str; } text;
        struct { int16_t x; int16_t y; } move;
        struct { uint16_t width; uint16_t height; } resize;
        struct { bool has_focus; } focus;
        struct { eAction action; float x; float y; uint8_t id; } touch;
        struct { uint8_t pad; bool active; } gp_connect;
        struct { uint8_t pad; uint8_t btn; bool down; } gp_button;
        struct { uint8_t pad; uint8_t axis; float val; } gp_axis;
        struct {} close;
    };
    operator bool() const;
};
```

Usage:

* `tag` specifies which union member is valid.
* `operator bool()` is true if `tag != NONE`.

---

### `struct Gamepad`

Represents state of one gamepad.

```cpp
struct Gamepad {
    bool active;        // True if this gamepad is connected
    bool buttons[16];   // gamepad button states (true if pressed)
    float axes[8];      // thumbsticks(-1 to 1) and triggers(0 to 1)
};
```

* Up to **4 gamepads** are supported simultaneously.

---

### `struct Mouse`

Represents mouse state:

```cpp
struct Mouse {
    struct { int16_t x; int16_t y; } pos;  // mouse x,y position
    bool btn[6];                           // mouse button states
};
```

---

## 🖼️ Class: `GWindow`

### Lifecycle

```cpp
GWindow();
virtual ~GWindow();
virtual void close();  // posts a window close event.
```

If window is closing (after clicking the close button or calling close()):

- isRunning() will return false
- Event loop functions will return false
- Run() function will exit

---

### 📝 State Queries

* `void getPosition(int16_t& x, int16_t& y)` — returns window x,y position
* `void getSize(int16_t& w, int16_t& h)` — returns window dimensions
* `void getSize(int32_t& w, int32_t& h)` — returns window dimensions
* `bool getKeyState(eKeycode key)` — returns true if **keyboard key** is pressed
* `bool getBtnState(uint8_t btn)` — returns true if **mouse button** is pressed
* `void getMousePos(int16_t& x, int16_t& y)` — returns mouse x,y position
* `Gamepad& getGamepad(uint8_t pad)` — returns state of given gamepad(0-3)
* `bool isRunning()` — returns false if window is closing
* `uint width()` — returns window width
* `uint height()` — return window height
* `bool resized()` — returns true if window was resized since last call
* `float getScale()` — returns the current window dpi scale setting (see setScale)
* `float getDisplayScale()` — returns system dpi scale setting
* `bool isFullscreen()` — returns true if window is fullscreen

---

### 📋 Clipboard (Copy/Paste)

* `const char* getClipboardText()` — return the clipboard contents as a string
* `void setClipboardText(const char* str)` — push text onto the clipboard

(Default implementation stores locally; platform-specific backends override.)

---

### 🎛️ Control Functions

* `void showKeyboard(bool enabled)` — show/hide Android soft-keyboard.
* `void setTitle(const char* title)` — Set Window title
* `void setPosition(uint x, uint y)` — Set Window position
* `void setSize(uint w, uint h)` — Set Window width and height
* `void setScale(float val)` — Set HiDPI scaling override. (0 for system scaling)
* `void setSizeScaled(uint w, uint h)` — applies display scale factor.
* `const void* getNativeHandle()` — create Vulkan/OpenGL surface. (see: [VkWindow](../extras/for Vulkan/README.md) )
* `void setCursor(eCursor id)` — Set mouse icon (pointer, caret, spinner, etc.)
* `void setFullscreen(bool enable)` — Set fullscreen or windowed mode
* `void showImage(uint32_t* buf, uint32_t width, uint32_t height)`
    — show image buffer (for software rendering)

---

### 🎮 Event Handlers (override in subclass)

* `void onMouse(eAction action, int16_t x, int16_t y, uint8_t btn)` — mouse event
* `void onKey(eAction action, eKeycode keycode)` — keyboard key press/release
* `void onText(const char* str)` — text input from keyboard (typed characters)
* `void onMove(int16_t x, int16_t y)` — window move to new position/
* `void onResize(uint16_t width, uint16_t height)` — window resized
* `void onFocus(bool hasFocus)` — window gained or lost focus.
* `void onTouch(eAction action, float x, float y, uint8_t id)` — touchscreen event
* `void onGpadConnect(uint8_t pad, bool active)` — gamepad (dis)connect
* `void onGpadButton(uint8_t pad, uint8_t btn, bool down)` — gamepad button event
* `void onGpadAxis(uint8_t pad, uint8_t axis, float val)` — thumbstick/trigger event
* `void onClose()` — window close event
* `void onFrame()` — called once per frame, ideal for rendering

---

### 🔄 Event Loop

GWindow supports three event processing styles.  
Start simple, and move to manual if you need finer control.  

If `wait=true`, the function blocks until an event arrives (power saving mode).  
If `wait=false`, the function returns immediately, for continuous animations.  
Non-blocking mode is suitable for games. Blocking is better for reactive GUIs.

1. Automatic (simple):
   * `void Run(bool wait=true)` — dispatch events continuously
2. Batch processing:
   * `bool processEvents(bool wait=false)` — dispatch all queued events
   * `bool pollEvents()` — short for `processEvents(false)` (non-blocking)
   * `bool waitEvents()` — short for `processEvents(true)`  (blocking)
3. Manual:
   * `EventType getEvent(bool wait=false)` — fetch next event from queue
   * `bool processEvent(EventType e)` — dispatch event to its handler

Returned bool value is false if the window is closing.
See examples below:

---

## 🌀 Typical Usage

#### 1. Automatic (Easiest)

Subclass GWindow, add event handlers, create the window, then simply call `Run()`:

```cpp
class MyWindow : public GWindow {
    // EVENT HANDLERS:
    void onResize(uint16_t w, uint16_t h) override {
        printf("Resized: %dx%d\n", w, h);
    }
    void onFrame() override {
        // Render a frame here
    }
};

int main(int argc, char *argv[]) {
    MyWindow win;                  // Create window
    win.setTitle("Example");       // Set window title
    win.setSize(800, 600);         // Set window size
    win.Run(true);                 // Run until window closed (blocking)
}
```

#### 2. Batch processing (Keeps main loop)

If you prefer not hiding the processing loop, use this approach instead.  
Create a main loop where you dispatch all waiting events, and render.  

```cpp
class MyWindow : public GWindow {
    void onResize(uint16_t w, uint16_t h) override {
        printf("Resized: %dx%d\n", w, h);
    }
};

int main(int argc, char *argv[]) {
    MyWindow win;                      // Create window
    win.setTitle("Example");           // Set window title
    win.setSize(800, 600);             // Set window size
    while(win.processEvents(false)) {  // Loop until window closed (non-blocking)
        // Render frame here. (eg. Vulkan / OpenGL / etc.)
    }
}
```

#### 3. Manual (Most Control)

Fetch events one-by-one, using `getEvent()` and either:  

- Use `processEvent(e)` to dispatch events to their handlers, or  
- Handle events directly with a switch statement.  

Make sure to drain the event queue each frame before rendering:

```cpp
int main(int argc, char *argv[]) {
    GWindow win;                            // create window(no handlers)
    win.setTitle("Manual Example");         // set window title
    win.setSize(800, 600);                  // set window size

    while (win.isRunning()) {               // Run until window is closed
        EventType e = win.getEvent();       // Fetch event from FIFO queue
        while (e) {                         // While queue is not empty...
            //win.processEvent(e);          // Dispatch event to handler
            switch (e.tag) {                // OR: process event directly
                case EventType::RESIZE: {   // Window resized event
                    int w = e.resize.width;
                    int h = e.resize.height;
                    printf("Resize:(%d,%d)\n", w, h);
                }break;
                case EventType::MOUSE: {    // mouse event
                    int a = e.mouse.action; // up=0 down=1 move=2
                    int x = e.mouse.x;
                    int y = e.mouse.y;
                    const char* action[]={"up","down","move"};
                    printf("Mouse:%s (%d,%d)\n", action[a], x, y);
                }break;
                default:
                break;
            }
            e = win.getEvent();             // Fetch next event
        }
        // Render one frame here
    }
}
```
