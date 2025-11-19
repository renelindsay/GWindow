// main.cpp
#include "GLWindow.h"
#include <cmath>
#include <chrono>

using namespace std::chrono;

typedef void (*PFNGLCLEARCOLORPROC)(float r, float g, float b, float a);
typedef void (*PFNGLCLEARPROC)(unsigned int mask);

PFNGLCLEARPROC      glClear      = nullptr;
PFNGLCLEARCOLORPROC glClearColor = nullptr;

constexpr unsigned int GL_COLOR_BUFFER_BIT = 0x00004000;

int main() {
    glClearColor = (PFNGLCLEARCOLORPROC)eglGetProcAddress("glClearColor");
    glClear      = (PFNGLCLEARPROC)     eglGetProcAddress("glClear");

    GLWindow window;
    window.setTitle("Minimal OpenGLES App - GLWindow + EGL + GLES");
    window.setSize(1024,768);

    if (!window.InitEGL(3, 2)) { printf("GLES failed.\n"); return -1; }

    eglSwapInterval(window.GetEGLDisplay(), 1);  // V-sync
    auto start = steady_clock::now();            // timer

    while(window.pollEvents()) {
        float t = duration<float>(steady_clock::now() - start).count();
        float r = sinf(t * 1.2f) * 0.5f + 0.5f;
        float g = sinf(t * 1.7f) * 0.5f + 0.5f;
        float b = sinf(t * 2.3f) * 0.5f + 0.5f;

        glClearColor(r, g, b, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        window.SwapBuffers();
    }
    return 0;
}
