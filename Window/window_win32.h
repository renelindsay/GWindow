//#define VK_USE_PLATFORM_WIN32_KHR
//#define GWINDOW_IMPLEMENTATION

//==========================Win32===============================
#ifdef VK_USE_PLATFORM_WIN32_KHR

#ifndef WINDOW_WIN32
#define WINDOW_WIN32

//#define ENABLE_MULTITOUCH //1kb
//#define ENABLE_GAMEPAD    //2kb  (requires xinput)
//#define ENABLE_CLIPBOARD  //1kb
//#define ENABLE_SHOWIMAGE
//#define ENABLE_FULLSCREEN
#define ENABLE_CURSOR

#include "WindowBase.h"
#include <windowsx.h>  // Mouse
#include <assert.h>
#include <ShellScalingApi.h> // for GetDpiForWindow
#include <Xinput.h>          // for Gamepad

#define MIN(a,b) ((a<b)?(a):(b))
#define MAX(a,b) ((a>b)?(a):(b))

//for MINGW
#undef  _WIN32_WINNT
#define _WIN32_WINNT 0x0A00  // Windows 10
#undef  WINVER
#define WINVER 0x0A00

// Convert native Win32 keyboard scancode to cross-platform USB HID code.
const unsigned char WIN32_TO_HID[256] = {
      0,  0,  0,  0,  0,  0,  0,  0, 42, 43,  0,  0,  0, 40,  0,  0,    // 16
    225,224,226, 72, 57,  0,  0,  0,  0,  0,  0, 41,  0,  0,  0,  0,    // 32
     44, 75, 78, 77, 74, 80, 82, 79, 81,  0,  0,  0, 70, 73, 76,  0,    // 48
     39, 30, 31, 32, 33, 34, 35, 36, 37, 38,  0,  0,  0,  0,  0,  0,    // 64
      0,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13, 14, 15, 16, 17, 18,    // 80
     19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29,  0,  0,  0,  0,  0,    // 96
     98, 89, 90, 91, 92, 93, 94, 95, 96, 97, 85, 87,  0, 86, 99, 84,    //112
     58, 59, 60, 61, 62, 63, 64, 65, 66, 67, 68, 69,104,105,106,107,    //128
    108,109,110,111,112,113,114,115,  0,  0,  0,  0,  0,  0,  0,  0,    //144
     83, 71,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,    //160
    225,229,224,228,226,230,  0,  0,  0,  0,  0,  0,  0,127,128,129,    //176    L/R shift/ctrl/alt  mute/vol+/vol-
      0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 51, 46, 54, 45, 55, 56,    //192
     53,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,    //208
      0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 47, 49, 48, 52,  0,    //224
      0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,    //240
      0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0     //256
};
//=============================Win32============================
class Window_win32 : public WindowBase {
    HINSTANCE hInstance;
    HWND hWnd;
    RECT rect;            // Save window rect while in fullscreen mode
    DWORD style;          // Save window stype while in fullscreen mode
    CMTouch MTouch;       // Multi-Touch device
    HBITMAP DIB = 0;      // For showImage().  Holds image to display.
    HCURSOR cursors[12];  // For mouse cursors

    void Create(const char* title="Window", uint width=640, uint height=480);
    void DetectGamepads();
    void ReadGamepadEvents();
    void EnableDPIAware();
public:
    void setTitle(const char* title);
    void setPosition(uint x, uint y);
    void setSize(uint w, uint h);

public:
    Window_win32(){Create();};
    Window_win32(const char* title, uint width, uint height);
    virtual ~Window_win32();
    EventType getEvent(bool wait_for_event = false);
    const void* getNativeHandle() const {return &hInstance;};
    float getDisplayScale();
#ifdef ENABLE_SHOWIMAGE
    void showImage(uint32_t* buf, uint32_t width, uint32_t height);
#endif
#ifdef ENABLE_CURSOR
    void setCursor(eCursor id);
#endif
#ifdef ENABLE_CLIPBOARD
    void setClipboardText(const char* text) override;
    const char* getClipboardText() override;
#endif
#ifdef ENABLE_FULLSCREEN
    void setFullscreen(bool enable);
#endif

};
//==============================================================
#endif

#ifdef GWINDOW_IMPLEMENTATION

//=====================Win32 IMPLEMENTATION=====================
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

Window_win32::Window_win32(const char* title, uint width, uint height) {
    Create(title, width, height);
}

