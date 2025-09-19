//#define VK_USE_PLATFORM_XCB_KHR
//#define GWINDOW_IMPLEMENTATION

//============================XCB===============================
#ifdef VK_USE_PLATFORM_XCB_KHR

#ifndef WINDOW_XCB
#define WINDOW_XCB

//#define ENABLE_MULTITOUCH  // requires libxi-dev
//#define ENABLE_GAMEPAD     // requires libevdev-dev (8kb)
//#define ENABLE_CLIPBOARD   // requires libxcb-icccm4-dev + libxcb1-dev
//#define ENABLE_SHOWIMAGE   // requires libxcb-image0-dev + libxcb1-dev
//#define ENABLE_CURSOR      // requires libxcb-cursor-dev + libxcb1-dev + libxcb-cursor0
//#define ENABLE_FULLSCREEN  // requires libxcb1-dev

//-------------------------------------------------
#include "WindowBase.h"
//#include <xcb/xcb.h>            // XCB only
//#include <X11/Xlib.h>           // XLib only
#include <X11/Xlib-xcb.h>         // Xlib + XCB
#include <xkbcommon/xkbcommon.h>  // Keyboard   libxkbcommon-dev
#include <X11/Xresource.h>        // DPI scale
#include <stdlib.h>               // atof
#include <assert.h>
#ifdef ENABLE_SHOWIMAGE
#include <xcb/xcb_image.h>        // showImage  libxcb-image0-dev
#endif
#ifdef ENABLE_CURSOR
#include <xcb/xcb_cursor.h>       // mouse cursor icons
#endif
#ifdef ENABLE_GAMEPAD
#include <libevdev/libevdev.h>    // libevdev-dev
#include <fcntl.h>                // gamepad open
#include <unistd.h>               // gamepad read
#include <sys/inotify.h>          // gamepad inotify
#include <dirent.h>               // For scanning the /dev/input/ directory
#include <sys/ioctl.h>
#include <linux/input.h>
#include "gamepads.h"
#endif
#ifdef ENABLE_FULLSCREEN
#include <xcb/xcb.h>
#endif
#ifdef ENABLE_CLIPBOARD
#include <xcb/xcb_icccm.h>
#endif
//-------------------------------------------------

#ifdef ENABLE_MULTITOUCH
#include <X11/extensions/XInput2.h>  // MultiTouch
typedef uint16_t xcb_input_device_id_t;
typedef uint32_t xcb_input_fp1616_t;
// clang-format off
typedef struct xcb_input_touch_begin_event_t {  // from xinput.h in XCB 1.12 (current version is 1.11)
    uint8_t                   response_type;
    uint8_t                   extension;
    uint16_t                  sequence;
    uint32_t                  length;
    uint16_t                  event_type;
    xcb_input_device_id_t     deviceid;
    xcb_timestamp_t           time;
    uint32_t                  detail;
    xcb_window_t              root;
    xcb_window_t              event;
    xcb_window_t              child;
    uint32_t                  full_sequence;
    xcb_input_fp1616_t        root_x;
    xcb_input_fp1616_t        root_y;
    xcb_input_fp1616_t        event_x;
    xcb_input_fp1616_t        event_y;
    uint16_t                  buttons_len;
    uint16_t                  valuators_len;
    xcb_input_device_id_t     sourceid;
    // uint8_t                   pad0[2];
    // uint32_t                  flags;
    // xcb_input_modifier_info_t mods;
    // xcb_input_group_info_t    group;
} xcb_input_touch_begin_event_t;
#endif

