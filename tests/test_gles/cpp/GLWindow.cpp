// GLWindow.cpp
#include "GLWindow.h"
//#include <cstdio>

bool GLWindow::InitEGL(int glMajor, int glMinor) {
    // 1. Get native display/handle
    native_handle* nh = getNativeHandle();

#ifdef _WIN32
    eglDisplay = eglGetDisplay(GetDC(nh->hWnd));
#elif __ANDROID__
    eglDisplay = eglGetDisplay(EGL_DEFAULT_DISPLAY);
#elif __linux__
    #ifdef   VK_USE_PLATFORM_XCB_KHR
        //eglDisplay = eglGetDisplay((EGLNativeDisplayType)nh->display);                        // for xlib
        eglDisplay = eglGetPlatformDisplay(EGL_PLATFORM_XCB_EXT, nh->xcb_connection, nullptr);  // for xcb

    #elifdef VK_USE_PLATFORM_WAYLAND_KHR
        eglDisplay = eglGetPlatformDisplay(EGL_PLATFORM_WAYLAND_EXT, nh->display, nullptr); // for wayland
    #endif

#endif

    if (eglDisplay == EGL_NO_DISPLAY) return false;

    EGLint major, minor;
    if (!eglInitialize(eglDisplay, &major, &minor)) return false;

    // 2. Choose config
    EGLint attribs[] = {
        EGL_RENDERABLE_TYPE, EGL_OPENGL_ES2_BIT,
        EGL_SURFACE_TYPE,    EGL_WINDOW_BIT,
        EGL_RED_SIZE,        8,
        EGL_GREEN_SIZE,      8,
        EGL_BLUE_SIZE,       8,
        EGL_ALPHA_SIZE,      8,
        EGL_DEPTH_SIZE,      24,
        EGL_STENCIL_SIZE,    8,
        EGL_NONE
    };

    EGLint numConfigs;
    eglChooseConfig(eglDisplay, attribs, &eglConfig, 1, &numConfigs);
    if (numConfigs == 0) return false;

    // 3. Create window surface
#ifdef _WIN32
    eglSurface = eglCreateWindowSurface(eglDisplay, eglConfig, nh->hWnd, nullptr);
#elif __ANDROID__
    eglSurface = eglCreateWindowSurface(eglDisplay, eglConfig, nh->window, nullptr);
#elif __linux__
    //eglSurface = eglCreateWindowSurface(eglDisplay, eglConfig, (EGLNativeWindowType)nh->xcb_window, nullptr);
#endif

#ifdef VK_USE_PLATFORM_XCB_KHR
    eglSurface = eglCreateWindowSurface(eglDisplay, eglConfig, (EGLNativeWindowType)nh->xcb_window, nullptr);
#elif  VK_USE_PLATFORM_WAYLAND_KHR
    eglSurface = eglCreateWindowSurface(eglDisplay, eglConfig, (EGLNativeWindowType)nh->egl_window, nullptr);
#endif


    if (eglSurface == EGL_NO_SURFACE) return false;

    // 4. Create context    
    EGLint contextAttribs[]{EGL_CONTEXT_MAJOR_VERSION, glMajor, EGL_NONE};
    eglContext = eglCreateContext(eglDisplay, eglConfig, EGL_NO_CONTEXT, contextAttribs);
    if (eglContext == EGL_NO_CONTEXT) return false;

    MakeCurrent();
    return true;
}

void GLWindow::MakeCurrent() {
    eglMakeCurrent(eglDisplay, eglSurface, eglSurface, eglContext);
}

void GLWindow::SwapBuffers() {
    eglSwapBuffers(eglDisplay, eglSurface);
}

void GLWindow::DestroyEGL() {
    if (eglDisplay != EGL_NO_DISPLAY) {
        eglMakeCurrent(eglDisplay, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
        if (eglContext)  eglDestroyContext(eglDisplay, eglContext);
        if (eglSurface)  eglDestroySurface(eglDisplay, eglSurface);
        eglTerminate(eglDisplay);
    }
    eglDisplay = EGL_NO_DISPLAY;
    eglContext = EGL_NO_CONTEXT;
    eglSurface = EGL_NO_SURFACE;
}