void Window_win32::Create(const char* title, uint width, uint height) {
    shape.width  = width;
    shape.height = height;
    running      = true;
    //printf("Creating Win32 Window...\n");

    EnableDPIAware();
    hInstance = GetModuleHandle(NULL);

    // Initialize the window class structure:
    WNDCLASSEX win_class;
    win_class.cbSize        = sizeof(WNDCLASSEX);
    win_class.style         = CS_HREDRAW | CS_VREDRAW;
    win_class.lpfnWndProc   = WndProc;
    win_class.cbClsExtra    = 0;
    win_class.cbWndExtra    = 0;
    win_class.hInstance     = hInstance;
    win_class.hIcon         = LoadIcon(NULL, IDI_APPLICATION);
#ifdef ENABLE_CURSOR
    win_class.hCursor       = LoadCursor(NULL, IDC_ARROW);
#endif
    win_class.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
    win_class.lpszMenuName  = NULL;
    win_class.lpszClassName = title;
    win_class.hIconSm       = LoadIcon(NULL, IDI_WINLOGO);
    // Register window class:
    ATOM atom = RegisterClassEx(&win_class);
    assert(atom && "Failed to register the window class.");

    // Create window with the registered class:
    RECT wr = {0, 0, (LONG)width, (LONG)height};
    AdjustWindowRect(&wr, WS_OVERLAPPEDWINDOW, FALSE);
    hWnd = CreateWindowEx(0,
                          title,                                          // class name
                          title,                                          // app name
                          WS_VISIBLE | WS_SYSMENU | WS_OVERLAPPEDWINDOW,  // window style
                          100, 100,                                       // x/y coords
                          wr.right - wr.left,                             // width
                          wr.bottom - wr.top,                             // height
                          NULL,                                           // handle to parent
                          NULL,                                           // handle to menu
                          hInstance,                                    // hInstance
                          NULL);                                          // no extra parameters
    assert(hWnd && "Failed to create a window.");

#ifdef ENABLE_CURSOR
    cursors[0] = LoadCursor(NULL, IDC_ARROW);       // Arrow
    cursors[1] = LoadCursor(NULL, IDC_IBEAM);       // Caret (Text Input)
    cursors[2] = LoadCursor(NULL, IDC_SIZEALL);     // Resize All
    cursors[3] = LoadCursor(NULL, IDC_SIZENS);      // Resize NS
    cursors[4] = LoadCursor(NULL, IDC_SIZEWE);      // Resize EW
    cursors[5] = LoadCursor(NULL, IDC_SIZENESW);    // Resize NESW
    cursors[6] = LoadCursor(NULL, IDC_SIZENWSE);    // Resize NWSE
    cursors[7] = LoadCursor(NULL, IDC_HAND);        // Hand
    cursors[8] = LoadCursor(NULL, IDC_WAIT);        // Wait
    cursors[9] = LoadCursor(NULL, IDC_APPSTARTING); // Progress
    cursors[10]= LoadCursor(NULL, IDC_NO);          // Not Allowed
#endif

    eventFIFO.push(resizeEvent(width, height));
}

Window_win32::~Window_win32() { DestroyWindow(hWnd); }

void Window_win32::setTitle(const char* title) { SetWindowText(hWnd, title); }

void Window_win32::setPosition(uint x, uint y) {
    SetWindowPos(hWnd, NULL, x, y, 0, 0, SWP_NOZORDER | SWP_NOACTIVATE | SWP_NOSIZE);
    if (x != shape.x || y != shape.y) eventFIFO.push(moveEvent(x, y));  // Trigger window moved event
}

void Window_win32::setSize(uint w, uint h) {
    RECT wr = {0, 0, (LONG)w, (LONG)h};
    AdjustWindowRect(&wr, WS_OVERLAPPEDWINDOW, FALSE);  // Add border size to create desired client area size
    int total_width = wr.right - wr.left;
    int total_height = wr.bottom - wr.top;
    SetWindowPos(hWnd, NULL, 0, 0, total_width, total_height, SWP_NOZORDER | SWP_NOACTIVATE | SWP_NOMOVE);
    if ((w != shape.width) | (h != shape.height)) eventFIFO.push(resizeEvent(w, h));  // Trigger resize event
}

#define WM_RESHAPE (WM_USER + 0)
#define WM_ACTIVE  (WM_USER + 1)

