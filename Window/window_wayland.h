//==========================WAYLAND=============================
#ifdef VK_USE_PLATFORM_WAYLAND_KHR

#ifndef WINDOW_WAYLAND
#define WINDOW_WAYLAND

#include "WindowBase.h"
#include <wayland-client.h>
#include <libdecor.h>

#include <sys/mman.h>
#include <unistd.h>
#include <fcntl.h>
#include <cstdlib>
#include <cassert>

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

    // libdecor owns the xdg_surface / xdg_toplevel internally now.
    libdecor*       decor_context = nullptr;
    libdecor_frame* decor_frame   = nullptr;
    bool            has_configured = false;  // set true after the first libdecor configure+commit

    // TEMPORARY: solid-color SHM buffer, used only until Vulkan swapchain presentation is wired up.
    // Once main.cpp creates a VkSurfaceKHR + swapchain for this window, vkQueuePresentKHR will
    // attach/commit swapchain images itself, and this test buffer can be removed.
    wl_buffer* test_buffer = nullptr;

    wl_buffer* MakeTestBuffer(int w, int h);  // TEMPORARY: see note above test_buffer

    void Create(const char* title="Window", uint width=640, uint height=480);
public:
    void setTitle(const char* title) { libdecor_frame_set_title(decor_frame, title); }
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
    // --- Registry listener --- (libdecor binds xdg_wm_base itself; we only need compositor + shm)
    static void on_global(void* data, wl_registry* reg, uint32_t name, const char* iface, uint32_t version) {
        auto* w = static_cast<Window_wayland*>(data);
        if (strcmp(iface, wl_compositor_interface.name) == 0)
            w->compositor = (wl_compositor*)wl_registry_bind(reg, name, &wl_compositor_interface, 4);
        else if (strcmp(iface, wl_shm_interface.name) == 0)
            w->shm = (wl_shm*)wl_registry_bind(reg, name, &wl_shm_interface, 1);
    }
    static void on_global_remove(void*, wl_registry*, uint32_t) {}
    static constexpr wl_registry_listener registry_listener = { on_global, on_global_remove };

    // --- libdecor top-level errors ---
    static void decor_error(libdecor* context, libdecor_error error, const char* message) {
        printf("libdecor error (%d): %s\n", (int)error, message);
    }
    static constexpr libdecor_interface decor_iface = { decor_error };

    // --- libdecor frame callbacks ---
    static void decor_frame_configure(libdecor_frame* frame, libdecor_configuration* configuration, void* data) {
        auto* w = static_cast<Window_wayland*>(data);

        int width = 0, height = 0;
        if (!libdecor_configuration_get_content_size(configuration, frame, &width, &height)) {
            // Compositor didn't suggest a size (e.g. first configure) -- keep current/default size.
            width  = w->shape.width  ? w->shape.width  : 640;
            height = w->shape.height ? w->shape.height : 480;
        }

        bool size_changed = (width != w->shape.width || height != w->shape.height);
        if (width > 0 && height > 0 && size_changed)
            w->eventFIFO.push(w->resizeEvent(width, height));

        // Focus (and other window state) no longer comes from a raw xdg_toplevel states
        // array -- libdecor exposes it as a bitmask via libdecor_configuration_get_window_state().
        libdecor_window_state window_state = LIBDECOR_WINDOW_STATE_NONE;
        if (!libdecor_configuration_get_window_state(configuration, &window_state))
            window_state = LIBDECOR_WINDOW_STATE_NONE;  // no state info yet (can happen on first configure)

        bool active = (window_state & LIBDECOR_WINDOW_STATE_ACTIVE) != 0;
        if (active != w->has_focus) w->eventFIFO.push(w->focusEvent(active));

        // TEMPORARY: (re)build the solid-color test buffer to match the configured size.
        // Remove this block once Vulkan swapchain presentation attaches real frames instead.
        if (!w->test_buffer || size_changed) {
            if (w->test_buffer) { wl_buffer_destroy(w->test_buffer); w->test_buffer = nullptr; }
            w->test_buffer = w->MakeTestBuffer(width, height);
        }
        if (w->test_buffer) {
            wl_surface_attach(w->surface, w->test_buffer, 0, 0);
            wl_surface_damage(w->surface, 0, 0, width, height);
        }

        libdecor_state* state = libdecor_state_new(width, height);
        libdecor_frame_commit(frame, state, configuration);
        libdecor_state_free(state);

        // libdecor_frame_commit() only commits libdecor's own bookkeeping (ack_configure,
        // window geometry, its decoration subsurfaces) -- it does NOT commit our content
        // surface. We must flush the attach+damage above ourselves, or nothing ever appears.
        wl_surface_commit(w->surface);

        w->has_configured = true;
    }

    static void decor_frame_close(libdecor_frame*, void* data) {
        auto* w = static_cast<Window_wayland*>(data);
        w->eventFIFO.push(w->closeEvent());
    }

    // libdecor asks us to commit the underlying wl_surface when it changes the decoration
    // (e.g. resizing the decoration buffer). Just commit -- the content buffer is already attached.
    static void decor_frame_commit(libdecor_frame*, void* data) {
        auto* w = static_cast<Window_wayland*>(data);
        wl_surface_commit(w->surface);
    }

    static void decor_frame_dismiss_popup(libdecor_frame*, const char*, void*) {}

    static constexpr libdecor_frame_interface decor_frame_iface = {
        decor_frame_configure,
        decor_frame_close,
        decor_frame_commit,
        decor_frame_dismiss_popup,
    };

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

    display = wl_display_connect(nullptr);
    if (!display) { printf("ERROR: wl_display_connect failed (no Wayland session?)\n"); return; }

    registry = wl_display_get_registry(display);
    wl_registry_add_listener(registry, &registry_listener, this);
    wl_display_roundtrip(display);  // populates compositor + shm

    if (!compositor) printf("ERROR: wl_compositor not found\n");
    if (!shm)        printf("ERROR: wl_shm not found\n");

    surface = wl_compositor_create_surface(compositor);
    assert(surface);

    decor_context = libdecor_new(display, const_cast<libdecor_interface*>(&decor_iface));
    assert(decor_context);

    decor_frame = libdecor_decorate(decor_context, surface,
                                     const_cast<libdecor_frame_interface*>(&decor_frame_iface), this);
    assert(decor_frame);

    libdecor_frame_set_title(decor_frame, title);
    libdecor_frame_set_app_id(decor_frame, title);
    libdecor_frame_map(decor_frame);  // triggers the first configure

    // Block until the first configure+commit has happened (mirrors the old double-roundtrip).
    while (!has_configured) {
        if (libdecor_dispatch(decor_context, -1) < 0) {
            printf("ERROR: libdecor_dispatch failed during initial configure\n");
            break;
        }
    }

    eventFIFO.push(resizeEvent(shape.width, shape.height));
}


EventType Window_wayland::getEvent(bool wait_for_event) {
    if (!eventFIFO.isEmpty()) return eventFIFO.pop();

    // libdecor_dispatch wraps wl_display's fd *and* libdecor's own internal fd(s),
    // so this replaces plain wl_display_dispatch()/dispatch_pending() entirely.
    libdecor_dispatch(decor_context, wait_for_event ? -1 : 0);

    if (!eventFIFO.isEmpty()) return eventFIFO.pop();
    return {EventType::NONE};
}

Window_wayland::~Window_wayland() {
    if (test_buffer)   wl_buffer_destroy(test_buffer);  // TEMPORARY: remove alongside MakeTestBuffer
    if (decor_frame)   libdecor_frame_unref(decor_frame);
    if (surface)       wl_surface_destroy(surface);
    if (decor_context) libdecor_unref(decor_context);
    if (compositor)    wl_compositor_destroy(compositor);
    if (shm)           wl_shm_destroy(shm);
    if (registry)      wl_registry_destroy(registry);
    if (display)       wl_display_disconnect(display);
}

#endif // GWINDOW_IMPLEMENTATION

#endif  // VK_USE_PLATFORM_WAYLAND_KHR
//==============================================================
