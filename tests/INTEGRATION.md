# GWindow Tests & Integration Methods

This folder contains example applications that demonstrate how to integrate GWindow into your project.  GWindow supports **two integration methods**:

- **Single-Header** — minimal setup, stb-style header + implementation.
- **Modular Library** — traditional CMake subproject with a compiled library target.

Each method has a corresponding test app to showcase usage.  
The CMake build scripts are for targeting Windows, Linux and Android.  
Android builds also require the additional gradle, manifest and icon files.  

---

## 🧩 Integration Methods
### 1. Single-Header Integration

The single-hearer-file version of GWindow is easiest to deploy and avoids clutter:

Copy **`GWindow.h`** and optionally, **`GWindow.cmake`** into your project.

In your **`CMakeLists.txt`**:
```cmake
   add_executable(myapp main.cpp)
   include(GWindow.cmake)
   add_gwindow(myapp)
```

In one .cpp file:
```cpp
   #define GWINDOW_IMPLEMENTATION
   #include "GWindow.h"
```

✅ Minimal files, fast compile, no external build step.  
❌ Less modular if you plan to extend or modify the library itself.

### 2. Modular Library Integration

Use this method if you prefer a modular library subproject:

Add the **`Window/`** folder to your project.

In your **`CMakeLists.txt`**:
```cmake
    add_subdirectory(path/to/Window Window)
    add_executable(myapp main.cpp)
    target_link_libraries(myapp Window)
```
Include in code:
```cpp
    #include "Window.h"
```
✅ The library version of GWindow is easier to extend or modify.  
❌ Slightly more setup and clutter than the single-header version.

## 🧪 Test Applications

The first two example apps are functionally equivalent, to demonstrate each method:

- **test_header** — Shows the single-header-file integration.  
- **test_lib** — Shows the modular library integration.

Both apps create a window, draws a test image, and prints events to the terminal.

- **test_gles** — A minimal OpenGLES app that just changes the background color.  
On Linux, run '*requirements.sh*' to install the dev package for EGL+GLES headers.  
On Windows, run '*requirements.bat*' to install ANGLE EGL+GLES dll's and headers.  

## 🔨 Building Tests
### Desktop (Windows / Linux)

#### Using an IDE (recommended):

1. Open `tests/CMakeLists.txt` in your IDE (MSVC, QtCreator, CLion, etc.).
2. The IDE will generate a project containing both apps: `test_header` and `test_lib`.
3. Choose one of the apps, then build and run from the IDE.
4. Use the terminal to view triggered event messsages.

#### Using the terminal (alternative):

```bash
    cd tests
    mkdir build && cd build
    cmake .. -DCMAKE_BUILD_TYPE=Debug
    cmake --build .
```
Run either `test_header` or `test_lib`.

### Android

#### Using Android Studio (recommended):

1. Open the `tests/` folder in Android Studio (contains Gradle files).
2. Android Studio will create a project with both apps: `test_header` and `test_lib`.
3. Select the app you want to run. Build, deploy, and run on your device or emulator.
4. Use logcat to view input event messages (touchscreen/keyboard/mouse/gamepad).

#### Using the terminal (alternative):
```bash
    ./gradlew build
    ./gradlew installDebug
```
## 📌 Notes

- `showImage()` is intended for software rendering only, using a memory buffer.
- For Vulkan or OpenGL, use `getNativeHandle()` to create the rendering context.
- Place asset files in `tests/*/assets/`, to include in the Android APK.
- On desktop, the test apps expect to find `assets/` in the local directory.
- On Linux, the test app, compiled with MinSizeRel, is 52 KB.
- On Windows, the test app, compiled with MinSizeRel, is 26 KB.