EventType Window_win32::getEvent(bool wait_for_event) {
    // EventType event;
    if (!eventFIFO.isEmpty()) return eventFIFO.pop();

    if (running) {
        MSG msg = {};
        if(wait_for_event) running = (GetMessage(&msg, NULL, 16, 0) > 0);           // Blocking mode
        else if(!PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) return {EventType::NONE};  // Non-blocking mode

        TranslateMessage(&msg);
        int16_t x = GET_X_LPARAM(msg.lParam);
        int16_t y = GET_Y_LPARAM(msg.lParam);

        //--Convert Shift / Ctrl / Alt key messages to LeftShift / RightShift / LeftCtrl / RightCtrl / LeftAlt / RightAlt--
        if (msg.message == WM_KEYDOWN || msg.message == WM_KEYUP) {
            if (msg.wParam == VK_CONTROL) msg.wParam = (msg.lParam & (1 << 24)) ? VK_RCONTROL : VK_LCONTROL;
            if (msg.wParam == VK_SHIFT) {
                if (!!(::GetKeyState(VK_LSHIFT) & 128) != getKeyState(eKEY_LeftShift )) PostMessage(hWnd, msg.message, VK_LSHIFT, 0);
                if (!!(::GetKeyState(VK_RSHIFT) & 128) != getKeyState(eKEY_RightShift)) PostMessage(hWnd, msg.message, VK_RSHIFT, 0);
                return {EventType::NONE};
            }
        } else if (msg.message == WM_SYSKEYDOWN || msg.message == WM_SYSKEYUP) {
            if (msg.wParam == VK_MENU) msg.wParam = (msg.lParam & (1 << 24)) ? VK_RMENU : VK_LMENU;
        }
        //-----------------------------------------------------------------------------------------------------------------

        static char buf[4] = {};
        uint8_t bestBtn = getBtnState(1) ? 1 : getBtnState(2) ? 2 : getBtnState(3) ? 3 : 0;
        switch (msg.message) {
            //--Mouse events--
            case WM_MOUSEMOVE  : return mouseEvent(eMOVE, x, y, bestBtn);
            case WM_LBUTTONDOWN: return mouseEvent(eDOWN, x, y, 1);
            case WM_MBUTTONDOWN: return mouseEvent(eDOWN, x, y, 2);
            case WM_RBUTTONDOWN: return mouseEvent(eDOWN, x, y, 3);
            case WM_LBUTTONUP  : return mouseEvent(eUP  , x, y, 1);
            case WM_MBUTTONUP  : return mouseEvent(eUP  , x, y, 2);
            case WM_RBUTTONUP  : return mouseEvent(eUP  , x, y, 3);
            //--Mouse wheel events--
            case WM_MOUSEWHEEL: {
                uint8_t wheel = (GET_WHEEL_DELTA_WPARAM(msg.wParam) > 0) ? 4 : 5;
                POINT point = {x, y};
                ScreenToClient(msg.hwnd, &point);
                return {EventType::MOUSE, {eDOWN, (int16_t)point.x, (int16_t)point.y, wheel}};
            }
            //--Keyboard events--
            case WM_KEYDOWN   : return keyEvent(eDOWN, WIN32_TO_HID[msg.wParam]);
            case WM_KEYUP     : return keyEvent(eUP  , WIN32_TO_HID[msg.wParam]);
            case WM_SYSKEYDOWN: {MSG discard; GetMessage(&discard, NULL, 0, 0);     // Alt-key triggers a WM_MOUSEMOVE message... Discard it.
                                return keyEvent(eDOWN, WIN32_TO_HID[msg.wParam]); } // +alt key
            case WM_SYSKEYUP  : return keyEvent(eUP  , WIN32_TO_HID[msg.wParam]);   // +alt key

            //--Char event--
            case WM_CHAR: { strncpy_s(buf, (const char*)&msg.wParam, 4);  return textEvent(buf); }  // return UTF8 code of key pressed
            //--Window events--
            case WM_ACTIVE: { return focusEvent(msg.wParam != WA_INACTIVE); }

            case WM_RESHAPE: {
                if (!has_focus) {
                    PostMessage(hWnd, WM_RESHAPE, msg.wParam, msg.lParam);  // Repost this event to the queue
                    return focusEvent(true);                                // Activate window before reshape
                }

                RECT r;
                GetClientRect(hWnd, &r);
                uint16_t w = (uint16_t)(r.right - r.left);
                uint16_t h = (uint16_t)(r.bottom - r.top);
                if (w != shape.width || h != shape.height) return resizeEvent(w, h);  // window resized

                GetWindowRect(hWnd, &r);
                int16_t x = (int16_t)r.left;
                int16_t y = (int16_t)r.top;
                if (x != shape.x || y != shape.y) return moveEvent(x, y);  // window moved
                break;
            }
            case WM_CLOSE: {
                if(msg.hwnd == hWnd) {
                    //printf("WM_CLOSE\n");
                    if(DIB) {DeleteObject(DIB); DIB=0;}
                    return closeEvent();
                }
                break;
            }
            case WM_PAINT: {
                if(DIB) {
                    PAINTSTRUCT ps{};
                    HDC hDC = BeginPaint(hWnd, &ps);
                    HDC hMemDC = CreateCompatibleDC(hDC);
                    HBITMAP hOldBitmap = (HBITMAP)SelectObject(hMemDC, DIB);
                    int w = ps.rcPaint.right;
                    int h = ps.rcPaint.bottom;
                    //float s = getScale();
                    //StretchBlt(hDC, 0, 0, w*s, h*s, hMemDC, 0, 0, w, h, SRCCOPY);
                    BitBlt(hDC, 0, 0, w, h, hMemDC, 0, 0, SRCCOPY);
                    SelectObject(hMemDC, hOldBitmap);
                    DeleteDC(hMemDC);
                    EndPaint(hWnd, &ps);
                }
                break;
            }

#ifdef ENABLE_MULTITOUCH

#define WM_POINTERUPDATE 0x0245
#define WM_POINTERDOWN   0x0246
#define WM_POINTERUP     0x0247

            case WM_POINTERUPDATE:
            case WM_POINTERDOWN:
            case WM_POINTERUP: {
                POINTER_INFO pointerInfo;
                if (GetPointerInfo(GET_POINTERID_WPARAM(msg.wParam), &pointerInfo)) {
                    uint  id = pointerInfo.pointerId;
                    POINT pt = pointerInfo.ptPixelLocation;
                    ScreenToClient(hWnd, &pt);
                    switch (msg.message) {
                        case WM_POINTERDOWN  : return MTouch.Event_by_ID(eDOWN, x, y,  0, id);  // touch down event
                        case WM_POINTERUPDATE: return MTouch.Event_by_ID(eMOVE, x, y, id, id);  // touch move event
                        case WM_POINTERUP    : return MTouch.Event_by_ID(eUP  , x, y, id,  0);  // touch up event
                    }
                }
            }
#endif
        }
        DispatchMessage(&msg);
    }

    ReadGamepadEvents();
    return {EventType::NONE};
}

