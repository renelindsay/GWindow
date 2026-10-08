#!/usr/bin/python3

# amalgamate.py for GWindow:
# This script merges the source files from the /Window/ library
# into an stb-style single header file, called 'GWindow.h'

import os
import textwrap

# header files
header_files = [
    "config.h",
    "keycodes.h",
    "WindowBase.h",
    "gamepads.h",    
    "gamepad_linux.h",
    "window_win32.h",
    "window_xcb.h",
    "window_wayland.h",
    
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
            out.write(textwrap.dedent("""\
            /*
             * GWindow.h - Lightweight cross-platform window + input library
             *            (Windows / Linux / Android) for Vulkan, OpenGL(ES) or software rendering.
             *
             * https://github.com/renelindsay/GWindow
             *
             * Copyright (c) 2024-2026 Rene Lindsay
             *
             * Permission is hereby granted, free of charge, to any person obtaining a copy
             * of this software and associated documentation files (the "Software"), to deal
             * in the Software without restriction, including without limitation the rights
             * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
             * copies of the Software, and to permit persons to whom the Software is
             * furnished to do so, subject to the following conditions:
             *
             * The above copyright notice and this permission notice shall be included in all
             * copies or substantial portions of the Software.
             *
             * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
             * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
             * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
             * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
             * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
             * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
             * SOFTWARE.
             *
             */\n    
             """))
    
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