// clang-format off
// Convert native EVDEV key-code to cross-platform USB HID code.
const unsigned char EVDEV_TO_HID[256] = {
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
//=============================XCB==============================
class Window_xcb : public WindowBase {
    Display* display;                  // for XLib
    xcb_connection_t* xcb_connection;  // for XCB
    xcb_screen_t* xcb_screen;
    xcb_window_t xcb_window;
    xcb_atom_t atom_wm_delete_window = XCB_NONE;
    //----xcb_image ----
    xcb_gcontext_t gc=0;
    xcb_pixmap_t   pixmap=0;
    //------------------
    //---xkb Keyboard---
    xkb_context* k_ctx;  // context for xkbcommon keyboard input
    xkb_keymap* k_keymap;
    xkb_state* k_state;
    //------------------
    //---Touch Device---
    CMTouch MTouch;
    int xi_opcode;  // 131
    int xi_devid;   // 2
    //------------------
    //----- Cursor -----
#ifdef ENABLE_CURSOR
    xcb_cursor_context_t *cursor_ctx;
    xcb_cursor_t cursors[12];
#endif
    //------------------
    //---- Gamepad ----
#ifdef ENABLE_GAMEPAD
#define MAX_BTNS 16
#define MAX_AXIS 16

    int inotify_fd = -1;          // gamepad inotify descriptor
    int watch_fd   = -1;          // gamepad watch descriptor
    struct Evdev {                // gamepad handle and axis ranges
        int fd = -1;
        libevdev* dev = nullptr;
        char name[256] = {};      // gamepad model name
        char path[256] = {};      // eg. /dev/input/event240

        struct Btns{
            uint16_t BTN;         // BTN event code
            int8_t  eBTN;         // eGamepadBtn
        }b[MAX_BTNS]={};          // buttons

        struct Axes{
            uint16_t AXIS;        // ABS event code
            int8_t  eAXIS;        // eGamepadAxis
            int  min =0;          // Axis range min value
            int  max =0;          // Axis range max value
            int  fuzz=0;          // Noise level
            int  flat=0;          // Dead zone
            bool flip=false;      // Flip this axis
            int  prev=0;          // previous value
        }a[MAX_AXIS]={};          // axes

    } evdev[MAX_GAMEPADS];

    void DetectGamepads();                                   // Detect connected gamepads
    bool ConnectGamepad(const char* path);                   // eg. /dev/input/event240
    void DisconnectGamepad(uint8_t id);                      // Disconnect gamepad by id (0-3)
    void MapGamepad(uint8_t id);                             // Map gamepad btn/axis layout
    void SetGamepadLEDs(uint8_t id, uint8_t state);          // pad-id(0-3), led-bitmask(0-15)
    void ReadGamepadEvents();                                // Process all gamepad events
    void GamepadBtnEvent(uint8_t id, input_event event);     // Button events
    void GamepadAxisEvent(uint8_t id, input_event event);    // Axis events
#endif
    //------------------

    bool InitTouch();                                        // Returns false if no touch-device was found.
    EventType TranslateEvent(xcb_generic_event_t* x_event);  // Convert x_event to Window event
    void Create(const char* title="Window", uint width=640, uint height=480);
    xcb_atom_t GetAtom(const char* name, bool only_if_exists = false);

  public:
    void setTitle(const char* title);
    void setPosition(uint x, uint y);
    void setSize(uint w, uint h);
    //void CreateSurface(VkInstance instance);

    Window_xcb() {Create();}
    Window_xcb(const char* title, uint width, uint height);
    virtual ~Window_xcb();
    EventType getEvent(bool wait_for_event = false);
    //bool CanPresent(VkPhysicalDevice phy, uint32_t queue_family);  // check if this window can present this queue type
    const void* getNativeHandle() const {return &xcb_connection;}
    float getDisplayScale();
#ifdef ENABLE_SHOWIMAGE
    void showImage(uint32_t* buf, uint32_t width, uint32_t height);
#endif
    void setCursor(eCursor id);
#ifdef ENABLE_FULLSCREEN
    void setFullscreen(bool enable);
    bool isFullscreen();
#endif

#ifdef ENABLE_CLIPBOARD
private:
    xcb_atom_t atom_CLIPBOARD;   // "CLIPBOARD"
    xcb_atom_t atom_UTF8_STRING; // "UTF8_STRING"
    xcb_atom_t atom_TARGETS;     // "TARGETS"
    xcb_atom_t atom_PROPERTY;    // any name, often "XSEL_DATA"
    xcb_atom_t atom_STRING;      // fallback

    void InitClipboard();
    bool RequestClipboard();
public:
    virtual const char* getClipboardText();
    virtual void setClipboardText(const char* str);
#else
    void InitClipboard(){};
#endif



};
//==============================================================
#endif

#ifdef GWINDOW_IMPLEMENTATION

//=======================XCB IMPLEMENTATION=====================

Window_xcb::Window_xcb(const char* title, uint width, uint height) {
    Create(title, width, height);
}

xcb_atom_t Window_xcb::GetAtom(const char* name, bool only_if_exists) {
    xcb_intern_atom_cookie_t cookie = xcb_intern_atom(xcb_connection, only_if_exists, strlen(name), name);
    xcb_intern_atom_reply_t* reply = xcb_intern_atom_reply(xcb_connection, cookie, nullptr);
    if (!reply) return XCB_NONE;
    xcb_atom_t atom = reply->atom;
    free(reply);
    return atom;
}

void Window_xcb::Create(const char* title, uint width, uint height) {
    shape.width  = width;
    shape.height = height;
    running      = true;

    //printf("Creating XCB-Window...\n");

    // --Init Connection-- XCB only
    // int scr;
    // xcb_connection = xcb_connect(NULL, &scr);
    // assert(xcb_connection && "XCB failed to connect to the X server.");
    // const xcb_setup_t*   setup = xcb_get_setup(xcb_connection);
    // xcb_screen_iterator_t iter = xcb_setup_roots_iterator(setup);
    // while(scr-- > 0) xcb_screen_next(&iter);
    // xcb_screen = iter.data;
    //-------------------

    //----XLib + XCB----
    XInitThreads(); // Required by Vulkan, when using XLib. (Vulkan spec section: 30.2.6 Xlib Platform)
    display = XOpenDisplay(NULL);                 assert(display && "Failed to open Display");        // for XLIB functions
    xcb_connection = XGetXCBConnection(display);  assert(display && "Failed to open XCB connection"); // for XCB functions
    const xcb_setup_t* setup = xcb_get_setup(xcb_connection);
    setup = xcb_get_setup(xcb_connection);
    xcb_screen = (xcb_setup_roots_iterator(setup)).data;
    XSetEventQueueOwner(display, XCBOwnsEventQueue);
    //------------------

    uint32_t value_mask = XCB_CW_BACK_PIXEL | XCB_CW_EVENT_MASK;
    uint32_t value_list[2];
    value_list[0] = xcb_screen->black_pixel;
    value_list[1] = XCB_EVENT_MASK_KEY_PRESS |          // 1
                    XCB_EVENT_MASK_KEY_RELEASE |        // 2
                    XCB_EVENT_MASK_BUTTON_PRESS |       // 4
                    XCB_EVENT_MASK_BUTTON_RELEASE |     // 8
                    XCB_EVENT_MASK_POINTER_MOTION |     // 64       motion with no mouse button held
                    XCB_EVENT_MASK_BUTTON_MOTION  |     // 8192     motion with one or more mouse buttons held
                  //XCB_EVENT_MASK_KEYMAP_STATE |       // 16384
                  //XCB_EVENT_MASK_EXPOSURE |           // 32768    Make showImage persistant
                  //XCB_EVENT_MASK_VISIBILITY_CHANGE,   // 65536,
                    XCB_EVENT_MASK_STRUCTURE_NOTIFY |   // 131072   Window move/resize events
                  //XCB_EVENT_MASK_RESIZE_REDIRECT |    // 262144
                    XCB_EVENT_MASK_FOCUS_CHANGE;        // 2097152  Window focus

#ifdef ENABLE_SHOWIMAGE
    gc     = xcb_generate_id(xcb_connection);
    pixmap = xcb_generate_id(xcb_connection);
#endif

    xcb_window = xcb_generate_id(xcb_connection);
    xcb_create_window(xcb_connection, XCB_COPY_FROM_PARENT, xcb_window, xcb_screen->root, 0, 0, width, height, 0,
                      XCB_WINDOW_CLASS_INPUT_OUTPUT, xcb_screen->root_visual, value_mask, value_list);

    xcb_atom_t wm_protocols     = GetAtom("WM_PROTOCOLS", true);
    xcb_atom_t wm_delete_window = GetAtom("WM_DELETE_WINDOW");
    xcb_change_property(xcb_connection, XCB_PROP_MODE_REPLACE, xcb_window, wm_protocols, 4, 32, 1, &wm_delete_window);
    atom_wm_delete_window = wm_delete_window;

    //---Keyboard input---
    k_ctx = xkb_context_new(XKB_CONTEXT_NO_FLAGS);
    // xkb_rule_names names = {NULL,"pc105","is","dvorak","terminate:ctrl_alt_bksp"};
    // keymap = xkb_keymap_new_from_names(k_ctx, &names,XKB_KEYMAP_COMPILE_NO_FLAGS);
    k_keymap = xkb_keymap_new_from_names(k_ctx, NULL, XKB_KEYMAP_COMPILE_NO_FLAGS);  // use current keyboard settings
    k_state  = xkb_state_new(k_keymap);
    //--------------------
    InitTouch();  
    InitClipboard();
    //--------------------

    setTitle(title);
    eventFIFO.push(resizeEvent(width, height));  // resizeEvent BEFORE focus, for consistency with win32 and android

    //---- Mouse Cursor ----
#ifdef ENABLE_CURSOR
    xcb_cursor_context_new(xcb_connection, xcb_setup_roots_iterator(setup).data, &cursor_ctx);
    cursors[eCursor::eArrow]      = xcb_cursor_load_cursor(cursor_ctx, "left_ptr");
    cursors[eCursor::eCaret]      = xcb_cursor_load_cursor(cursor_ctx, "xterm");
    cursors[eCursor::eResizeAll]  = xcb_cursor_load_cursor(cursor_ctx, "fleur");
    cursors[eCursor::eResizeNS]   = xcb_cursor_load_cursor(cursor_ctx, "sb_v_double_arrow");
    cursors[eCursor::eResizeEW]   = xcb_cursor_load_cursor(cursor_ctx, "sb_h_double_arrow");
    cursors[eCursor::eResizeNESW] = xcb_cursor_load_cursor(cursor_ctx, "top_right_corner");
    cursors[eCursor::eResizeNWSE] = xcb_cursor_load_cursor(cursor_ctx, "top_left_corner");
    cursors[eCursor::eHand]       = xcb_cursor_load_cursor(cursor_ctx, "hand2");
    cursors[eCursor::eWait]       = xcb_cursor_load_cursor(cursor_ctx, "wait");
    cursors[eCursor::eProgress]   = xcb_cursor_load_cursor(cursor_ctx, "progress");
    cursors[eCursor::eNotAllowed] = xcb_cursor_load_cursor(cursor_ctx, "circle");
#endif
    //----------------------

    //----Map the window----
    xcb_map_window(xcb_connection, xcb_window);
    xcb_flush(xcb_connection);

    // Wait for the window to be mapped (so resize works correctly)
    xcb_generic_event_t *event;
    while ((event = xcb_wait_for_event(xcb_connection))) {
        bool mapped = ((event->response_type & ~0x80) == XCB_MAP_NOTIFY);
        free(event);
        if(mapped) break;
    }
    //---------------------- 
}

Window_xcb::~Window_xcb() {

#ifdef ENABLE_SHOWIMAGE
    if(gc)     xcb_free_gc    (xcb_connection, gc);
    if(pixmap) xcb_free_pixmap(xcb_connection, pixmap);
#endif
#ifdef ENABLE_CURSOR
    int cnt = sizeof(cursors) / sizeof(cursors[0]);
    for(int i=0; i<cnt; ++i) xcb_free_cursor(xcb_connection, cursors[i]);
    xcb_cursor_context_free(cursor_ctx);
#endif
#ifdef ENABLE_GAMEPAD
    for (int i = 0; i < MAX_GAMEPADS; ++i) { DisconnectGamepad(i); }
    if (watch_fd   != -1) { inotify_rm_watch(inotify_fd, watch_fd); watch_fd=-1;}
    if (inotify_fd != -1) { ::close(inotify_fd); inotify_fd=-1; }
#endif
    xcb_disconnect(xcb_connection);
    XFree(k_ctx);  // xkb keyboard
}

void Window_xcb::setTitle(const char* title) {
    xcb_change_property(xcb_connection, XCB_PROP_MODE_REPLACE, xcb_window, XCB_ATOM_WM_NAME,  // set window title
                        XCB_ATOM_STRING, 8, strlen(title), title);
    xcb_change_property(xcb_connection, XCB_PROP_MODE_REPLACE, xcb_window, XCB_ATOM_WM_ICON_NAME,  // set icon title
                        XCB_ATOM_STRING, 8, strlen(title), title);
    xcb_map_window(xcb_connection, xcb_window);
    xcb_flush(xcb_connection);
}

void Window_xcb::setPosition(uint x, uint y) {
    uint values[] = {x, y};
    xcb_configure_window(xcb_connection, xcb_window, XCB_CONFIG_WINDOW_X | XCB_CONFIG_WINDOW_Y, values);
    xcb_flush(xcb_connection);
}

void Window_xcb::setSize(uint w, uint h) {
    uint values[] = {w, h};
    xcb_configure_window(xcb_connection, xcb_window, XCB_CONFIG_WINDOW_WIDTH | XCB_CONFIG_WINDOW_HEIGHT, values);
    xcb_flush(xcb_connection);
}
/*
void Window_xcb::CreateSurface(VkInstance instance) {
    if (surface) return;
    this->instance = instance;
    VkXcbSurfaceCreateInfoKHR xcb_createInfo;
    xcb_createInfo.sType      = VK_STRUCTURE_TYPE_XCB_SURFACE_CREATE_INFO_KHR;
    xcb_createInfo.pNext      = NULL;
    xcb_createInfo.flags      = 0;
    xcb_createInfo.connection = xcb_connection;
    xcb_createInfo.window     = xcb_window;
    VKERRCHECK(vkCreateXcbSurfaceKHR(instance, &xcb_createInfo, NULL, &surface));
    LOGI("Vulkan Surface created\n");
}
*/
//---------------------------------------------------------------------------
bool Window_xcb::InitTouch() {
#ifdef ENABLE_MULTITOUCH
    int ev, err;
    if (!XQueryExtension(display, "XInputExtension", &xi_opcode, &ev, &err)) {
        printf("WARNING: XInputExtension not available.\n");
        return false;
    }

    // check the version of XInput
    int major = 2;
    int minor = 3;
    if (XIQueryVersion(display, &major, &minor) != Success) {
        printf("WARNING: No XI2 support. (%d.%d only)\n", major, minor);
        return false;
    }

    {  // select device
        int cnt;
        XIDeviceInfo* di = XIQueryDevice(display, XIAllDevices, &cnt);
        for (int i = 0; i < cnt; ++i) {
            XIDeviceInfo* dev = &di[i];
            for (int j = 0; j < dev->num_classes; ++j) {
                XITouchClassInfo* tcinfo = (XITouchClassInfo*)(dev->classes[j]);
                if (tcinfo->type != XITouchClass) {
                    xi_devid = dev->deviceid;
                    goto endloop;
                }
            }
        }
    endloop:
        XIFreeDeviceInfo(di);
    }

    {  // select which events to listen to
        unsigned char buf[3] = {};
        XIEventMask mask     = {};
        mask.deviceid        = xi_devid;
        mask.mask_len        = XIMaskLen(XI_TouchEnd);
        mask.mask            = buf;
        XISetMask(mask.mask, XI_TouchBegin);
        XISetMask(mask.mask, XI_TouchUpdate);
        XISetMask(mask.mask, XI_TouchEnd);
        XISelectEvents(display, xcb_window, &mask, 1);
    }
    return true;
#else
    return false;
#endif
}
//---------------------------------------------------------------------------

EventType Window_xcb::TranslateEvent(xcb_generic_event_t* x_event) {
    static char buf[4] = {};                                            // store char for text event
    xcb_button_press_event_t& e = *(xcb_button_press_event_t*)x_event;  // xcb_motion_notify_event_t
    int16_t mx = e.event_x;
    int16_t my = e.event_y;
    uint8_t btn= e.detail;
    uint8_t bestBtn = getBtnState(1) ? 1 : getBtnState(2) ? 2 : getBtnState(3) ? 3 : 0;  // If multiple buttons pressed, pick left one.
    switch(x_event->response_type & ~0x80) {
        case XCB_MOTION_NOTIFY : return mouseEvent(eMOVE, mx, my, bestBtn); // mouse move
        case XCB_BUTTON_PRESS  : return mouseEvent(eDOWN, mx, my, btn);     // mouse btn press
        case XCB_BUTTON_RELEASE: return mouseEvent(eUP  , mx, my, btn);     // mouse btn release
        case XCB_KEY_PRESS:{
            //printf("btn %d", btn);
            uint8_t keycode = EVDEV_TO_HID[btn];                    // On Stratus XL gamepad, 2 buttons trigger keyboard events
            if(!keycode) {                                          // remap key to gamepad btn
                if(btn==166) return gpadButton(0, eBTN_SELECT, 1);  // Steelseries Stratus XL: select button (XF86Back)
                if(btn==180) return gpadButton(0, eBTN_MODE, 1);    // Steelseries Stratus XL: mode button   (XF86HomePage)
            }
            xkb_state_key_get_utf8(k_state,btn,buf,sizeof(buf));
            xkb_state_update_key(k_state,btn,XKB_KEY_DOWN);
            if(buf[0]) eventFIFO.push(textEvent(buf));              // text typed event (store in FIFO for next run)
            return keyEvent(eDOWN, keycode);                        // key pressed event
        }
        case XCB_KEY_RELEASE: {
            xkb_state_update_key(k_state, btn, XKB_KEY_UP);
            uint8_t keycode = EVDEV_TO_HID[btn];
            if(!keycode) {                                          // remap key to gamepad btn
                if(btn==166) return gpadButton(0, eBTN_SELECT, 0);  // Steelseries Stratus XL
                if(btn==180) return gpadButton(0, eBTN_MODE, 0);    // Steelseries Stratus XL
            }
            return keyEvent(eUP, keycode);                          // key released event
        }
        case XCB_CLIENT_MESSAGE: {                                  // window close event
            if ((*(xcb_client_message_event_t*)x_event).data.data32[0] == atom_wm_delete_window) {
                //printf("Closing Window\n");
                return closeEvent();
            }
            break;
        }
        case XCB_CONFIGURE_NOTIFY: {                                // Window Reshape (move or resize)
            auto& e = *(xcb_configure_notify_event_t*)x_event;
            //bool se = (e.response_type & 128);                    // True if message was sent with "SendEvent"
            if (e.width != shape.width || e.height != shape.height) return resizeEvent(e.width, e.height); // window resized
            else if (e.x != shape.x || e.y != shape.y)              return moveEvent(e.x, e.y);            // window moved
            break;
        }
        case XCB_FOCUS_IN  : if (!has_focus) return focusEvent(true);   // window gained focus
        case XCB_FOCUS_OUT : if ( has_focus) return focusEvent(false);  // window lost focus

        case XCB_GE_GENERIC: {                                            // Multi touch screen events
#ifdef ENABLE_MULTITOUCH
            xcb_input_touch_begin_event_t& te = *(xcb_input_touch_begin_event_t*)x_event;
            if(te.extension == xi_opcode) {  // check if this event is from the touch device
                float x = te.event_x / 65536.f;
                float y = te.event_y / 65536.f;
                uint id = te.detail;

                switch(te.event_type){
                    case XI_TouchBegin : return MTouch.Event_by_ID(eDOWN, x, y,  0, id); // touch down event
                    case XI_TouchUpdate: return MTouch.Event_by_ID(eMOVE, x, y, id, id); // touch move event
                    case XI_TouchEnd   : return MTouch.Event_by_ID(eUP  , x, y, id,  0); // touch up event
                    default : break;
                }
            }
#endif
            return {EventType::UNKNOWN};
        }  // XCB_GE_GENERIC

#ifdef ENABLE_SHOWIMAGE
        case XCB_EXPOSE: {  // for showImage
             xcb_expose_event_t& e = *(xcb_expose_event_t*)x_event;
             xcb_copy_area(xcb_connection,pixmap,xcb_window,gc,e.x,e.y,e.x,e.y,e.width,e.height);
             xcb_flush(xcb_connection);
        }break;
#endif

#ifdef ENABLE_CLIPBOARD
        case XCB_SELECTION_NOTIFY: {  // get clipboard text
            xcb_selection_notify_event_t* sel = (xcb_selection_notify_event_t*)x_event;
            if (sel->property == XCB_NONE) { clipboard = ""; break; }

            xcb_get_property_cookie_t prop_cookie =
                xcb_get_property(xcb_connection, 0, xcb_window, atom_PROPERTY,
                                          XCB_GET_PROPERTY_TYPE_ANY, 0, 1024);
            xcb_get_property_reply_t* prop_reply =
                xcb_get_property_reply(xcb_connection, prop_cookie, nullptr);
            if (prop_reply) {
                int len = xcb_get_property_value_length(prop_reply);
                const char* val = (const char*)xcb_get_property_value(prop_reply);
                clipboard.assign(val, len);
                free(prop_reply);
            }
        }break;

        case XCB_SELECTION_REQUEST: {
            xcb_selection_request_event_t* req = (xcb_selection_request_event_t*)x_event;

            xcb_selection_notify_event_t notify = {};
            notify.response_type = XCB_SELECTION_NOTIFY;
            notify.sequence  = 0;
            notify.time      = req->time;
            notify.requestor = req->requestor;
            notify.selection = req->selection;
            notify.target    = req->target;
            notify.property  = req->property;

            if (req->target == atom_TARGETS) {
                xcb_atom_t targets[] = { atom_UTF8_STRING, atom_STRING, atom_TARGETS };
                xcb_change_property(
                    xcb_connection,
                    XCB_PROP_MODE_REPLACE,
                    req->requestor,
                    req->property,
                    XCB_ATOM_ATOM,
                    32,
                    sizeof(targets) / sizeof(targets[0]),
                    targets
                );
            } else
            if (req->target == atom_UTF8_STRING || req->target == atom_STRING) {
                xcb_atom_t type = (req->target == atom_UTF8_STRING) ? atom_UTF8_STRING : atom_STRING;
                xcb_change_property(
                    xcb_connection,
                    XCB_PROP_MODE_REPLACE,
                    req->requestor,
                    req->property,
                    type,
                    8,  // 8 bits per char
                    clipboard.size(),
                    clipboard.c_str()
                );
            } else {
                notify.property = XCB_NONE;  // Unsupported target
            }

            xcb_send_event(xcb_connection, false, req->requestor, XCB_EVENT_MASK_NO_EVENT, (const char*)&notify);
            xcb_flush(xcb_connection);
        }break;
#endif
        default:
            // printf("EVENT: %d\n",(x_event->response_type & ~0x80));  //get event numerical value
            break;
    }  // switch
    return {EventType::NONE};
}

EventType Window_xcb::getEvent(bool wait_for_event) {
#ifdef ENABLE_GAMEPAD
    ReadGamepadEvents();
#endif
    if (!eventFIFO.isEmpty()) return eventFIFO.pop();  // pop message from message queue buffer
    xcb_generic_event_t* x_event;
    if (wait_for_event) x_event = xcb_wait_for_event(xcb_connection);  // Blocking mode
    else                x_event = xcb_poll_for_event(xcb_connection);  // Non-blocking mode
    while(x_event) {
        EventType event = TranslateEvent(x_event);
        XFree(x_event);
        if (event.tag == EventType::UNKNOWN) {
            x_event = xcb_poll_for_event(xcb_connection);  // Discard unknown events (Intel Mesa drivers spams event 35)
        } else return event;
    }
    return {EventType::NONE};
}

float Window_xcb::getDisplayScale() {
    float dpi = 0.f;
    XrmValue value;
    char *type = NULL;
    char *resourceString = XResourceManagerString(display);
    XrmInitialize();
    XrmDatabase db = XrmGetStringDatabase(resourceString);
    if (resourceString) {
        //printf("Entire DB:\n%s\n", resourceString);
        if (XrmGetResource(db, "Xft.dpi", "String", &type, &value) == True) {
            if (value.addr) {
                dpi = atof(value.addr);
            }
        }
    }
    //printf("Monitor DPI: %f\n", dpi);
    //display_scale = dpi / 96.f;
    return dpi / 96.f;
}

#ifdef ENABLE_SHOWIMAGE
void Window_xcb::showImage(uint32_t* buf, uint32_t width, uint32_t height) {  // Shows image for 1 frame.
    xcb_connection_t* c = xcb_connection;
    xcb_image_format_t format = XCB_IMAGE_FORMAT_Z_PIXMAP;
    int depth  = xcb_screen->root_depth;

    xcb_create_pixmap(c,depth,pixmap,xcb_window,width,height);
    xcb_create_gc(c,gc,pixmap,0,NULL);

    xcb_image_t* image = xcb_image_create_native(c,width,height,format,depth,0,0,(uint8_t*)buf);
    xcb_image_put(c, pixmap, gc, image, 0, 0, 0);
    xcb_image_destroy(image);
    xcb_copy_area(c,pixmap,xcb_window,gc,0,0,0,0,width,height);
    xcb_flush(xcb_connection);

    xcb_free_gc(c, gc);
    xcb_free_pixmap(c, pixmap);
}
#endif


void Window_xcb::setCursor(eCursor id) {
#ifdef ENABLE_CURSOR
    xcb_change_window_attributes(xcb_connection, xcb_window, XCB_CW_CURSOR, &cursors[id]);
#endif
}

#ifdef ENABLE_FULLSCREEN
void Window_xcb::setFullscreen(bool enable) {
    xcb_atom_t a_wm_state = GetAtom("_NET_WM_STATE");
    xcb_atom_t a_fullscreen = GetAtom("_NET_WM_STATE_FULLSCREEN");

    if (a_wm_state == XCB_NONE || a_fullscreen == XCB_NONE) return;

    xcb_client_message_event_t ev = {};
    ev.response_type = XCB_CLIENT_MESSAGE;
    ev.window = xcb_window;
    ev.type = a_wm_state;
    ev.format = 32;
    ev.data.data32[0] = enable ? 1 : 0;  // _NET_WM_STATE_ADD : _NET_WM_STATE_REMOVE
    ev.data.data32[1] = a_fullscreen;
    ev.data.data32[2] = 0;
    ev.data.data32[3] = 1;  // Normal source indication (application)
    ev.data.data32[4] = 0;

    xcb_send_event(xcb_connection, 0,
                   xcb_setup_roots_iterator(xcb_get_setup(xcb_connection)).data->root,
                   XCB_EVENT_MASK_SUBSTRUCTURE_REDIRECT | XCB_EVENT_MASK_SUBSTRUCTURE_NOTIFY,
                   reinterpret_cast<const char*>(&ev));
    xcb_flush(xcb_connection);
}

bool Window_xcb::isFullscreen() {
    xcb_atom_t net_wm_state = GetAtom("_NET_WM_STATE");
    xcb_atom_t fs_atom = GetAtom("_NET_WM_STATE_FULLSCREEN");

    xcb_get_property_cookie_t prop_cookie = xcb_get_property(
        xcb_connection,
        0,
        xcb_window,
        net_wm_state,
        XCB_ATOM_ATOM,
        0,  // offset
        1024  // length
    );

    xcb_get_property_reply_t* prop_reply = xcb_get_property_reply(xcb_connection, prop_cookie, nullptr);
    if (!prop_reply) return false;

    xcb_atom_t* atoms = (xcb_atom_t*)xcb_get_property_value(prop_reply);
    int len = xcb_get_property_value_length(prop_reply) / sizeof(xcb_atom_t);

    bool is_fs = false;
    for (int i = 0; i < len; ++i) {
        if (atoms[i] == fs_atom) {
            is_fs = true;
            break;
        }
    }

    free(prop_reply);
    return is_fs;
}
#endif

//---Gamepad---
#ifdef ENABLE_GAMEPAD
void Window_xcb::DetectGamepads() {
    if (inotify_fd == -1) {  // inotify not started yet
        // On first run, scan for already connected gamepads
        DIR* dir = opendir("/dev/input/");
        if(dir) {
            dirent* entry;
            while ((entry = readdir(dir))) {
                if (strncmp(entry->d_name, "event", 5) == 0) { // Look for "event#"
                    char path[512]{};
                    snprintf(path, sizeof(path), "/dev/input/%s", entry->d_name);
                    ConnectGamepad(path);
                }
            }
        }
        // Watch for new device connections using inotify
        inotify_fd = inotify_init1(IN_NONBLOCK);
        if (inotify_fd < 0) { perror("inotify_init1"); return; }
        watch_fd = inotify_add_watch(inotify_fd, "/dev/input/", IN_CREATE);
    }
    // Process inotify events
    char buffer[1024];
    int len = read(inotify_fd, buffer, sizeof(buffer));
    if (len > 0) {
        for (char* ptr = buffer; ptr < buffer + len;) {
            struct inotify_event* event = (struct inotify_event*)ptr;
            ptr += sizeof(struct inotify_event) + event->len;
            if (event->mask & (IN_CREATE)) {
                if (strncmp(event->name, "event", 5) == 0) {  // Look for "eventX"
                    char path[256]{};
                    snprintf(path, sizeof(path), "/dev/input/%s", event->name);
                    usleep(60000);         // Allow time for the device to appear
                    ConnectGamepad(path);  // Try to connect as Gamepad
                }
            }
        }
    }
}

void setGamepadLED(const char* devicePath, int LED_id, bool state) {
    int fd = open(devicePath, O_WRONLY);
    if(fd<0) return;
    struct input_event event;
    event.type = EV_LED;
    event.code = LED_id;
    event.value = state ? 1 : 0;
    ssize_t s=write(fd, &event, sizeof(event));
    close(fd);
}

void Window_xcb::SetGamepadLEDs(uint8_t id, uint8_t state) {
    setGamepadLED(evdev[id].path, 0, !!(state&1));
    setGamepadLED(evdev[id].path, 1, !!(state&2));
    setGamepadLED(evdev[id].path, 2, !!(state&4));
    setGamepadLED(evdev[id].path, 3, !!(state&8));
}

bool Window_xcb::ConnectGamepad(const char* path) {
    int fd = open(path, O_RDONLY | O_NONBLOCK);
    if(fd<0) return false;
    libevdev* dev = nullptr;
    if (libevdev_new_from_fd(fd, &dev) >= 0) {
        if(libevdev_has_event_type(dev, EV_ABS)
        && libevdev_has_event_type(dev, EV_KEY)
        && libevdev_has_event_code(dev, EV_KEY, BTN_SOUTH)
        && libevdev_has_event_code(dev, EV_KEY, BTN_NORTH)
        && libevdev_has_event_code(dev, EV_KEY, BTN_EAST )
        && libevdev_has_event_code(dev, EV_KEY, BTN_WEST )
        && libevdev_has_event_code(dev, EV_ABS, ABS_X)
        && libevdev_has_event_code(dev, EV_ABS, ABS_Y)) {  // make sure this is a gamepad
            for (int i = 0; i < MAX_GAMEPADS; ++i) {       // find a free slot
                Evdev& ev = evdev[i];
                if(ev.fd < 0) {
                    ev.fd = fd;
                    ev.dev= dev;
                    strncpy(ev.name, libevdev_get_name(dev), sizeof(ev.name)-1);  // query gamepad name
                    strncpy(ev.path, path, sizeof(ev.path)-1);                    // get gamepad event file path
                    //printf("Gamepad %d found: %s at %s\n", i, ev.name, path);
                    MapGamepad(i);           // Detect gamepad button layout
                    SetGamepadLEDs(i,1<<i);  // Set Gamepad LEDs to indicate which slot its in.
                    eventFIFO.push(gpadConnect(i, true));
                    return true;
                }
            }
        } else {libevdev_free(dev); ::close(fd);}
    } else ::close(fd);
    return false;
}

void Window_xcb::DisconnectGamepad(uint8_t id) {
    Evdev& ev = evdev[id];
    if(ev.fd==-1) return;
    eventFIFO.push(gpadConnect(id, false));
    //SetGamepadLEDs(id, 0);  // Does not restore blinking :(
    libevdev_free(ev.dev);
    ::close(ev.fd);
    memset(&ev, 0, sizeof(ev));
    ev.fd = -1;
    ev.dev = 0;
    ev.name[0] = '\0';
    ev.path[0] = '\0';
    //printf("Gamepad %d disconnected.\n", id);
}

void Window_xcb::MapGamepad(uint8_t id) {
    Evdev& pad = evdev[id];
    auto dev = pad.dev;
    const char* name = libevdev_get_name(dev);
    uint BUS=libevdev_get_id_bustype(dev);
    uint VID=libevdev_get_id_vendor(dev);
    uint PID=libevdev_get_id_product(dev);
    printf("Gamepad %d: \"%s\"\n",id , name);
    //printf("bus:%#x vendor:%#x product:%#x\n",bus, VID, PID);
    //-----------------------------------------------------------------
    int hatx_inx=0;  // HAT0X axis-index
    int haty_inx=0;  // HAT0Y axis-index
    int axis_cnt=0;  // not used

    // get the button event code list
    for (int code=0,b=0; code<KEY_MAX && b<MAX_BTNS; code++)
        if(libevdev_has_event_code(dev, EV_KEY, code)) pad.b[b++].BTN=code;

    // get axis event code and limits
    for (int code=0,a=0; code<KEY_MAX && a<MAX_AXIS; code++) {
        if(libevdev_has_event_code(dev, EV_ABS, code)) {
            if(code==ABS_HAT0X) hatx_inx=a;  // save the hatx index for later
            if(code==ABS_HAT0Y) haty_inx=a;  // save the haty index for later
            auto& axis = pad.a[a++];
            axis.AXIS = code;
            axis.min  = libevdev_get_abs_minimum(dev, code);  // axis min value
            axis.max  = libevdev_get_abs_maximum(dev, code);  // axis max value
            axis.fuzz = libevdev_get_abs_fuzz   (dev, code);  // noise level
            axis.flat = libevdev_get_abs_flat   (dev, code);  // dead zone
        }
        axis_cnt=a;
    }
    //for(auto& line : gamepad_layouts){for(uint8_t val : line) printf("%02X ", val); printf("\n");}

    auto* p_layout = get_gamepad_layout(VID, PID, BUS, name);  // Find layout by gamepad ID

    if(p_layout) {  // Gamepad found. Decode layout tokens.
        int8_t eBTN [] = {1,2,3,4,5,6,7,8,9,10,11,12,-1,-2,-3,-4,-5,-6,13,14};  // pos-to-eBTN
        int8_t eAXIS[] = {0,0,0,0,0,0,0,0,0, 0, 0, 0, 1, 2, 3, 4, 5, 6, 0, 0};  // pos-to-eAxis
        auto layout = *p_layout;
        for(int i=0; i<layout.size(); ++i) {
            uint8_t code = layout[i];
            //printf("%02x ",code);
            if(code==0xff) continue;                         // not mapped
            uint8_t num = code &0x1F;                        // extract number
            bool flip   = code &0x80;                        // extract flip flag
            bool isBtn  =!(code&0x60);                       // No flag for button
            bool isHat  = code &0x40;                        // extract hat flag
            bool isAxis = code &0x20;                        // extract axis flag
            if(isBtn)  pad.b[num].eBTN  = eBTN[i];           // button event code to eGamepadBtn map
            if(isAxis) pad.a[num].eAXIS = eAXIS[i];          // axis event code to eGamepadAxis map
            if(isAxis) pad.a[num].flip  = flip;              // flip axis
            if(isHat && i==8)  pad.a[hatx_inx].flip = flip;  // flip HAT0X
            if(isHat && i==10) pad.a[haty_inx].flip = flip;  // flip HAT0Y
            //printf("i=%d num=%d isBtn=%d isHat=%d isAxis=%d filp=%d\n",i ,num, isBtn, isHat, isAxis, flip);
        }
    } else {  // Gamepad not listed.  Use heuristics to guess layout.
        bool hasHAT=!!(hatx_inx & haty_inx);
        bool hasTrigger=false;

        auto& a = pad.a;
        a[0].eAXIS = eAXIS_LX;                                                                                 // left stick X (always first listed axis)
        a[1].eAXIS = eAXIS_LY;                                                                                 // left stick Y (always second listed axis)
        for(auto& ax : a) if(ax.AXIS==ABS_HAT0X) {ax.eAXIS=7; hasHAT=true; break;}                             // Hat X (always ABS_HAT0X, or a button)
        for(auto& ax : a) if(ax.AXIS==ABS_HAT0Y) {ax.eAXIS=8; hasHAT=true; break;}                             // Hat Y (always ABS_HAT0Y, or a button)
        for(auto& ax : a) if(!ax.eAXIS && ax.min==a[0].min && ax.max==a[0].max) {ax.eAXIS=eAXIS_RX; break;}    // right stick X (min/max should match left stick)
        for(auto& ax : a) if(!ax.eAXIS && ax.min==a[0].min && ax.max==a[0].max) {ax.eAXIS=eAXIS_RY; break;}    // right stick Y
        for(auto& ax : a) if(!ax.eAXIS && ax.min==0 && ax.max>1) {ax.eAXIS=eAXIS_TL; hasTrigger=true; break;}  // left trigger (Usually has min=0)
        for(auto& ax : a) if(!ax.eAXIS && ax.min==0 && ax.max>1) {ax.eAXIS=eAXIS_TR; hasTrigger=true; break;}  // right trigger
        //for(int i=0; i<axis_cnt; ++i) { auto& ax = a[i]; printf("i:%d  AXIS=%2d eAXIS=%2d  min=%5d  max=%5d  fuzz=%5d  flat=%5d  flip=%d\n", i, ax.AXIS, ax.eAXIS, ax.min, ax.max, ax.fuzz, ax.flat, ax.flip);}

        bool HID_style      = ( hasHAT &&  hasTrigger);  // HID and XInput compliant (XBox)
        bool Nintendo_style = ( hasHAT && !hasTrigger);  // Nintendo uses button triggers
        bool Sony_style     = (!hasHAT &&  hasTrigger);  // Sony uses DPad instead of HAT

        auto& btns = pad.b;
        if(HID_style) {
            for(auto& b : btns) {
                if(b.BTN==BTN_A)       b.eBTN=eBTN_A;
                if(b.BTN==BTN_B)       b.eBTN=eBTN_B;
                if(b.BTN==BTN_X)       b.eBTN=eBTN_X;
                if(b.BTN==BTN_Y)       b.eBTN=eBTN_Y;
                if(b.BTN==BTN_TL)      b.eBTN=eBTN_TL;
                if(b.BTN==BTN_TR)      b.eBTN=eBTN_TR;
                if(b.BTN==BTN_THUMBL)  b.eBTN=eBTN_THUMBL;
                if(b.BTN==BTN_THUMBR)  b.eBTN=eBTN_THUMBR;
                if(b.BTN==BTN_SELECT)  b.eBTN=eBTN_SELECT;
                if(b.BTN==BTN_START)   b.eBTN=eBTN_START;
            }
        }

        if(Nintendo_style) {
            for(auto& b : btns) {
                if(b.BTN==0x130) b.eBTN=eBTN_A;
                if(b.BTN==0x131) b.eBTN=eBTN_B;
                if(b.BTN==0x132) b.eBTN=eBTN_X;
                if(b.BTN==0x133) b.eBTN=eBTN_Y;
                if(b.BTN==0x134) b.eBTN=eBTN_TL;
                if(b.BTN==0x135) b.eBTN=eBTN_TR;
                if(b.BTN==0x136) b.eBTN=-eAXIS_TL;  // button trigger
                if(b.BTN==0x137) b.eBTN=-eAXIS_TR;  // button trigger
                if(b.BTN==0x13a) b.eBTN=eBTN_THUMBL;
                if(b.BTN==0x13b) b.eBTN=eBTN_THUMBR;
                if(b.BTN==0x138) b.eBTN=eBTN_SELECT;
                if(b.BTN==0x139) b.eBTN=eBTN_START;
            }
        }

        if(Sony_style) {  // untested
            for(auto& b : btns) {
                if(b.BTN==0x130) b.eBTN=eBTN_A;
                if(b.BTN==0x131) b.eBTN=eBTN_B;
                if(b.BTN==0x132) b.eBTN=eBTN_X;
                if(b.BTN==0x133) b.eBTN=eBTN_Y;
                if(b.BTN==0x134) b.eBTN=eBTN_TL;
                if(b.BTN==0x135) b.eBTN=eBTN_TR;
                if(b.BTN==0x13d) b.eBTN=eBTN_THUMBL;
                if(b.BTN==0x13e) b.eBTN=eBTN_THUMBR;
                if(b.BTN==0x138) b.eBTN=eBTN_SELECT;
                if(b.BTN==0x139) b.eBTN=eBTN_START;
            }
        }

        for(auto& b : btns) {  // DPad buttons (Sony?)
            if(b.BTN==BTN_DPAD_UP)    b.eBTN=eDPAD_UP;
            if(b.BTN==BTN_DPAD_DOWN)  b.eBTN=eDPAD_DOWN;
            if(b.BTN==BTN_DPAD_LEFT)  b.eBTN=eDPAD_LEFT;
            if(b.BTN==BTN_DPAD_RIGHT) b.eBTN=eDPAD_RIGHT;
        }

        //for(int i=0; i<MAX_BTNS; ++i) {auto& b = pad.map.b[i]; printf("BTN=%d eBTN=%d\n", b.BTN, b.eBTN);}
    }
}

void Window_xcb::ReadGamepadEvents() {
    DetectGamepads();
    for (int i=0; i<MAX_GAMEPADS; ++i) {
        Evdev&   ev  = evdev[i];
        Gamepad& pad = gamepad[i];
        if (!pad.active) continue;

       int rc=0;
        struct input_event event;
        while ((rc=libevdev_next_event(ev.dev, LIBEVDEV_READ_FLAG_NORMAL, &event)) == 0) {
            if (event.type == EV_KEY) { GamepadBtnEvent (i, event); } else // Button press/release
            if (event.type == EV_ABS) { GamepadAxisEvent(i, event); }      // Analog axes and hat buttons
        }
        if(rc==-ENODEV) DisconnectGamepad(i);
    }
}

void Window_xcb::GamepadBtnEvent(uint8_t id, input_event event) {
    auto& ev = evdev[id];
    uint keycode = event.code;
    //printf("keycode=%d (0x%3x) %d\n", keycode, keycode, event.value);
    if(event.value>1) return;  // ignore repeats (0=up 1=down 2=repeat)
    for(auto& b : ev.b) if(keycode==b.BTN) {
        if(b.eBTN>0) eventFIFO.push(gpadButton(id, b.eBTN, event.value));
        if(b.eBTN<0) eventFIFO.push(gpadAxis  (id,-b.eBTN, event.value));
    }
}

void Window_xcb::GamepadAxisEvent(uint8_t id, input_event event) {
    Gamepad& pad = gamepad[id];
    Evdev&   ev  = evdev[id];

    //------------------------------------------------------------------------------
    auto find_axis = [&](uint axiscode) -> Evdev::Axes& {
        for(auto& a : ev.a) if(axiscode==a.AXIS) return a;
        return ev.a[0];
    };

    auto Hat = [&](int val, int btnNeg, int btnPos) { // convert hat axis values to button events
        if((val!=-1) && ( pad.buttons[btnNeg])) eventFIFO.push(gpadButton(id, btnNeg, 0));
        if((val!= 1) && ( pad.buttons[btnPos])) eventFIFO.push(gpadButton(id, btnPos, 0));
        if((val==-1) && (!pad.buttons[btnNeg])) eventFIFO.push(gpadButton(id, btnNeg, 1));
        if((val== 1) && (!pad.buttons[btnPos])) eventFIFO.push(gpadButton(id, btnPos, 1));
    };

    auto isFuzz = [](int value, auto& a) -> bool { // detect fuzz events
        int delta = abs(a.prev - value);
        if(delta<a.fuzz) return true;
        a.prev = value;
        return false;
    };

    auto Trigger = [](int value, auto& a) -> float { // Apply dead-zone, normalize
        int val   = std::max(value-a.min-a.flat,0);
        int range = std::max(a.max-a.min-a.flat,1);
        return val / (float)range;
    };

    auto Thumb = [](int value, auto& a) -> float { // Apply dead-zone, normalize
        int center = (a.min + a.max) / 2;
        int centered = value - center;
        int sign  = centered<0 ? -1:1;
        int val   = std::max(std::abs(centered) - a.flat, 0);
        int range = a.max - center - a.flat;
        return (val / (float)range) * sign;
    };
    //------------------------------------------------------------------------------

    auto& a = find_axis(event.code);
    int val = a.flip ? -event.value : event.value;
    if(event.code == ABS_HAT0X) {Hat(val, eDPAD_LEFT, eDPAD_RIGHT); return;}
    if(event.code == ABS_HAT0Y) {Hat(val, eDPAD_UP,   eDPAD_DOWN);  return;}
    if(event.code > 10) return;         // Ignore HAT1+

    if(isFuzz(event.value, a)) return;  // defuzz
    bool isTrigger = (a.eAXIS==eAXIS_TL || a.eAXIS==eAXIS_TR);
    float fval = isTrigger? Trigger(event.value, a)
                          : Thumb  (event.value, a);

    if(pad.axes[a.eAXIS] == fval) return;  // deadzone
    if(a.eAXIS==eAXIS_LY || a.eAXIS==eAXIS_RY) fval=-fval; // flip y axis
    if(a.flip) fval=-fval;
    eventFIFO.push(gpadAxis(id, a.eAXIS, fval));
}

/*
// eg. SetGamepadRumble(0, 20000, 0);
void Window_xcb::SetGamepadRumble(int index, uint16_t weak, uint16_t strong) {  // TODO
    if (index < 0 || index >= MAX_GAMEPADS || gamepads[index].fd < 0) return;

    struct ff_effect effect = {};
    effect.type = FF_RUMBLE;
    effect.id = -1;
    effect.u.rumble.strong_magnitude = strong;
    effect.u.rumble.weak_magnitude = weak;
    if (ioctl(gamepads[index].fd, EVIOCSFF, &effect) < 0) return;

    struct input_event play = {};
    play.type = EV_FF;
    play.code = effect.id;
    play.value = 1;

    write(gamepads[index].fd, &play, sizeof(play));  // Start rumble
    usleep(500000);                                  // Let it run for 500ms
    ioctl(gamepads[index].fd, EVIOCRMFF, effect.id); // Remove the effect
}
*/

#endif

#ifdef ENABLE_CLIPBOARD
    void Window_xcb::InitClipboard() {
        atom_CLIPBOARD   = GetAtom("CLIPBOARD");
        atom_UTF8_STRING = GetAtom("UTF8_STRING");
        atom_STRING      = GetAtom("STRING");
        atom_PROPERTY    = GetAtom("XSEL_DATA");
    }

    bool Window_xcb::RequestClipboard() {
        xcb_convert_selection(xcb_connection, xcb_window,
            atom_CLIPBOARD,    // selection
            atom_UTF8_STRING,  // target
            atom_PROPERTY,     // property to receive data
            XCB_CURRENT_TIME);
        xcb_flush(xcb_connection);
        return true;
    }


    const char* Window_xcb::getClipboardText() {
        RequestClipboard();  // triggers XCB_SELECTION_NOTIFY event
        xcb_generic_event_t* event = xcb_wait_for_event(xcb_connection);
        TranslateEvent(event);
        return clipboard.c_str();
    }

    void Window_xcb::setClipboardText(const char* str) {
        clipboard = str;
        xcb_set_selection_owner(xcb_connection, xcb_window, atom_CLIPBOARD, XCB_CURRENT_TIME);
        xcb_flush(xcb_connection);
    }
#endif

//-------------

#endif  // GWINDOW_IMPLEMENTATION

#endif  // VK_USE_PLATFORM_XCB_KHR
//==============================================================