static bool inClientArea = false;
// MS-Windows event handling function:
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
        case WM_CLOSE:
            PostMessage(hWnd, WM_CLOSE, 0, 0);  // for onCloseEvent
            return 0;
        case WM_DESTROY:
            //LOGI("WM_DESTROY\n");
            PostQuitMessage(0);
            return 0;
        case WM_PAINT:
             //LOGI("WM_PAINT\n");
            return 0;
        case WM_GETMINMAXINFO: {  // set window's minimum size
            MINMAXINFO* pmmi = (MINMAXINFO*)lParam;
            pmmi->ptMinTrackSize.x = 8;
            pmmi->ptMinTrackSize.y = 8;
            return 0;
        }
        case WM_NCHITTEST: {
            POINT pt;
            pt.x = GET_X_LPARAM(lParam);
            pt.y = GET_Y_LPARAM(lParam);
            ScreenToClient(hWnd, &pt);

            RECT rect;
            GetClientRect(hWnd, &rect);

            // Flag if mouse is in client area (for mouse cursor)
            inClientArea = ((pt.x > rect.left)&&(pt.x < rect.right)
                          &&(pt.y > rect.top )&&(pt.y < rect.bottom));
            break;
        }

//        case WM_IME_CHAR: 
//            wprintf(L"WM_IME_CHAR : %c\n", (wchar_t)wParam);
//            return 0;

        case WM_SIZE         : { PostMessage(hWnd, WM_RESHAPE, 0, 0);          break; }
        case WM_EXITSIZEMOVE : { PostMessage(hWnd, WM_RESHAPE, 0, 0);          break; }
        case WM_ACTIVATE     : { PostMessage(hWnd, WM_ACTIVE, wParam, lParam); break; }
        default: break;
    }
    return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

