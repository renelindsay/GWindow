// dear imgui: Platform Binding for GWindow
//
// GWindow is a cross-platform library for creating a window for graphics rendering.
// It provides input event hooks for keyboard, mouse, touch-screen and gamepad events,
// and works on Windows, Linux and Android. Bring your own graphics renderer. (Vulkan/OpenGL/pixbuf)

// You can copy and use unmodified imgui_impl_* files in your project. See main.cpp for an example of using this.
// If you are new to dear imgui, read examples/README.txt and read the documentation at the top of imgui.cpp.
// https://github.com/ocornut/imgui

// TODO: IME support
// TODO: SetMousePos

#pragma once

#include "imgui.h"

//class GWindow;

IMGUI_IMPL_API bool ImGui_ImplgWindow_Init(GWindow* window);
IMGUI_IMPL_API void ImGui_ImplgWindow_Shutdown();
IMGUI_IMPL_API void ImGui_ImplgWindow_NewFrame();

extern float ActualFramerate;  // Wall-clock framerate average over last 120 frames (io.framerate gives cpu-time)

void ImGui_ImplgWindow_UpdateMouse(uint8_t btns, int16_t x, int16_t y);
void ImGui_ImplgWindow_ScrollWheel(GWindow* window, float xoffset, float yoffset);
void ImGui_ImplgWindow_KeyPressed(GWindow* window, int keycode, int action);
void ImGui_ImplgWindow_TextInput(GWindow* window, const char* str);
void ImGui_ImplgWindow_UpdateMouseCursor();
void ImGui_ImplgWindow_UpdateGamepads();
