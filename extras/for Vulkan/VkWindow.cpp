//#include "Logging.h"
//#include "Validation.h"
#include "VkWindow.h"

//----------------------------------------------------------------------
#ifndef VKERRCHECK
#ifdef NDEBUG
    #define LOGI(...) {}
    #define LOGE(...) {}
    #define VKERRCHECK(VKFN) { (void)VKFN; }
#else
    #define LOGI(...) {printf("INFO : " ); printf(__VA_ARGS__);}
    #define LOGE(...) {printf("ERROR : "); printf(__VA_ARGS__);}
    #define VKERRCHECK(VKFN) {    \
        VkResult VKRESULT = VKFN; \
        if(VKRESULT != VK_SUCCESS) {printf("Error(%d): %s\n", VKRESULT, #VKFN); exit(0);}  \
     }
     #endif
 #endif
//----------------------------------------------------------------------

VkWindow::VkWindow(const char *title, int width, int height) : GWindow(title, width, height) {}

VkWindow::~VkWindow() {
    if(vkSurface) vkDestroySurfaceKHR(vkInstance, vkSurface, nullptr); vkSurface = nullptr;
    LOGI("Vulkan Surface destroyed\n");
}

VkSurfaceKHR VkWindow::CreateVkSurface(VkInstance instance) {
    if(vkSurface) return vkSurface;
    vkInstance = instance;
#ifdef WIN32
    struct native_handle {
        HINSTANCE hInstance;
        HWND hWnd;
    };
    //native_handle* hnd =(native_handle*)m_wsiWindow.GetNativeHandle();
    native_handle* hnd =(native_handle*)getNativeHandle();
    VkWin32SurfaceCreateInfoKHR win32_createInfo;
    win32_createInfo.sType     = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
    win32_createInfo.pNext     = NULL;
    win32_createInfo.flags     = 0;
    win32_createInfo.hinstance = hnd->hInstance;
    win32_createInfo.hwnd      = hnd->hWnd;
    VKERRCHECK(vkCreateWin32SurfaceKHR(instance, &win32_createInfo, NULL, &vkSurface));
#elif __ANDROID__
    ANativeWindow* hnd = (ANativeWindow*)getNativeHandle();
    VkAndroidSurfaceCreateInfoKHR android_createInfo {VK_STRUCTURE_TYPE_ANDROID_SURFACE_CREATE_INFO_KHR};
    android_createInfo.pNext  = NULL;
    android_createInfo.flags  = 0;
    android_createInfo.window = hnd;  //app->window;
    VKERRCHECK(vkCreateAndroidSurfaceKHR(instance, &android_createInfo, NULL, &vkSurface));
#elif __linux__
    struct native_handle {
        xcb_connection_t* xcb_connection;
        xcb_screen_t* xcb_screen;
        xcb_window_t xcb_window;
    };

    native_handle* hnd =(native_handle*)getNativeHandle();
    VkXcbSurfaceCreateInfoKHR xcb_createInfo;
    xcb_createInfo.sType      = VK_STRUCTURE_TYPE_XCB_SURFACE_CREATE_INFO_KHR;
    xcb_createInfo.pNext      = nullptr;
    xcb_createInfo.flags      = 0;
    xcb_createInfo.connection = hnd->xcb_connection;
    xcb_createInfo.window     = hnd->xcb_window;
    VKERRCHECK(vkCreateXcbSurfaceKHR(instance, &xcb_createInfo, NULL, &vkSurface));
#else 
    LOGE("Unsupported platform\n");
    return 0;
#endif
    LOGI("Vulkan Surface created\n");
    return vkSurface;
}

bool VkWindow::CanPresent(VkPhysicalDevice gpu, uint32_t queue_family) const {
    // If surface was created, use this method
    if(vkSurface) {
        VkBool32 can_present = false;
        VKERRCHECK(vkGetPhysicalDeviceSurfaceSupportKHR(gpu, queue_family, vkSurface, &can_present));
        return !!can_present;
    }

    // If no surface was created, use native handle instead
#ifdef WIN32
    return vkGetPhysicalDeviceWin32PresentationSupportKHR(gpu, queue_family) == VK_TRUE;
#elif __ANDROID__
    return true;  // There's no vkGetPhysicalDeviceAndroidSupportKHR. Just assume all queue families can present
#elif __linux__
    struct native_handle {
        xcb_connection_t* xcb_connection;
        xcb_screen_t* xcb_screen;
        xcb_window_t xcb_window;
    };

    native_handle* hnd =(native_handle*)getNativeHandle();
    return vkGetPhysicalDeviceXcbPresentationSupportKHR(gpu, queue_family, hnd->xcb_connection, hnd->xcb_screen->root_visual) == VK_TRUE;
#endif
  //return false;
}