float Window_win32::getDisplayScale() {
    int dpi = GetDpiForWindow(hWnd);
    //printf("dpi = %d\n", dpi);
    return dpi/96.f;
}
#ifdef ENABLE_SHOWIMAGE
void Window_win32::showImage(uint32_t* buf, uint32_t width, uint32_t height) {  // using GDI only
    if(DIB) {DeleteObject(DIB); DIB=0;}  // delete previous bitmap
    BITMAPINFO bmi{};
    bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    bmi.bmiHeader.biWidth = width;
    bmi.bmiHeader.biHeight = -height;  // Negative height for top-down DIB
    bmi.bmiHeader.biPlanes = 1;
    bmi.bmiHeader.biBitCount = 32;     // RGBA format
    bmi.bmiHeader.biCompression = BI_RGB;

    HDC hdc = GetDC(0);
    DIB = CreateDIBitmap(hdc, &bmi.bmiHeader, CBM_INIT, buf, &bmi, DIB_RGB_COLORS);
    ReleaseDC(NULL, hdc);
    InvalidateRect(hWnd, NULL, false);
}
#endif

#ifdef ENABLE_CURSOR
void Window_win32::setCursor(eCursor id) {      // Override mouse cursor,
    if(inClientArea) ::SetCursor(cursors[id]);  // but not on window edges
}
#endif

//---- Gamepads ----
#ifdef ENABLE_GAMEPAD
void Window_win32::DetectGamepads() {
    static DWORD last_time = 0;
    DWORD curr_time = GetTickCount();
    if(curr_time - last_time < 1000) return; // Only check once per second
    last_time = curr_time;

    for (DWORD i = 0; i < MAX_GAMEPADS; i++) {
        XINPUT_STATE state;
        Gamepad& pad = gamepad[i];
        bool active = (XInputGetState(i, &state) == ERROR_SUCCESS);
        if(active != pad.active) eventFIFO.push(gpadConnect(i, active));
    }
}

void Window_win32::ReadGamepadEvents() {
    DetectGamepads();
    for (DWORD i = 0; i < MAX_GAMEPADS; i++) {
        Gamepad& pad = gamepad[i];
        if(!pad.active) continue;
        XINPUT_STATE state;
        if (XInputGetState(i, &state) != ERROR_SUCCESS) continue;

        auto btnCheck = [&](eGamepadBtn btn, bool isDown) {
            if(pad.buttons[btn] == isDown) return;
            eventFIFO.push(gpadButton(i, btn, isDown));
        };

        WORD buttons = state.Gamepad.wButtons;
        btnCheck(eBTN_A,      buttons & XINPUT_GAMEPAD_A);
        btnCheck(eBTN_B,      buttons & XINPUT_GAMEPAD_B);
        btnCheck(eBTN_X,      buttons & XINPUT_GAMEPAD_X);
        btnCheck(eBTN_Y,      buttons & XINPUT_GAMEPAD_Y);
        btnCheck(eBTN_TL,     buttons & XINPUT_GAMEPAD_LEFT_SHOULDER);
        btnCheck(eBTN_TR,     buttons & XINPUT_GAMEPAD_RIGHT_SHOULDER);
        btnCheck(eBTN_THUMBL, buttons & XINPUT_GAMEPAD_LEFT_THUMB);
        btnCheck(eBTN_THUMBR, buttons & XINPUT_GAMEPAD_RIGHT_THUMB);
        btnCheck(eBTN_SELECT, buttons & XINPUT_GAMEPAD_BACK);
        btnCheck(eBTN_START,  buttons & XINPUT_GAMEPAD_START);
      //btnCheck(eBTN_MODE,   buttons & XINPUT_GAMEPAD_GUIDE);
        btnCheck(eDPAD_UP,    buttons & XINPUT_GAMEPAD_DPAD_UP);
        btnCheck(eDPAD_DOWN,  buttons & XINPUT_GAMEPAD_DPAD_DOWN);
        btnCheck(eDPAD_LEFT,  buttons & XINPUT_GAMEPAD_DPAD_LEFT);
        btnCheck(eDPAD_RIGHT, buttons & XINPUT_GAMEPAD_DPAD_RIGHT);

        auto axisCheck = [&](eGamepadAxis axis, float val) {
            if(pad.axes[axis] == val) return;
            eventFIFO.push(gpadAxis(i, axis, val));
        };

        axisCheck(eAXIS_LX, state.Gamepad.sThumbLX / 32767.f);
        axisCheck(eAXIS_LY, state.Gamepad.sThumbLY / 32767.f);
        axisCheck(eAXIS_RX, state.Gamepad.sThumbRX / 32767.f);
        axisCheck(eAXIS_RY, state.Gamepad.sThumbRY / 32767.f);
        axisCheck(eAXIS_TL, state.Gamepad.bLeftTrigger / 255.f);
        axisCheck(eAXIS_TR, state.Gamepad.bRightTrigger / 255.f);
    }
}
#else
    void Window_win32::DetectGamepads() {}
    void Window_win32::ReadGamepadEvents(){}
