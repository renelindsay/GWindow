//==========================WAYLAND=============================
#ifdef VK_USE_PLATFORM_WAYLAND_KHR

#ifndef WINDOW_WAYLAND
#define WINDOW_WAYLAND

//#define ENABLE_MULTITOUCH
//#define ENABLE_GAMEPAD
//#define ENABLE_CLIPBOARD
#define ENABLE_SHOWIMAGE
#define ENABLE_CURSOR
//#define ENABLE_FULLSCREEN

#include "WindowBase.h"
#include <wayland-client.h>
#include <wayland-egl.h>
#include <wayland-cursor.h>
#include <libdecor.h>
#include <xkbcommon/xkbcommon.h>

#include <sys/mman.h>
#include <unistd.h>
#include <fcntl.h>
#include <cstdlib>
#include <cassert>

struct native_handle {
    wl_display* display;
    wl_surface* surface;
    wl_egl_window* egl_window;
};

//==========================WAYLAND=============================
class Window_wayland : public WindowBase {
public:
    wl_display*    display    = nullptr;
    wl_surface*    surface    = nullptr;
    wl_egl_window* egl_window = nullptr;

    wl_registry*   registry   = nullptr;
    wl_compositor* compositor = nullptr;
    wl_shm*        shm        = nullptr;

    // libdecor owns the xdg_surface / xdg_toplevel internally now.
    libdecor*       decor_context = nullptr;
    libdecor_frame* decor_frame   = nullptr;
    bool            has_configured = false;  // set true after the first libdecor configure+commit

    // --- Input (wl_seat) ---
    wl_seat*     seat     = nullptr;
    wl_keyboard* keyboard = nullptr;
    wl_pointer*  pointer  = nullptr;

    xkb_context* xkb_ctx        = nullptr;  // xkbcommon context (created once)
    xkb_keymap*  xkb_keymap_ptr = nullptr;  // (re)created from compositor-supplied keymap
    xkb_state*   xkb_state_ptr  = nullptr;  // tracks modifier state

#ifdef ENABLE_CURSOR
    wl_cursor_theme* cursor_theme        = nullptr;
    wl_surface*      cursor_surface      = nullptr;
    wl_cursor*       cursors[11]         = {};  // indexed by eCursor
    uint32_t         pointer_enter_serial = 0;  // serial from the most recent wl_pointer::enter
    void LoadCursorTheme();
#endif

    void Create(const char* title="Window", uint width=640, uint height=480);
public:
    void setTitle(const char* title) { libdecor_frame_set_title(decor_frame, title); }
    void setPosition(uint x, uint y) {}  // No Wayland protocol equivalent -- permanent no-op.
    void setSize(uint w, uint h);

    Window_wayland() {Create();}
    Window_wayland(const char* title, uint width, uint height);
    virtual ~Window_wayland();
    EventType getEvent(bool wait_for_event = false);
    native_handle* getNativeHandle() const {return (native_handle*)&display;}

#ifdef ENABLE_SHOWIMAGE
    void showImage(uint32_t* buf, uint32_t width, uint32_t height);
#endif

#ifdef ENABLE_CURSOR
    void setCursor(eCursor id);
#endif
};
//==============================================================
#endif  // WINDOW_WAYLAND
//#define GWINDOW_IMPLEMENTATION
#ifdef GWINDOW_IMPLEMENTATION

