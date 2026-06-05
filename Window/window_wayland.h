//==========================WAYLAND=============================
#ifdef VK_USE_PLATFORM_WAYLAND_KHR

#ifndef WINDOW_WAYLAND
#define WINDOW_WAYLAND

#include "WindowBase.h"
#include "wayland/xdg-decoration.h"
#include "wayland/xdg-shell-protocol.h"

struct native_handle {
    wl_display* display;
    wl_surface* surface;
};

//==========================WAYLAND=============================
class Window_wayland : public WindowBase {
    wl_display*    display    = nullptr;
    wl_surface*    surface    = nullptr;
    wl_registry*   registry   = nullptr;
    wl_compositor* compositor = nullptr;
    xdg_wm_base*   xdg_wm     = nullptr;
    xdg_surface*   xdg_surf   = nullptr;
    xdg_toplevel*  toplevel   = nullptr;


    void Create(const char* title="Window", uint width=640, uint height=480);
public:
    Window_wayland() {Create();}
    Window_wayland(const char* title, uint width, uint height);
    virtual ~Window_wayland();

    EventType getEvent(bool wait_for_event = false);

    native_handle* getNativeHandle() const {return (native_handle*)&display;}
};
//==============================================================
#endif  // WINDOW_WAYLAND

#ifdef GWINDOW_IMPLEMENTATION
Window_wayland::Window_wayland(const char* title, uint width, uint height) {
    Create(title, width, height);
}

void Window_wayland::Create(const char* title, uint width, uint height) {
    shape.width  = width;
    shape.height = height;
    running      = true;

    //printf("Creating XCB-Window...\n");

}


EventType Window_wayland::getEvent(bool wait_for_event) {
    return {EventType::NONE};
}

Window_wayland::~Window_wayland() {};

#endif // GWINDOW_IMPLEMENTATION

#endif  // VK_USE_PLATFORM_WAYLAND_KHR
//==============================================================