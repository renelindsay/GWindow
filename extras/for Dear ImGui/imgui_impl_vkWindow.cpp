// dear imgui: Platform Binding for VkWindow
//
// VkWindow is a cross-platform library for creating a window for graphics rendering.
// It provides input event hooks for keyboard, mouse, touch-screen and gamepad events,
// and works on Windows, Linux and Android. Bring your own graphics renderer. (Vulkan/OpenGL/pixbuf)

#include "VkWindow.h"
#include "imgui_impl_vkWindow.h"
#include "imgui.h"
#include <chrono>

float min(float a, float b) { return a>b?b:a; }
float max(float a, float b) { return a>b?a:b; }

static VkWindow* g_window = nullptr;
static float g_scale = 1.f;
static ImGuiKey KeyMap[256]{};

static const char* ImGui_ImplvkWindow_GetClipboardText(void* user_data) {
    return g_window->getClipboardText();
}

static void ImGui_ImplvkWindow_SetClipboardText(void* user_data, const char* text) {
    g_window->setClipboardText(text);
}

bool ImGui_ImplvkWindow_Init(VkWindow* window) {
    g_window = window;
    g_scale = g_window->getScale();

    // Setup back-end capabilities flags
    ImGuiIO& io = ImGui::GetIO();
    io.BackendFlags |= ImGuiBackendFlags_HasGamepad;         // Gamepad supported
    io.BackendFlags |= ImGuiBackendFlags_HasMouseCursors;    // We can honor GetMouseCursor() values
    //io.BackendFlags |= ImGuiBackendFlags_HasSetMousePos;     // io.WantSetMousePos requests (Not supported on Wayland or Android. Don't use.)
    io.BackendPlatformName = "imgui_impl_vkWindow";

    KeyMap[eKEY_NONE]         = ImGuiKey_None;
    KeyMap[eKEY_A]            = ImGuiKey_A;
    KeyMap[eKEY_B]            = ImGuiKey_B;
    KeyMap[eKEY_C]            = ImGuiKey_C;
    KeyMap[eKEY_D]            = ImGuiKey_D;
    KeyMap[eKEY_E]            = ImGuiKey_E;
    KeyMap[eKEY_F]            = ImGuiKey_F;
    KeyMap[eKEY_G]            = ImGuiKey_G;
    KeyMap[eKEY_H]            = ImGuiKey_H;
    KeyMap[eKEY_I]            = ImGuiKey_I;
    KeyMap[eKEY_J]            = ImGuiKey_J;
    KeyMap[eKEY_K]            = ImGuiKey_K;
    KeyMap[eKEY_L]            = ImGuiKey_L;
    KeyMap[eKEY_M]            = ImGuiKey_M;
    KeyMap[eKEY_N]            = ImGuiKey_N;
    KeyMap[eKEY_O]            = ImGuiKey_O;
    KeyMap[eKEY_P]            = ImGuiKey_P;
    KeyMap[eKEY_Q]            = ImGuiKey_Q;
    KeyMap[eKEY_R]            = ImGuiKey_R;
    KeyMap[eKEY_S]            = ImGuiKey_S;
    KeyMap[eKEY_T]            = ImGuiKey_T;
    KeyMap[eKEY_U]            = ImGuiKey_U;
    KeyMap[eKEY_V]            = ImGuiKey_V;
    KeyMap[eKEY_W]            = ImGuiKey_W;
    KeyMap[eKEY_X]            = ImGuiKey_X;
    KeyMap[eKEY_Y]            = ImGuiKey_Y;
    KeyMap[eKEY_Z]            = ImGuiKey_Z;
    KeyMap[eKEY_1]            = ImGuiKey_1;
    KeyMap[eKEY_2]            = ImGuiKey_2;
    KeyMap[eKEY_3]            = ImGuiKey_3;
    KeyMap[eKEY_4]            = ImGuiKey_4;
    KeyMap[eKEY_5]            = ImGuiKey_5;
    KeyMap[eKEY_6]            = ImGuiKey_6;
    KeyMap[eKEY_7]            = ImGuiKey_7;
    KeyMap[eKEY_8]            = ImGuiKey_8;
    KeyMap[eKEY_9]            = ImGuiKey_9;
    KeyMap[eKEY_0]            = ImGuiKey_0;
    KeyMap[eKEY_Enter]        = ImGuiKey_Enter;
    KeyMap[eKEY_Escape]       = ImGuiKey_Escape;
    KeyMap[eKEY_Delete]       = ImGuiKey_Backspace;
    KeyMap[eKEY_Tab]          = ImGuiKey_Tab;
    KeyMap[eKEY_Space]        = ImGuiKey_Space;
    KeyMap[eKEY_Minus]        = ImGuiKey_Minus;
    KeyMap[eKEY_Equals]       = ImGuiKey_Equal;
    KeyMap[eKEY_LeftBracket]  = ImGuiKey_LeftBracket;
    KeyMap[eKEY_RightBracket] = ImGuiKey_RightBracket;
    KeyMap[eKEY_Backslash]    = ImGuiKey_Backslash;
    KeyMap[eKEY_Semicolon]    = ImGuiKey_Semicolon;
    KeyMap[eKEY_Quote]        = ImGuiKey_Apostrophe;
    KeyMap[eKEY_Grave]        = ImGuiKey_GraveAccent;
    KeyMap[eKEY_Comma]        = ImGuiKey_Comma;
    KeyMap[eKEY_Period]       = ImGuiKey_Period;
    KeyMap[eKEY_Slash]        = ImGuiKey_Slash;
    KeyMap[eKEY_CapsLock]     = ImGuiKey_CapsLock;
    KeyMap[eKEY_F1]           = ImGuiKey_F1;
    KeyMap[eKEY_F2]           = ImGuiKey_F2;
    KeyMap[eKEY_F3]           = ImGuiKey_F3;
    KeyMap[eKEY_F4]           = ImGuiKey_F4;
    KeyMap[eKEY_F5]           = ImGuiKey_F5;
    KeyMap[eKEY_F6]           = ImGuiKey_F6;
    KeyMap[eKEY_F7]           = ImGuiKey_F7;
    KeyMap[eKEY_F8]           = ImGuiKey_F8;
    KeyMap[eKEY_F9]           = ImGuiKey_F9;
    KeyMap[eKEY_F10]          = ImGuiKey_F10;
    KeyMap[eKEY_F11]          = ImGuiKey_F11;
    KeyMap[eKEY_F12]          = ImGuiKey_F12;
    KeyMap[eKEY_PrintScreen]  = ImGuiKey_PrintScreen;
    KeyMap[eKEY_ScrollLock]   = ImGuiKey_ScrollLock;
    KeyMap[eKEY_Pause]        = ImGuiKey_Pause;
    KeyMap[eKEY_Insert]       = ImGuiKey_Insert;
    KeyMap[eKEY_Home]         = ImGuiKey_Home;
    KeyMap[eKEY_PageUp]       = ImGuiKey_PageUp;
    KeyMap[eKEY_DeleteForward]= ImGuiKey_Delete;
    KeyMap[eKEY_End]          = ImGuiKey_End;
    KeyMap[eKEY_PageDown]     = ImGuiKey_PageDown;
    KeyMap[eKEY_Right]        = ImGuiKey_RightArrow;
    KeyMap[eKEY_Left]         = ImGuiKey_LeftArrow;
    KeyMap[eKEY_Down]         = ImGuiKey_DownArrow;
    KeyMap[eKEY_Up]           = ImGuiKey_UpArrow;
    KeyMap[eKP_NumLock]       = ImGuiKey_NumLock;
    KeyMap[eKP_Divide]        = ImGuiKey_KeypadDivide;
    KeyMap[eKP_Multiply]      = ImGuiKey_KeypadMultiply;
    KeyMap[eKP_Subtract]      = ImGuiKey_KeypadSubtract;
    KeyMap[eKP_Add]           = ImGuiKey_KeypadAdd;
    KeyMap[eKP_Enter]         = ImGuiKey_KeypadEnter;
    KeyMap[eKP_1]             = ImGuiKey_Keypad1;
    KeyMap[eKP_2]             = ImGuiKey_Keypad2;
    KeyMap[eKP_3]             = ImGuiKey_Keypad3;
    KeyMap[eKP_4]             = ImGuiKey_Keypad4;
    KeyMap[eKP_5]             = ImGuiKey_Keypad5;
    KeyMap[eKP_6]             = ImGuiKey_Keypad6;
    KeyMap[eKP_7]             = ImGuiKey_Keypad7;
    KeyMap[eKP_8]             = ImGuiKey_Keypad8;
    KeyMap[eKP_9]             = ImGuiKey_Keypad9;
    KeyMap[eKP_0]             = ImGuiKey_Keypad0;
    KeyMap[eKP_Point]         = ImGuiKey_KeypadDecimal;
    KeyMap[eKEY_F13]          = ImGuiKey_F13;
    KeyMap[eKEY_F14]          = ImGuiKey_F14;
    KeyMap[eKEY_F15]          = ImGuiKey_F15;
    KeyMap[eKEY_F16]          = ImGuiKey_F16;
    KeyMap[eKEY_F17]          = ImGuiKey_F17;
    KeyMap[eKEY_F18]          = ImGuiKey_F18;
    KeyMap[eKEY_F19]          = ImGuiKey_F19;
    KeyMap[eKEY_F20]          = ImGuiKey_F20;
    KeyMap[eKEY_F21]          = ImGuiKey_F21;
    KeyMap[eKEY_F22]          = ImGuiKey_F22;
    KeyMap[eKEY_F23]          = ImGuiKey_F23;
    KeyMap[eKEY_F24]          = ImGuiKey_F24;
    KeyMap[eKEY_Menu]         = ImGuiKey_Menu;
    KeyMap[eKEY_LeftControl]  = ImGuiKey_LeftCtrl;
    KeyMap[eKEY_LeftShift]    = ImGuiKey_LeftShift;
    KeyMap[eKEY_LeftAlt]      = ImGuiKey_LeftAlt;
    KeyMap[eKEY_LeftGUI]      = ImGuiKey_LeftSuper;
    KeyMap[eKEY_RightControl] = ImGuiKey_RightCtrl;
    KeyMap[eKEY_RightShift]   = ImGuiKey_RightShift;
    KeyMap[eKEY_RightAlt]     = ImGuiKey_RightAlt;
    KeyMap[eKEY_RightGUI]     = ImGuiKey_RightSuper;

    io.SetClipboardTextFn = ImGui_ImplvkWindow_SetClipboardText;
    io.GetClipboardTextFn = ImGui_ImplvkWindow_GetClipboardText;
    io.ClipboardUserData = g_window;
#if defined(_WIN32)
    //io.ImeWindowHandle = (void*)glfwGetWin32Window(g_Window);
#endif

    return true;
}