// Convert native EVDEV key-code to cross-platform USB HID code.
// (Same table used by the XCB backend; duplicated here since the two backends
//  are never compiled together and window_xcb.h guards its copy behind VK_USE_PLATFORM_XCB_KHR.)
// clang-format off
static const unsigned char WAYLAND_EVDEV_TO_HID[256] = {
  0,  0,  0,  0,  0,  0,  0,  0,  0, 41, 30, 31, 32, 33, 34, 35,
 36, 37, 38, 39, 45, 46, 42, 43, 20, 26,  8, 21, 23, 28, 24, 12,
 18, 19, 47, 48, 40,224,  4, 22,  7,  9, 10, 11, 13, 14, 15, 51,
 52, 53,225, 49, 29, 27,  6, 25,  5, 17, 16, 54, 55, 56,229, 85,
226, 44, 57, 58, 59, 60, 61, 62, 63, 64, 65, 66, 67, 83, 71, 95,
 96, 97, 86, 92, 93, 94, 87, 89, 90, 91, 98, 99,  0,  0,100, 68,
 69,  0,  0,  0,  0,  0,  0,  0, 88,228, 84, 70,230,  0, 74, 82,
 75, 80, 79, 77, 81, 78, 73, 76,  0,127,128,129,  0,103,  0, 72,
  0,  0,  0,  0,  0,227,231,118,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,104,
105,106,107,108,109,110,111,112,113,114,115,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0
};
// clang-format on

