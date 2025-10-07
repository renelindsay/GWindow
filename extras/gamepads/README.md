# Gamepad Mappings Generator

The `gamepads.py` script generates the gamepad layout mappings for the **GWindow** library.  
It takes `gamecontrollerdb.txt` as input, and generates the `gamepads.h` output file.

## Overview

Gamepads from different vendors have wildly inconsistent button and axes layouts.  
To provide a consistent API across platforms, GWindow uses a lookup table of known layouts.  


## Files:

- **`gamecontrollerdb.txt`**
  Obtained from the community-maintained [SDL_GameControllerDB](https://github.com/mdqinc/SDL_GameControllerDB).

- **`gamepads.py`**
  
  The Python script that parses `gamecontrollerdb.txt` to generate the `gamepads.h` header.
  The script:
  
  - Filters out incomplete or duplicate controller entries.
  - Normalizes mappings into a compact lookup format.
  - Generates VID/PID → layout index tables for quick runtime matching.
  - Provides string-based fallback matching for controllers without VID/PID.

- **`gamepads.h`**  
  The generated C++ header file which gets included in GWindow.  
  It contains:
  
  - `constexpr` arrays of standardized button layouts.
  - Lookup tables indexed by USB vendor/product ID or controller name.
  - A helper function to retrieve the correct layout at runtime.

## Usage

1. Ensure you have the latest `gamecontrollerdb.txt` from the SDL GameControllerDB repository.

2. Run the script to regenerate `gamepads.h`:
   
   `python3 gamepads.py`

3. Include the generated `gamepads.h` in your GWindow build.  
   At runtime, GWindow will:
   
   - Identify connected controllers by VID/PID (or name).
   - Select the appropriate standardized button layout.
   - Provide consistent mapping of buttons and axes across platforms.

## Notes

- Provides gamepad lookup tables for **Linux** and **Android** platforms.
- On **Windows**, GWindow uses **XInput**, which already works with all **XInput** compatible gamepads.
- Controllers missing essential buttons or with invalid mappings are discarded during generation.
- Some controllers may still require manual testing and validation.