void ImGui_ImplvkWindow_Shutdown() {
    g_window = nullptr;
}

void ImGui_ImplvkWindow_UpdateMouse(uint8_t btns, int16_t x, int16_t y) {
    // Update buttons
    ImGuiIO& io = ImGui::GetIO();
    io.MouseDown[0] = btns & 1;  // left button
    io.MouseDown[1] = btns & 4;  // middle button
    io.MouseDown[2] = btns & 2;  // right button

    // Update mouse position
    if (!io.WantSetMousePos) {
        io.MousePos = ImVec2((float)x / g_scale, (float)y / g_scale);
    }
}

void ImGui_ImplvkWindow_ScrollWheel(VkWindow* window, float xoffset, float yoffset) {
    ImGuiIO& io = ImGui::GetIO();
    io.MouseWheelH += xoffset;
    io.MouseWheel += yoffset;
}

void ImGui_ImplvkWindow_KeyPressed(VkWindow* window, int keycode, int action) {
    ImGuiIO& io = ImGui::GetIO();
    bool down = (action==(int)eDOWN);
    ImGuiKey scancode = KeyMap[keycode];
    io.AddKeyEvent(scancode, down);

    if((keycode == eKEY_LeftControl)||(keycode == eKEY_RightControl)) io.AddKeyEvent(ImGuiMod_Ctrl,  down);
    if((keycode == eKEY_LeftShift)  ||(keycode == eKEY_RightShift)  ) io.AddKeyEvent(ImGuiMod_Shift, down);
    if((keycode == eKEY_LeftAlt)    ||(keycode == eKEY_RightAlt)    ) io.AddKeyEvent(ImGuiMod_Alt,   down);
    if((keycode == eKEY_LeftGUI)    ||(keycode == eKEY_RightGUI)    ) io.AddKeyEvent(ImGuiMod_Super, down);
}

