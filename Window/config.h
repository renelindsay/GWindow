#pragma once

#ifdef _WIN32
    #define VK_USE_PLATFORM_WIN32_KHR
#elif  __ANDROID__
    #define VK_USE_PLATFORM_ANDROID_KHR
#elif  __linux__
    #define VK_USE_PLATFORM_XCB_KHR
#endif

#define ENABLE_MULTITOUCH
#define ENABLE_GAMEPAD
#define ENABLE_CLIPBOARD
#define ENABLE_SHOWIMAGE
#define ENABLE_FULLSCREEN
#define ENABLE_CURSOR
