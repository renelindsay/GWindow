#!/usr/bin/python3

# amalgamate.py for GWindow:
# This script merges the source files from the /Window/ library
# into an stb-style single header file, called 'GWindow.h'

import os

# header files
header_files = [
    "config.h",
    "keycodes.h",
    "WindowBase.h",
    "gamepads.h",
    "window_win32.h",
    "window_xcb.h",
    
    "android/android_fopen.h",
    "android/JClass.h",
    "window_android.h",
    
    "Window.h",
]

# cpp files
cpp_files = [
    "WindowBase.cpp",
    "android/entry.cpp"
]

# Output file
output_file = "GWindow.h"

def writelines(file, guard, out):
    if guard: out.write(f"#ifdef {guard}\n")
    with open("Window/"+file, "r") as f:
        print(f"{file}")
        for line in f:
            stripped = line.strip()
            if stripped.startswith('#include "'):
                #print(f"{file:<20}:  {stripped}")
                continue
            out.write(line)
    if guard: out.write(f"#endif // {guard}\n")        
    out.write("\n")

if __name__ == "__main__":
    with open(output_file, "w") as out:
            out.write("// =========\n")
            out.write("// GWindow.h\n")
            out.write("// =========\n\n")
            
            out.write("// How to use GWindow.h:\n")
            out.write("// #define GWINDOW_IMPLEMENTATION    // Do this in one .cpp file only.\n")
            out.write('// #include "GWindow.h"              // Do this wherever GWindow is used.\n\n')
            
            out.write("#ifndef GWINDOW_H\n")
            out.write("#define GWINDOW_H\n\n")

            # header files
            for file in header_files:
                out.write(f"// -------------------------------\n")
                out.write(f"// {file}\n")
                out.write(f"// -------------------------------\n\n")
                guard = "VK_USE_PLATFORM_ANDROID_KHR" if file.startswith("android/") else ""
                writelines(file, guard, out);

            out.write("#endif // GWINDOW_H\n\n")

            # cpp files
            out.write("#ifdef GWINDOW_IMPLEMENTATION\n\n")

            for file in cpp_files:
                out.write(f"// -------------------------------\n")
                out.write(f"// {file}\n")
                out.write(f"// -------------------------------\n\n")
                guard = "VK_USE_PLATFORM_ANDROID_KHR" if file.startswith("android/") else ""
                writelines(file, guard, out);

            out.write("#endif // GWINDOW_IMPLEMENTATION\n")



