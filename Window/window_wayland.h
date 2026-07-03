//==========================WAYLAND=============================
#ifdef VK_USE_PLATFORM_WAYLAND_KHR

#ifndef WINDOW_WAYLAND
#define WINDOW_WAYLAND

#include "WindowBase.h"
#include <wayland-client.h>
#include "wayland/xdg-decoration.h"
#include "wayland/xdg-shell-protocol.h"

#include <sys/mman.h>
#include <unistd.h>
#include <fcntl.h>
#include <cstdlib>
#include <assert.h>
#include <poll.h>

struct native_handle {
    wl_display* display;
    wl_surface* surface;
};

//==========================WAYLAND=============================
class Window_wayland : public WindowBase {
public:
    wl_display*    display    = nullptr;
    wl_surface*    surface    = nullptr;
    wl_registry*   registry   = nullptr;
    wl_compositor* compositor = nullptr;
    wl_shm*        shm        = nullptr;
    xdg_wm_base*   xdg_wm     = nullptr;
    xdg_surface*   xdg_surf   = nullptr;
    xdg_toplevel*  toplevel   = nullptr;

    // TEMPORARY: solid-color SHM buffer, used only until Vulkan swapchain presentation is wired up.
    // Once main.cpp creates a VkSurfaceKHR + swapchain for this window, vkQueuePresentKHR will
    // attach/commit swapchain images itself, and this test buffer can be removed.
    wl_buffer* test_buffer = nullptr;

    // --- xdg_toplevel events ---
    static void on_toplevel_configure(void* data, xdg_toplevel*,  int32_t w, int32_t h, wl_array* states) {
        if (w > 0 && h > 0) {
            auto* self = static_cast<Window_wayland*>(data);

           if(w != self->shape.width || h != self->shape.height) {
               self->eventFIFO.push(self->resizeEvent(w,h));
            }

            bool activated = false;
            auto* state = static_cast<uint32_t*>(states->data);
            auto* end = reinterpret_cast<uint32_t*>(static_cast<char*>(states->data) + states->size);
            for (; state != end; ++state) {
                if (*state == XDG_TOPLEVEL_STATE_ACTIVATED) { activated = true; break; }
            }
            if (activated != self->has_focus) self->eventFIFO.push(self->focusEvent(activated));
        }
    }
    static void on_toplevel_close(void* data, xdg_toplevel*) {
        auto* self = static_cast<Window_wayland*>(data);
        self->eventFIFO.push(self->closeEvent());
    }
    static constexpr xdg_toplevel_listener toplevel_listener = { on_toplevel_configure, on_toplevel_close };

    wl_buffer* MakeTestBuffer(int w, int h);  // TEMPORARY

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
//#define GWINDOW_IMPLEMENTATION
#ifdef GWINDOW_IMPLEMENTATION

//======================WAYLAND CALLBACKS=======================
    // --- Registry listener ---
    static void on_global(void* data, wl_registry* reg, uint32_t name, const char* iface, uint32_t version) {
        auto* w = static_cast<Window_wayland*>(data);
        if (strcmp(iface, wl_compositor_interface.name) == 0)
            w->compositor = (wl_compositor*)wl_registry_bind(reg, name, &wl_compositor_interface, 4);
        else if (strcmp(iface, xdg_wm_base_interface.name) == 0)
            w->xdg_wm = (xdg_wm_base*)wl_registry_bind(reg, name, &xdg_wm_base_interface, 1);
        else if (strcmp(iface, wl_shm_interface.name) == 0)
            w->shm = (wl_shm*)wl_registry_bind(reg, name, &wl_shm_interface, 1);
    }
    static void on_global_remove(void*, wl_registry*, uint32_t) {}
    static constexpr wl_registry_listener registry_listener = { on_global, on_global_remove };

    // --- xdg_wm_base ping/pong (required or compositor kills the client) ---
    static void on_ping(void*, xdg_wm_base* base, uint32_t serial) {xdg_wm_base_pong(base, serial);}
    static constexpr xdg_wm_base_listener wm_base_listener = { on_ping };