//======================WAYLAND CALLBACKS=======================
    // --- Registry listener --- (libdecor binds xdg_wm_base itself; we bind compositor + shm + seat)
    static void seat_capabilities(void* data, wl_seat* seat, uint32_t caps);
    static void seat_name(void*, wl_seat*, const char*) {}
    static constexpr wl_seat_listener seat_listener = { seat_capabilities, seat_name };

    static void on_global(void* data, wl_registry* reg, uint32_t name, const char* iface, uint32_t version) {
        auto* w = static_cast<Window_wayland*>(data);
        if (strcmp(iface, wl_compositor_interface.name) == 0)
            w->compositor = (wl_compositor*)wl_registry_bind(reg, name, &wl_compositor_interface, 4);
        else if (strcmp(iface, wl_shm_interface.name) == 0)
            w->shm = (wl_shm*)wl_registry_bind(reg, name, &wl_shm_interface, 1);
        else if (strcmp(iface, wl_seat_interface.name) == 0) {
            w->seat = (wl_seat*)wl_registry_bind(reg, name, &wl_seat_interface, 5);
            wl_seat_add_listener(w->seat, &seat_listener, w);
        }
    }

    static void on_global_remove(void*, wl_registry*, uint32_t) {}
    static constexpr wl_registry_listener registry_listener = { on_global, on_global_remove };

    // --- wl_buffer release ---
    static void buffer_release(void* data, wl_buffer* buffer) {
        //auto* w = static_cast<Window_wayland*>(data);
        wl_buffer_destroy(buffer);
    }
    static constexpr wl_buffer_listener buffer_listener = { buffer_release };

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
        if (width > 0 && height > 0 && size_changed) {
            if (w->egl_window) wl_egl_window_resize(w->egl_window, width, height, 0, 0);
            w->eventFIFO.push(w->resizeEvent(width, height));
        }

        // Focus (and other window state) no longer comes from a raw xdg_toplevel states
        // array -- libdecor exposes it as a bitmask via libdecor_configuration_get_window_state().
        libdecor_window_state window_state = LIBDECOR_WINDOW_STATE_NONE;
        if (!libdecor_configuration_get_window_state(configuration, &window_state))
            window_state = LIBDECOR_WINDOW_STATE_NONE;  // no state info yet (can happen on first configure)

        bool active = (window_state & LIBDECOR_WINDOW_STATE_ACTIVE) != 0;
        if (active != w->has_focus) w->eventFIFO.push(w->focusEvent(active));

        libdecor_state* state = libdecor_state_new(width, height);
        libdecor_frame_commit(frame, state, configuration);
        libdecor_state_free(state);
        wl_surface_commit(w->surface);

        w->has_configured = true;
    }

    static void decor_frame_close(libdecor_frame*, void* data) {
        auto* w = static_cast<Window_wayland*>(data);
        w->eventFIFO.push(w->closeEvent());
    }

    // Called by libdecor, to flush decoration-only changes.
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

    // --- wl_keyboard listener ---
    static void keyboard_keymap(void* data, wl_keyboard*, uint32_t format, int32_t fd, uint32_t size) {
        auto* w = static_cast<Window_wayland*>(data);
        if (format != WL_KEYBOARD_KEYMAP_FORMAT_XKB_V1) { ::close(fd); return; }

        char* map_str = (char*)mmap(nullptr, size, PROT_READ, MAP_PRIVATE, fd, 0);
        ::close(fd);
        if (map_str == MAP_FAILED) return;

        if (w->xkb_state_ptr)  { xkb_state_unref(w->xkb_state_ptr);   w->xkb_state_ptr  = nullptr; }
        if (w->xkb_keymap_ptr) { xkb_keymap_unref(w->xkb_keymap_ptr); w->xkb_keymap_ptr = nullptr; }

        w->xkb_keymap_ptr = xkb_keymap_new_from_string(w->xkb_ctx, map_str,
                                XKB_KEYMAP_FORMAT_TEXT_V1, XKB_KEYMAP_COMPILE_NO_FLAGS);
        munmap(map_str, size);
        if (w->xkb_keymap_ptr) w->xkb_state_ptr = xkb_state_new(w->xkb_keymap_ptr);
    }

    static void keyboard_enter(void* data, wl_keyboard*, uint32_t, wl_surface*, wl_array*) {
        auto* w = static_cast<Window_wayland*>(data);
        if (!w->has_focus) w->eventFIFO.push(w->focusEvent(true));
    }

    static void keyboard_leave(void* data, wl_keyboard*, uint32_t, wl_surface*) {
        auto* w = static_cast<Window_wayland*>(data);
        if (w->has_focus) w->eventFIFO.push(w->focusEvent(false));
    }

    static void keyboard_key(void* data, wl_keyboard*, uint32_t, uint32_t, uint32_t key, uint32_t state) {
        auto* w = static_cast<Window_wayland*>(data);
        uint32_t xkb_code = key + 8;  // wl keycode is raw evdev; xkbcommon/X11 convention adds 8
        uint8_t hidcode = (xkb_code < 256) ? WAYLAND_EVDEV_TO_HID[xkb_code] : 0;
        bool down = (state == WL_KEYBOARD_KEY_STATE_PRESSED);
        if (hidcode) w->eventFIFO.push(w->keyEvent(down ? eDOWN : eUP, hidcode));

        if (down && w->xkb_state_ptr) {
            static char buf[8] = {};  // static: textEvent() only stores the pointer, not a copy
            int n = xkb_state_key_get_utf8(w->xkb_state_ptr, xkb_code, buf, sizeof(buf));
            if (n > 0) w->eventFIFO.push(w->textEvent(buf));
        }
    }

    static void keyboard_modifiers(void* data, wl_keyboard*, uint32_t, uint32_t depressed,
                                    uint32_t latched, uint32_t locked, uint32_t group) {
        auto* w = static_cast<Window_wayland*>(data);
        if (w->xkb_state_ptr)
            xkb_state_update_mask(w->xkb_state_ptr, depressed, latched, locked, 0, 0, group);
    }

    static void keyboard_repeat_info(void*, wl_keyboard*, int32_t, int32_t) {}

    static constexpr wl_keyboard_listener keyboard_listener = {
        keyboard_keymap, keyboard_enter, keyboard_leave, keyboard_key,
        keyboard_modifiers, keyboard_repeat_info
    };

    // --- wl_pointer listener ---
    // Wayland button codes are raw Linux input-event-codes (BTN_LEFT=0x110, BTN_RIGHT=0x111, BTN_MIDDLE=0x112).
    #define GW_BTN_LEFT   0x110
    #define GW_BTN_RIGHT  0x111
    #define GW_BTN_MIDDLE 0x112

    static void pointer_enter(void* data, wl_pointer*, uint32_t serial, wl_surface* entered_surface, wl_fixed_t sx, wl_fixed_t sy) {
        auto* w = static_cast<Window_wayland*>(data);
        w->eventFIFO.push(w->mouseEvent(eMOVE, (int16_t)wl_fixed_to_int(sx), (int16_t)wl_fixed_to_int(sy), 0));
#ifdef ENABLE_CURSOR
        if (entered_surface == w->surface) {
            w->pointer_enter_serial = serial;
            w->setCursor(eArrow);  // Set mouse cursor icon when mouse enters content surface.
        }
#endif
    }

    static void pointer_leave(void*, wl_pointer*, uint32_t, wl_surface*) {}

    static void pointer_motion(void* data, wl_pointer*, uint32_t, wl_fixed_t sx, wl_fixed_t sy) {
        auto* w = static_cast<Window_wayland*>(data);
        uint8_t bestBtn = w->getBtnState(1) ? 1 : w->getBtnState(2) ? 2 : w->getBtnState(3) ? 3 : 0;
        w->eventFIFO.push(w->mouseEvent(eMOVE, (int16_t)wl_fixed_to_int(sx), (int16_t)wl_fixed_to_int(sy), bestBtn));
    }

    static void pointer_button(void* data, wl_pointer*, uint32_t, uint32_t, uint32_t button, uint32_t state) {
        auto* w = static_cast<Window_wayland*>(data);
        uint8_t btn = (button == GW_BTN_LEFT) ? 1 : (button == GW_BTN_RIGHT) ? 2 : (button == GW_BTN_MIDDLE) ? 3 : 0;
        if (!btn) return;
        bool down = (state == WL_POINTER_BUTTON_STATE_PRESSED);
        w->eventFIFO.push(w->mouseEvent(down ? eDOWN : eUP, w->mouse.pos.x, w->mouse.pos.y, btn));
    }

    static void pointer_axis(void* data, wl_pointer*, uint32_t, uint32_t axis, wl_fixed_t value) {
        auto* w = static_cast<Window_wayland*>(data);
        if (axis != WL_POINTER_AXIS_VERTICAL_SCROLL) return;
        uint8_t wheel = (wl_fixed_to_double(value) < 0) ? 4 : 5;
        w->eventFIFO.push(w->mouseEvent(eDOWN, w->mouse.pos.x, w->mouse.pos.y, wheel));
    }

    static void pointer_frame(void*, wl_pointer*) {}
    static void pointer_axis_source(void*, wl_pointer*, uint32_t) {}
    static void pointer_axis_stop(void*, wl_pointer*, uint32_t, uint32_t) {}
    static void pointer_axis_discrete(void*, wl_pointer*, uint32_t, int32_t) {}

    static constexpr wl_pointer_listener pointer_listener = {
        pointer_enter, pointer_leave, pointer_motion, pointer_button, pointer_axis,
        pointer_frame, pointer_axis_source, pointer_axis_stop, pointer_axis_discrete
    };

    // --- wl_seat capabilities: attach/detach keyboard + pointer as they come and go ---
    static void seat_capabilities(void* data, wl_seat* seat, uint32_t caps) {
        auto* w = static_cast<Window_wayland*>(data);
        bool has_kbd = caps & WL_SEAT_CAPABILITY_KEYBOARD;
        bool has_ptr = caps & WL_SEAT_CAPABILITY_POINTER;

        if (has_kbd && !w->keyboard) {
            w->keyboard = wl_seat_get_keyboard(seat);
            wl_keyboard_add_listener(w->keyboard, &keyboard_listener, w);
        } else if (!has_kbd && w->keyboard) {
            wl_keyboard_release(w->keyboard);
            w->keyboard = nullptr;
        }

        if (has_ptr && !w->pointer) {
            w->pointer = wl_seat_get_pointer(seat);
            wl_pointer_add_listener(w->pointer, &pointer_listener, w);
        } else if (!has_ptr && w->pointer) {
            wl_pointer_release(w->pointer);
            w->pointer = nullptr;
        }
    }