void ImGui_ImplvkWindow_TextInput(VkWindow* window, const char* str) {
    ImGuiIO& io = ImGui::GetIO();
    unsigned int chr = str[0];
    io.AddInputCharacter(chr);
}
/*
// Similar to GetTickCount, but portable.
// It rolls over every ~12.1 days (0x100000/24/60/60)
uint GetMilliCount() {  // wall time
  timeb tb;
  ftime(&tb);
  return (uint)(tb.millitm+tb.time*1000);
}

float cpu_delta_time() {  // returns CPU time in seconds, since last call
    static clock_t last = 0;
           clock_t curr = clock();
    uint32_t delta = curr - last;
    last = curr;
    return delta / (float)CLOCKS_PER_SEC;
};

float wall_delta_time() {  // returns wall clock time in seconds, since last call (milli-second res)
    static uint last = 0;
           uint curr = GetMilliCount();
     uint32_t delta = curr - last;
     last = curr;
     return delta / 1000.f;
};
*/

double wall_delta_time_hires() {  // returns wall clock time in seconds, since last call (nano-second res)
    using namespace std::chrono;
    static high_resolution_clock::time_point last = high_resolution_clock::now();
    high_resolution_clock::time_point curr = high_resolution_clock::now();
    duration<double> time_span = duration_cast<duration<double>>(curr - last);
    last = curr;
    return time_span.count();
}