    // --- xdg_surface configure (must ack or nothing appears) ---
    static void on_xdg_surface_configure(void* data, xdg_surface* surf, uint32_t serial) {
        //printf("configure received\n");
        auto* w = static_cast<Window_wayland*>(data);
        xdg_surface_ack_configure(surf, serial);

        // TEMPORARY: attach a solid-color test buffer so the window becomes visible.
        // Remove this once Vulkan swapchain presentation attaches real frames instead.
        if (!w->test_buffer) w->test_buffer = w->MakeTestBuffer(w->shape.width, w->shape.height);
        if (w->test_buffer) {
            wl_surface_attach(w->surface, w->test_buffer, 0, 0);
            wl_surface_damage(w->surface, 0, 0, w->shape.width, w->shape.height);
        }
        wl_surface_commit(w->surface);
        //wl_display_flush(w->display);
    }
    static constexpr xdg_surface_listener surface_listener = { on_xdg_surface_configure };

//==============================================================


Window_wayland::Window_wayland(const char* title, uint width, uint height) {
    Create(title, width, height);
}

// TEMPORARY: allocates a shared-memory buffer filled with a solid color, just so the
// surface has *something* to display. A Wayland wl_surface stays unmapped (invisible)
// until a wl_buffer is attached and committed -- ack_configure + commit alone is not
// enough, unlike X11/Win32 where a blank window appears automatically.
// Delete this function once real frames are presented via the Vulkan swapchain.
wl_buffer* Window_wayland::MakeTestBuffer(int w, int h) {
    if (!shm || w <= 0 || h <= 0) return nullptr;
    int stride = w * 4;
    int size   = stride * h;

    char path[] = "/tmp/wl_shm-XXXXXX";
    int fd = mkstemp(path);
    if (fd < 0) return nullptr;
    ::unlink(path);
    if (::ftruncate(fd, size) < 0) { ::close(fd); return nullptr; }

    void* data = mmap(nullptr, size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (data == MAP_FAILED) { ::close(fd); return nullptr; }

    uint32_t* pixels = (uint32_t*)data;
    for (int i = 0; i < w * h; ++i) pixels[i] = 0xFF3060C0;  // solid ARGB (opaque blue-ish)
    munmap(data, size);

    wl_shm_pool* pool   = wl_shm_create_pool(shm, fd, size);
    wl_buffer*   buffer = wl_shm_pool_create_buffer(pool, 0, w, h, stride, WL_SHM_FORMAT_ARGB8888);
    wl_shm_pool_destroy(pool);
    ::close(fd);
    return buffer;
}

void Window_wayland::Create(const char* title, uint width, uint height) {
    shape.width  = width;
    shape.height = height;
    running      = true;

    printf("Creating Wayland-Window...\n");

    display  = wl_display_connect(nullptr);
    if (!display) { printf("ERROR: wl_display_connect failed (no Wayland session?)\n"); return; }

    registry = wl_display_get_registry(display);
    wl_registry_add_listener(registry, &registry_listener, this);
    wl_display_roundtrip(display);  // populates compositor + xdg_wm + shm

    if (!compositor) printf("ERROR: wl_compositor not found\n");
    if (!xdg_wm)     printf("ERROR: xdg_wm_base not found\n");
    if (!shm)        printf("ERROR: wl_shm not found\n");

    xdg_wm_base_add_listener(xdg_wm, &wm_base_listener, this);

    surface  = wl_compositor_create_surface(compositor);            assert(surface);
    xdg_surf = xdg_wm_base_get_xdg_surface(xdg_wm, surface);        assert(xdg_surf);
    xdg_surface_add_listener(xdg_surf, &surface_listener, this);

    toplevel = xdg_surface_get_toplevel(xdg_surf);                  assert(toplevel);
    xdg_toplevel_add_listener(toplevel, &toplevel_listener, this);
    xdg_toplevel_set_title(toplevel, title);

    wl_surface_commit(surface);
    //wl_display_flush(display);

    //wl_display_roundtrip(display);  // triggers the initial configure -> on_xdg_surface_configure -> buffer attach
    int ret = wl_display_roundtrip(display);
    printf("roundtrip returned %d\n", ret);

    int err = wl_display_get_error(display);
    if (err){printf("Wayland error: %d\n", err);}

    eventFIFO.push(resizeEvent(width, height));
}


EventType Window_wayland::getEvent(bool wait_for_event) {
    if (!eventFIFO.isEmpty()) return eventFIFO.pop();

    if (wait_for_event) wl_display_dispatch(display);
    //else                wl_display_dispatch_pending(display);
    else {
        // Non-blocking: only read from the socket if data is actually available.
        while (wl_display_prepare_read(display) != 0)
            wl_display_dispatch_pending(display);   // drain anything already queued first

        wl_display_flush(display);                  // send any pending outgoing requests (e.g. pong)
        pollfd pfd = { wl_display_get_fd(display), POLLIN, 0 };
        if (poll(&pfd, 1, 0) > 0) wl_display_read_events(display);  // new data available -> read it
        else                      wl_display_cancel_read(display);  // nothing to read, cancel cleanly
        wl_display_dispatch_pending(display);                       // dispatch whatever we just read
    }



    if (!eventFIFO.isEmpty()) return eventFIFO.pop();
    return {EventType::NONE};
}

Window_wayland::~Window_wayland() {
    if (test_buffer) wl_buffer_destroy(test_buffer);  // TEMPORARY: remove alongside MakeTestBuffer
    if (toplevel)   xdg_toplevel_destroy(toplevel);
    if (xdg_surf)   xdg_surface_destroy(xdg_surf);
    if (surface)    wl_surface_destroy(surface);
    if (xdg_wm)     xdg_wm_base_destroy(xdg_wm);
    if (compositor) wl_compositor_destroy(compositor);
    if (shm)        wl_shm_destroy(shm);
    if (registry)   wl_registry_destroy(registry);
    if (display)    wl_display_disconnect(display);
}

#endif // GWINDOW_IMPLEMENTATION

#endif  // VK_USE_PLATFORM_WAYLAND_KHR
//==============================================================
