//==========================WAYLAND=============================
#ifdef VK_USE_PLATFORM_WAYLAND_KHR

#ifndef WINDOW_WAYLAND
#define WINDOW_WAYLAND

#include "WindowBase.h"
#include <wayland-client.h>
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

    // --- Registry listener ---
    static void on_global(void* data, wl_registry* reg, uint32_t name, const char* iface, uint32_t version) {
        auto* w = static_cast<Window_wayland*>(data);
        if (strcmp(iface, wl_compositor_interface.name) == 0)
            w->compositor = (wl_compositor*)wl_registry_bind(reg, name, &wl_compositor_interface, 4);
        else if (strcmp(iface, xdg_wm_base_interface.name) == 0)
            w->xdg_wm = (xdg_wm_base*)wl_registry_bind(reg, name, &xdg_wm_base_interface, 1);
    }
    static void on_global_remove(void*, wl_registry*, uint32_t) {}
    static constexpr wl_registry_listener registry_listener = { on_global, on_global_remove };

    // --- xdg_wm_base ping/pong (required or compositor kills the client) ---
    static void on_ping(void*, xdg_wm_base* base, uint32_t serial) {xdg_wm_base_pong(base, serial);}
    static constexpr xdg_wm_base_listener wm_base_listener = { on_ping };

    // --- xdg_surface configure (must ack or nothing appears) ---
    static void on_xdg_surface_configure(void* data, xdg_surface* surf, uint32_t serial) {
        auto* w = static_cast<Window_wayland*>(data);
        xdg_surface_ack_configure(surf, serial);
        wl_surface_commit(w->surface);
    }
    static constexpr xdg_surface_listener surface_listener = { on_xdg_surface_configure };

    // --- xdg_toplevel events ---
    static void on_toplevel_configure(void* data, xdg_toplevel*,  int32_t w, int32_t h, wl_array*) {
        if (w > 0 && h > 0) {
            auto* self = static_cast<Window_wayland*>(data);
            self->eventFIFO.push(self->resizeEvent(w, h));
        }
    }
    static void on_toplevel_close(void* data, xdg_toplevel*) {
        auto* self = static_cast<Window_wayland*>(data);
        self->eventFIFO.push(self->closeEvent());
    }
    static constexpr xdg_toplevel_listener toplevel_listener = { on_toplevel_configure, on_toplevel_close };


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

    display  = wl_display_connect(nullptr);
    registry = wl_display_get_registry(display);
    wl_registry_add_listener(registry, &registry_listener, this);
    wl_display_roundtrip(display);  // populates compositor + xdg_wm

    xdg_wm_base_add_listener(xdg_wm, &wm_base_listener, this);

    surface  = wl_compositor_create_surface(compositor);
    xdg_surf = xdg_wm_base_get_xdg_surface(xdg_wm, surface);
    xdg_surface_add_listener(xdg_surf, &surface_listener, this);

    toplevel = xdg_surface_get_toplevel(xdg_surf);
    xdg_toplevel_add_listener(toplevel, &toplevel_listener, this);
    xdg_toplevel_set_title(toplevel, title);

    wl_surface_commit(surface);
    wl_display_roundtrip(display);  // triggers the initial configure

    eventFIFO.push(resizeEvent(width, height));
}


EventType Window_wayland::getEvent(bool wait_for_event) {
    if (!eventFIFO.isEmpty()) return eventFIFO.pop();

    if (wait_for_event) wl_display_dispatch(display);
    else                wl_display_dispatch_pending(display);

    if (!eventFIFO.isEmpty()) return eventFIFO.pop();
    //return {};
    return {EventType::NONE};
}

Window_wayland::~Window_wayland() {
    if (toplevel)   xdg_toplevel_destroy(toplevel);
    if (xdg_surf)   xdg_surface_destroy(xdg_surf);
    if (surface)    wl_surface_destroy(surface);
    if (xdg_wm)     xdg_wm_base_destroy(xdg_wm);
    if (compositor) wl_compositor_destroy(compositor);
    if (registry)   wl_registry_destroy(registry);
    if (display)    wl_display_disconnect(display);
};

#endif // GWINDOW_IMPLEMENTATION

#endif  // VK_USE_PLATFORM_WAYLAND_KHR
//==============================================================