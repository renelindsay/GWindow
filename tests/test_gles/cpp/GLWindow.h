// GLWindow.h
#pragma once
#include "Window.h"     // GWindow base
#include <EGL/egl.h>
#include <EGL/eglext.h>

class GLWindow : public GWindow {
protected:
    EGLDisplay eglDisplay = EGL_NO_DISPLAY;
    EGLContext eglContext = EGL_NO_CONTEXT;
    EGLSurface eglSurface = EGL_NO_SURFACE;
    EGLConfig  eglConfig  = nullptr;

public:
    // Call after window is created, before any GL calls
    bool InitEGL(int glMajor = 3, int glMinor = 1);

    void MakeCurrent();
    void SwapBuffers();
    void DestroyEGL();

    EGLDisplay GetEGLDisplay() const { return eglDisplay; }
    EGLContext GetEGLContext() const { return eglContext; }
    EGLSurface GetEGLSurface() const { return eglSurface; }
};