//==============================================================

Window_wayland::Window_wayland(const char* title, uint width, uint height) {
    Create(title, width, height);
}

void Window_wayland::setSize(uint w, uint h) {
    if ((int)w == shape.width && (int)h == shape.height) return;
    if (egl_window) wl_egl_window_resize(egl_window, w, h, 0, 0);

    // Update libdecor with new content size
    libdecor_state* state = libdecor_state_new(w, h);
    libdecor_frame_commit(decor_frame, state, nullptr);
    libdecor_state_free(state);
    wl_surface_commit(surface);

    eventFIFO.push(resizeEvent(w, h));
}

#ifdef ENABLE_SHOWIMAGE
void Window_wayland::showImage(uint32_t* buf, uint32_t width, uint32_t height) {
    int sw = shape.width;
    int sh = shape.height;
    int stride = sw*4;
    int size = stride*sh;

    char path[] = "/tmp/wl_shm-XXXXXX";
    int fd = mkstemp(path);  if (fd < 0) return;
    ::unlink(path);
    if (::ftruncate(fd, size) < 0) { ::close(fd); return; }
    void* data = mmap(nullptr, size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (data == MAP_FAILED) { ::close(fd); return; }
    uint32_t* pixels = (uint32_t*)data;
    //for (int i=0; i<sw*sh; ++i) pixels[i] = 0xFF000000;

    uint32_t w = std::min((uint32_t)shape.width,  width );
    uint32_t h = std::min((uint32_t)shape.height, height);

    for (int y=0; y<h; ++y) {
       uint32_t* src = buf + (width*y);
       uint32_t* dst = pixels + (sw*y);
       //memcpy(dst, src, w*4);
       for (int x=0; x<w; ++x) dst[x] = src[x] | 0xFF000000;
       for (int x=w; x<sw; ++x) dst[x] = 0xFF000000;
    }
    for (int y=h; y<sh; ++y) {
        uint32_t* dst = pixels + (sw*y);
        for (int x=0; x<sw; ++x) dst[x] = 0xFF000000;
    }

    munmap(data, size);
    wl_shm_pool* pool   = wl_shm_create_pool(shm, fd, size);
    wl_buffer*   buffer = wl_shm_pool_create_buffer(pool, 0, sw, sh, stride, WL_SHM_FORMAT_ARGB8888);
    wl_shm_pool_destroy(pool);
    ::close(fd);
    if (!buffer) return;

    wl_buffer_add_listener(buffer, &buffer_listener, this);

    wl_surface_attach(surface, buffer, 0, 0);
    wl_surface_damage(surface, 0, 0, sw, sh);
    wl_surface_commit(surface);
}
#endif

#ifdef ENABLE_CURSOR
void Window_wayland::LoadCursorTheme() {
    cursor_theme = wl_cursor_theme_load(nullptr, 24, shm);
    if (!cursor_theme) { printf("WARNING: wl_cursor_theme_load failed -- no cursor icons\n"); return; }
    cursors[eCursor::eArrow]      = wl_cursor_theme_get_cursor(cursor_theme, "left_ptr");
    cursors[eCursor::eCaret]      = wl_cursor_theme_get_cursor(cursor_theme, "xterm");
    cursors[eCursor::eResizeAll]  = wl_cursor_theme_get_cursor(cursor_theme, "fleur");
    cursors[eCursor::eResizeNS]   = wl_cursor_theme_get_cursor(cursor_theme, "sb_v_double_arrow");
    cursors[eCursor::eResizeEW]   = wl_cursor_theme_get_cursor(cursor_theme, "sb_h_double_arrow");
    cursors[eCursor::eResizeNESW] = wl_cursor_theme_get_cursor(cursor_theme, "top_right_corner");
    cursors[eCursor::eResizeNWSE] = wl_cursor_theme_get_cursor(cursor_theme, "top_left_corner");
    cursors[eCursor::eHand]       = wl_cursor_theme_get_cursor(cursor_theme, "hand2");
    cursors[eCursor::eWait]       = wl_cursor_theme_get_cursor(cursor_theme, "wait");
    cursors[eCursor::eProgress]   = wl_cursor_theme_get_cursor(cursor_theme, "progress");
    cursors[eCursor::eNotAllowed] = wl_cursor_theme_get_cursor(cursor_theme, "circle");
    cursor_surface = wl_compositor_create_surface(compositor);
}

void Window_wayland::setCursor(eCursor id) {
    wl_cursor* cursor = cursors[id];
    wl_cursor_image* image  = cursor->images[0];
    uint             count  = cursor->image_count;  assert(count>0);
    wl_buffer*       buffer = wl_cursor_image_get_buffer(image);
    wl_pointer_set_cursor(pointer, pointer_enter_serial, cursor_surface, image->hotspot_x, image->hotspot_y);
    wl_surface_attach(cursor_surface, buffer, 0, 0);
    wl_surface_damage(cursor_surface, 0, 0, image->width, image->height);
    wl_surface_commit(cursor_surface);
}
#endif  // ENABLE_CURSOR

//NOTE: Wayland window does not appear until after you attach an image to it.
void Window_wayland::Create(const char* title, uint width, uint height) {
    shape.width  = width;
    shape.height = height;
    running      = true;

    printf("Creating Wayland-Window...\n");

    display = wl_display_connect(nullptr);
    if (!display) { printf("ERROR: wl_display_connect failed (no Wayland session?)\n"); return; }

    xkb_ctx = xkb_context_new(XKB_CONTEXT_NO_FLAGS);

    registry = wl_display_get_registry(display);
    wl_registry_add_listener(registry, &registry_listener, this);
    wl_display_roundtrip(display);  // populates compositor + shm + seat

    if (!compositor) { printf("ERROR: wl_compositor not found\n"); return; }
    if (!shm)        { printf("ERROR: wl_shm not found\n");        return; }
    if (!seat)       printf("WARNING: wl_seat not found -- no keyboard/mouse input\n");

#ifdef ENABLE_CURSOR
    LoadCursorTheme();
#endif

    surface = wl_compositor_create_surface(compositor);
    if (!surface) { printf("ERROR: wl_compositor_create_surface failed\n"); return; }
    egl_window = wl_egl_window_create(surface, width, height);

    decor_context = libdecor_new(display, const_cast<libdecor_interface*>(&decor_iface));
    if (!decor_context) { printf("ERROR: libdecor_new failed\n"); return; }

    decor_frame = libdecor_decorate(decor_context, surface,
                                     const_cast<libdecor_frame_interface*>(&decor_frame_iface), this);
    if (!decor_frame) { printf("ERROR: libdecor_decorate failed\n"); return; }

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
    libdecor_dispatch(decor_context, wait_for_event ? -1 : 0);
    return eventFIFO.pop();
}

Window_wayland::~Window_wayland() {
    if (xkb_state_ptr)  xkb_state_unref(xkb_state_ptr);
    if (xkb_keymap_ptr) xkb_keymap_unref(xkb_keymap_ptr);
    if (xkb_ctx)        xkb_context_unref(xkb_ctx);

    if (keyboard)      wl_keyboard_release(keyboard);
    if (pointer)       wl_pointer_release(pointer);
    if (seat)          wl_seat_release(seat);

#ifdef ENABLE_CURSOR
    if (cursor_surface) wl_surface_destroy(cursor_surface);
    if (cursor_theme)   wl_cursor_theme_destroy(cursor_theme);
#endif

    if (decor_frame)   libdecor_frame_unref(decor_frame);
    if (egl_window)    wl_egl_window_destroy(egl_window);
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
