#!/bin/bash

# This script downloads dependencies for building GWindow on Ubuntu 24.04.
# Most runtime dependencies are already included in a fresh install of Linux,
# except for libxcb-cursor0 for mouse cursors, and libxcb-icccm4 for clipboard support.
# Either install them, or disable those features in config.h.

sudo apt install build-essential ninja-build          # C++ compiler
sudo apt install libx11-xcb-dev                       # X11/XCB
sudo apt install libxi-dev                            # Multitouch
sudo apt install libxkbcommon-dev                     # Keyboard
sudo apt install libxcb-image0-dev                    # ShowImage
sudo apt install libevdev-dev                         # Gamepad
sudo apt install libxcb-icccm4  libxcb-icccm4-dev     # Clipboard
sudo apt install libxcb-cursor0 libxcb-cursor-dev     # Mouse cursors