float ActualFramerate;  // Wall-clock framerate average over last 120 frames

void ImGui_ImplvkWindow_NewFrame() {
    ImGuiIO& io = ImGui::GetIO();
    IM_ASSERT(io.Fonts->IsBuilt() && "Font atlas not built! It is generally built by the renderer back-end. Missing call to renderer _NewFrame() function? e.g. ImGui_ImplOpenGL3_NewFrame().");

    // Setup display size (every frame to accommodate for window resizing)
    int w, h;
    g_scale = g_window->getScale();
    g_window->getSize(w, h);
    io.DisplaySize = ImVec2((float)w, (float)h);
    if (w > 0 && h > 0) io.DisplayFramebufferScale = ImVec2(g_scale ,g_scale);

    //float delta =  max(cpu_delta_time(), 0.0001);
    //float delta =  max(wall_delta_time(), 0.0001);
    float delta =  max((float)wall_delta_time_hires(), 0.0001f);

    io.DeltaTime = delta;
    //printf("%f\n", delta);

    //---Get average framerate over last 120 frames---
    const int count = 120;
    static int ofs = 0;
    static float ftimes[count]{};
    ftimes[ofs++] = delta;
    ofs = ofs % count;

    float rate = 0;
    for (uint32_t i=0; i<count; ++i) rate += ftimes[i];
    //repeat(120) rate += ftimes[i];
    rate = rate / float(count);
    rate = 1.f / rate;
    //printf("%f\n", frate);
    //io.ActualFramerate = rate;
    ActualFramerate = rate;
    //------------------------------------------------

    ImGui_ImplvkWindow_UpdateMouseCursor();
    ImGui_ImplvkWindow_UpdateGamepads();

    //ImGui_ImplGlfw_UpdateMousePosAndButtons();  //TODO
}

void ImGui_ImplvkWindow_UpdateMouseCursor() {
    ImGuiIO& io = ImGui::GetIO();
    ImGuiConfigFlags flags = io.ConfigFlags;
    if(flags & ImGuiConfigFlags_NoMouseCursorChange) return;

    eCursor cursor = eArrow;
    ImGuiMouseCursor cursor_type = ImGui::GetMouseCursor();
    switch (cursor_type) {
        case ImGuiMouseCursor_Arrow:      cursor = eArrow;      break;
        case ImGuiMouseCursor_TextInput:  cursor = eCaret;      break;
        case ImGuiMouseCursor_ResizeAll:  cursor = eResizeAll;  break;
        case ImGuiMouseCursor_ResizeNS:   cursor = eResizeNS;   break;
        case ImGuiMouseCursor_ResizeEW:   cursor = eResizeEW;   break;
        case ImGuiMouseCursor_ResizeNESW: cursor = eResizeNESW; break;
        case ImGuiMouseCursor_ResizeNWSE: cursor = eResizeNWSE; break;
        case ImGuiMouseCursor_Hand:       cursor = eHand;       break;
        case ImGuiMouseCursor_Wait:       cursor = eWait;       break;
        case ImGuiMouseCursor_Progress:   cursor = eProgress;   break;
        case ImGuiMouseCursor_NotAllowed: cursor = eNotAllowed; break;
        default: cursor = eArrow;
    }
    g_window->setCursor(cursor);
}