#endif
//------------------

//---- Clipboard ----
#ifdef ENABLE_CLIPBOARD
    void Window_win32::setClipboardText(const char* text) {
        if (!OpenClipboard(nullptr)) return;
        EmptyClipboard();

        size_t len = strlen(text) + 1;
        HGLOBAL hMem = GlobalAlloc(GMEM_MOVEABLE, len);
        if (!hMem) { CloseClipboard(); return; }

        memcpy(GlobalLock(hMem), text, len);
        GlobalUnlock(hMem);
        SetClipboardData(CF_TEXT, hMem);
        CloseClipboard();
    }

    const char* Window_win32::getClipboardText() {
        if (!OpenClipboard(nullptr)) return nullptr;

        HANDLE hData = GetClipboardData(CF_TEXT);
        if (!hData) { CloseClipboard(); return nullptr; }

        char* pszText = static_cast<char*>(GlobalLock(hData));
        if (pszText) {
            clipboard = pszText;
            GlobalUnlock(hData);
        }
        CloseClipboard();
        return clipboard.c_str();
    }

#endif ENABLE_CLIPBOARD
//-------------------

#ifdef ENABLE_FULLSCREEN
    void Window_win32::setFullscreen(bool enable) {
        if(enable==fullscreen) return;
        fullscreen = enable;

        if(enable) {
            // Save windowed rect and style
            style = GetWindowLong(hWnd, GWL_STYLE);
            GetWindowRect(hWnd, &rect);

            // Remove borders and title bar
            SetWindowLong(hWnd, GWL_STYLE, style & ~(WS_OVERLAPPEDWINDOW));

            // Get monitor dimensions
            HMONITOR hMonitor = MonitorFromWindow(hWnd, MONITOR_DEFAULTTONEAREST);
            MONITORINFO mi = { sizeof(mi) };
            if (GetMonitorInfo(hMonitor, &mi)) {
                float scale = getDisplayScale();
                int x = int(mi.rcMonitor.left / scale);
                int y = int(mi.rcMonitor.top / scale);
                int w = int((mi.rcMonitor.right - mi.rcMonitor.left) / scale);
                int h = int((mi.rcMonitor.bottom - mi.rcMonitor.top) / scale);
                SetWindowPos(hWnd, HWND_TOP, x, y, w, h, SWP_NOOWNERZORDER | SWP_FRAMECHANGED);
            }
        }else{
            // Restore windowed style and size
            SetWindowLong(hWnd, GWL_STYLE, style);
            int x = rect.left;
            int y = rect.top;
            int w = rect.right - rect.left;
            int h = rect.bottom - rect.top;
            SetWindowPos(hWnd, HWND_NOTOPMOST, x, y, w, h, SWP_NOOWNERZORDER | SWP_FRAMECHANGED);
        }
    }
#endif  // ENABLE_FULLSCREEN

void Window_win32::EnableDPIAware() {
    auto user32 = LoadLibraryA("user32.dll");  // Windows 10+
    if (user32) {
        using SetDpiAwarenessContext_t = BOOL(WINAPI*)(DPI_AWARENESS_CONTEXT);
        auto setContext = reinterpret_cast<SetDpiAwarenessContext_t>(
            GetProcAddress(user32, "SetProcessDpiAwarenessContext"));
        if (setContext) {
            setContext(DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE_V2);
            return;
        }
    }
    SetProcessDPIAware();  // Fallback for Vista / Windows 7
}

#endif  // GWINDOW_IMPLEMENTATION

#endif  // VK_USE_PLATFORM_WIN32_KHR
//==============================================================
