# GWindow Developer Guide

This guide is for developers extending GWindow, such as adding platform support or modifying internals.   

For user-facing API usage, see `API-Reference.md`.

---

## Overview

GWindow uses a class hierarchy:

- **`WindowBase` (`WindowBase.h`)**: Abstract base class with core logic.
- **Platform Classes**: `Window_xcb`, `Window_win32`, `Window_android` implement platform-specific functionality.
- **`GWindow` (`Window.h`)**: Alias for the platform-specific class.

---

## Internal Components

### Protected Members (`WindowBase`)

- `Mouse mouse`: Mouse position/buttons.
- `bool keystate[256]`: Keyboard state.
- `Gamepad gamepad[MAX_GAMEPADS]`: Up to 4 gamepads.
- `EventFIFO eventFIFO`: Event queue (64 slots).
- `float display_scale`: HiDPI scaling.
- `bool running`, `has_focus`, `is_resized`, `fullscreen`: Window state.
- `struct shape_t { int16_t x, y; uint16_t width, height; } shape`: Window geometry.
- `std::string clipboard`: Clipboard storage.

### Helper Classes

- **`EventFIFO`**: Manages event queue (`push`, `pop`, `isEmpty`).
- **`CMTouch`**: Handles multi-touch (up to 10 pointers; `Clear`, `Event`, `Event_by_ID`).

### Protected Event Methods

Generate `EventType` objects:

- `mouseEvent`, `keyEvent`, `textEvent`, `moveEvent`, `resizeEvent`, `focusEvent`, `gpadConnect`, `gpadButton`, `gpadAxis`, `closeEvent`.

---

## Adding Platform Support

1. **Create Class**:
   
   - Define `Window_newplatform` in `window_newplatform.h`, inheriting from `WindowBase`.
   - Implement `getEvent()`, `getNativeHandle()`, and other virtual methods.

2. **Update `Window.h`**:
   
   ```cpp
   #elif defined(VK_USE_PLATFORM_NEWPLATFORM_KHR)
       typedef Window_newplatform GWindow;
   #include "window_newplatform.h"
   ```

3. **Implement Logic**:
   
   - Use platform APIs to manage windows and inputs.
   - Update `shape`, `mouse`, etc., and push events to `eventFIFO`.

4. **Test**:
   
   - Add tests in `tests/`.
   - Update CMake/Gradle scripts.

---

## Amalgamation

Run `amalgamate.py` to combine `Window/` sources into `GWindow.h`:

```bash
python3 amalgamate.py
```

- Update script for new platform headers.

---

## Guidelines

- **Minimize Dependencies**: Avoid external libraries.
- **Style**: Match `WindowBase.h` conventions.
- **Tests**: Update `tests/` for new features.
- **Documentation**: Update `docs/todo.txt` and `API-Reference.md` if user-facing APIs change.

---

## Resources

- **Source**: `Window/`.
- **Tests**: `tests/test_header/`, `tests/test_lib/`.
- **Gamepad Mappings**: `gamepads/gamecontrollerdb.txt`.
- **Documentation**: `docs/` (see `docs/todo.txt`).