void ImGui_ImplvkWindow_UpdateGamepads() {
    ImGuiIO& io = ImGui::GetIO();
    if ((io.ConfigFlags & ImGuiConfigFlags_NavEnableGamepad) == 0) return;
    Gamepad& pad = g_window->getGamepad(0);
    if(pad.active==false) return;

    // Buttons
    io.AddKeyEvent(ImGuiKey_GamepadFaceDown,  pad.buttons[eBTN_A]);       // A
    io.AddKeyEvent(ImGuiKey_GamepadFaceRight, pad.buttons[eBTN_B]);       // B
    io.AddKeyEvent(ImGuiKey_GamepadFaceLeft,  pad.buttons[eBTN_X]);       // X
    io.AddKeyEvent(ImGuiKey_GamepadFaceUp,    pad.buttons[eBTN_Y]);       // Y
    io.AddKeyEvent(ImGuiKey_GamepadL1,        pad.buttons[eBTN_TL]);      // Shoulder Left
    io.AddKeyEvent(ImGuiKey_GamepadR1,        pad.buttons[eBTN_TR]);      // Shoulder Right
    io.AddKeyEvent(ImGuiKey_GamepadL3,        pad.buttons[eBTN_THUMBL]);  // Thumb Left
    io.AddKeyEvent(ImGuiKey_GamepadR3,        pad.buttons[eBTN_THUMBR]);  // Thumb Right
    io.AddKeyEvent(ImGuiKey_GamepadDpadUp,    pad.buttons[eDPAD_UP]);     // DPad Up
    io.AddKeyEvent(ImGuiKey_GamepadDpadDown,  pad.buttons[eDPAD_DOWN]);   // DPad Down
    io.AddKeyEvent(ImGuiKey_GamepadDpadLeft,  pad.buttons[eDPAD_LEFT]);   // DPad Left
    io.AddKeyEvent(ImGuiKey_GamepadDpadRight, pad.buttons[eDPAD_RIGHT]);  // DPad Right
    io.AddKeyEvent(ImGuiKey_GamepadBack,      pad.buttons[eBTN_SELECT]);  // Back
    io.AddKeyEvent(ImGuiKey_GamepadStart,     pad.buttons[eBTN_START]);   // Start

    // Axes
    float LX = pad.axes[eAXIS_LX];
    float LY = pad.axes[eAXIS_LY];
    float RX = pad.axes[eAXIS_RX];
    float RY = pad.axes[eAXIS_RY];
    float TL = pad.axes[eAXIS_TL];
    float TR = pad.axes[eAXIS_TR];

    const float DEADZONE = 0.3f;

    // Left Stick
    io.AddKeyAnalogEvent(ImGuiKey_GamepadLStickLeft,  LX < -DEADZONE, -LX * (LX < 0));
    io.AddKeyAnalogEvent(ImGuiKey_GamepadLStickRight, LX >  DEADZONE,  LX * (LX > 0));
    io.AddKeyAnalogEvent(ImGuiKey_GamepadLStickDown,  LY < -DEADZONE, -LY * (LY < 0));
    io.AddKeyAnalogEvent(ImGuiKey_GamepadLStickUp,    LY >  DEADZONE,  LY * (LY > 0));

    // Right Stick
    io.AddKeyAnalogEvent(ImGuiKey_GamepadRStickLeft,  RX < -DEADZONE, -RX * (RX < 0));
    io.AddKeyAnalogEvent(ImGuiKey_GamepadRStickRight, RX >  DEADZONE,  RX * (RX > 0));
    io.AddKeyAnalogEvent(ImGuiKey_GamepadRStickDown,  RY < -DEADZONE, -RY * (RY < 0));
    io.AddKeyAnalogEvent(ImGuiKey_GamepadRStickUp,    RY >  DEADZONE,  RY * (RY > 0));

    // Triggers
    io.AddKeyAnalogEvent(ImGuiKey_GamepadL2, TL > 0.1f, TL);
    io.AddKeyAnalogEvent(ImGuiKey_GamepadR2, TR > 0.1f, TR);
}
