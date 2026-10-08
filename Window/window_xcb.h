//#define VK_USE_PLATFORM_XCB_KHR
//#define GWINDOW_IMPLEMENTATION

//============================XCB===============================
#ifdef VK_USE_PLATFORM_XCB_KHR

#ifndef WINDOW_XCB
#define WINDOW_XCB

//#define ENABLE_MULTITOUCH  // requires libxi-dev
//#define ENABLE_GAMEPAD     // requires libevdev-dev (14kb)
//#define ENABLE_CLIPBOARD   // requires libxcb-icccm4-dev + libxcb1-dev
//#define ENABLE_SHOWIMAGE   // requires libxcb-image0-dev + libxcb1-dev
//#define ENABLE_CURSOR      // requires libxcb-cursor-dev + libxcb1-dev + libxcb-cursor0
//#define ENABLE_FULLSCREEN  // requires libxcb1-dev

//-------------------------------------------------
#include "WindowBase.h"
#include "gamepad_linux.h"

//#include <xcb/xcb.h>            // XCB only
//#include <X11/Xlib.h>           // XLib only
#include <X11/Xlib-xcb.h>         // Xlib + XCB
#include <xkbcommon/xkbcommon.h>  // Keyboard   libxkbcommon-dev
#include <stdlib.h>               // atof
#include <assert.h>
#ifdef ENABLE_SHOWIMAGE
#include <xcb/xcb_image.h>        // showImage  libxcb-image0-dev
#endif
#ifdef ENABLE_CURSOR
#include <xcb/xcb_cursor.h>       // mouse cursor icons
#endif
#ifdef ENABLE_FULLSCREEN
#include <xcb/xcb.h>
#endif
#ifdef ENABLE_CLIPBOARD
#include <xcb/xcb_icccm.h>
#endif
#ifdef ENABLE_MULTITOUCH
#include <xcb/xinput.h>
#include <X11/extensions/XInput2.h>  // MultiTouch
#endif
//-------------------------------------------------

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

struct native_handle {
    Display* display;
    xcb_connection_t* xcb_connection;
    xcb_screen_t* xcb_screen;
    xcb_window_t xcb_window;
};

//=============================XCB==============================
class Window_xcb : public WindowBase, GamepadLinux {
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
    //------------------
    //----- Cursor -----
#ifdef ENABLE_CURSOR
    xcb_cursor_context_t *cursor_ctx;
    xcb_cursor_t cursors[12];
#endif
    //------------------
    //---- Gamepad ----
#ifdef ENABLE_GAMEPAD
    void onGpadConnect(uint8_t pad, bool active)            override {eventFIFO.push(gpadConnect(pad, active));}
    void onGpadButton (uint8_t pad, uint8_t btn, bool down) override {eventFIFO.push(gpadButton(pad,btn,down));}
    void onGpadAxis   (uint8_t pad, uint8_t axis, float val)override {eventFIFO.push(gpadAxis(pad, axis, val));}
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
    native_handle* getNativeHandle() const {return (native_handle*)&display;}
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
    void InitClipboard(){}
#endif



};
//==============================================================
#endif  // WINDOW_XCB

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

    printf("Creating XCB-Window...\n");

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

#ifndef ENABLE_DPIAWARE
    setScale(1);
#endif

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

    {  // select which events to listen to
        unsigned char buf[3] = {};
        XIEventMask mask     = {};
        mask.deviceid        = XIAllMasterDevices;
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
    xcb_atom_t resource_manager = GetAtom("RESOURCE_MANAGER");
    xcb_get_property_cookie_t cookie = xcb_get_property(
        xcb_connection, 0, xcb_screen->root, resource_manager, XCB_GET_PROPERTY_TYPE_ANY, 0, UINT32_MAX);
    xcb_get_property_reply_t* reply = xcb_get_property_reply(xcb_connection, cookie, nullptr);
    const char* text = (const char*)xcb_get_property_value(reply);
    //int len = xcb_get_property_value_length(reply);
    //std::string resources(text, len);
    //printf("%s", text);
    const char* key = "Xft.dpi:";
    const char* p = strstr(text, key);
    float scale = 1.0f;
    if (p) {
        p += strlen(key);
        scale = strtof(p, nullptr) / 96.0f;
    }
    free(reply);
    return scale;
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
