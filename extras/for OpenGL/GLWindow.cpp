// GLWindow.cpp
#include "GLWindow.h"
#include <cstdio>

bool GLWindow::InitEGL(int glMajor, int glMinor, bool gles) {
    // 1. Get native display/handle
    native_handle* nh = getNativeHandle();

#ifdef _WIN32
    eglDisplay = eglGetDisplay(nh->hDC ? nh->hDC : GetDC(nh->hWnd));
#elif __ANDROID__
    eglDisplay = eglGetDisplay(EGL_DEFAULT_DISPLAY);
    // ANativeWindow will be set later with eglCreateWindowSurface
#elif __linux__
    eglDisplay = eglGetDisplay((EGLNativeDisplayType)nh->display);
#endif

    if (eglDisplay == EGL_NO_DISPLAY) return false;

    EGLint major, minor;
    if (!eglInitialize(eglDisplay, &major, &minor)) return false;

    // 2. Choose config
    EGLint attribs[] = {
        EGL_RENDERABLE_TYPE, gles ? EGL_OPENGL_ES2_BIT : EGL_OPENGL_BIT,
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
    eglSurface = eglCreateWindowSurface(eglDisplay, eglConfig, (EGLNativeWindowType)nh->xcb_window, nullptr);
#endif
    if (eglSurface == EGL_NO_SURFACE) return false;

    // 4. Create context
    EGLint contextAttribs[16]{EGL_CONTEXT_MAJOR_VERSION, glMajor,
                              EGL_CONTEXT_MINOR_VERSION, glMinor,
                              EGL_NONE};
    //if(gles)contextAttribs[2]=EGL_NONE;

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
