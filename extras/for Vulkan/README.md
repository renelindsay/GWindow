# VkWindow (For Vulkan)

If you plan to use **Vulkan**, use `VkWindow` and `Vexel` to initialize the `VkSurface` for rendering.  
`VkWindow` derives from `GWindow`, and adds the following functions for creating a Vulkan `VkSurface`:  

- `VkSurfaceKHR CreateVkSurface(VkInstance instance);` — Creates a VkSurface for this window.
- `bool CanPresent(VkPhysicalDevice gpu, uint32_t queue_family);` — Checks if surface can present given queue type.

This is easier than using `getNativeHandle()` directly, but if you insist:

## native_handle* getNativeHandle():

The `native_handle* getNativeHandle()` function returns platform specific window handle parameters,  
required by Vulkan WSI, to create a VkSurfaceKHR, so Vulkan can render to the window surface.  

On **Windows**, it returns this struct, for use by `VkWin32SurfaceCreateInfoKHR` and `vkCreateWin32SurfaceKHR()`:

```cpp
struct native_handle {
    HINSTANCE hInstance;
    HWND hWnd;
};
```

On **Linux**, it returns this struct, for use by `VkXcbSurfaceCreateInfoKHR` and `vkCreateXcbSurfaceKHR()`:

```cpp
struct native_handle {
    xcb_connection_t* xcb_connection;
    xcb_screen_t* xcb_screen;
    xcb_window_t xcb_window;
};
```

On **Android**, it returns this struct, for use by `VkAndroidSurfaceCreateInfoKHR` and `vkCreateAndroidSurfaceKHR()`:

```cpp
struct native_handle {
    ANativeWindow* window;
};
```

But like I said, its easier to just use the included `VkWindow` class instead.

## Dependencies:

[Vexel](https://github.com/renelindsay/Vexel) is a Vulkan Extension Loader (recommended)