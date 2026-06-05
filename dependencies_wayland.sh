sudo apt install libwayland-dev wayland-protocols     # Wayland

mkdir ./Window/wayland/

wayland-scanner client-header \
  /usr/share/wayland-protocols/stable/xdg-shell/xdg-shell.xml \
  ./Window/wayland/xdg-shell-protocol.h

wayland-scanner private-code \
  /usr/share/wayland-protocols/stable/xdg-shell/xdg-shell.xml \
  ./Window/wayland/xdg-shell-protocol.c
  
wayland-scanner client-header \
  /usr/share/wayland-protocols/unstable/xdg-decoration/xdg-decoration-unstable-v1.xml \
  ./Window/wayland/xdg-decoration.h

wayland-scanner private-code \
  /usr/share/wayland-protocols/unstable/xdg-decoration/xdg-decoration-unstable-v1.xml \
  ./Window/wayland/xdg-decoration.c
