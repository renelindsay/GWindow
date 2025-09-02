#pragma warning(disable:4996)  // for fopen

#define GWINDOW_IMPLEMENTATION
#include "GWindow.h"
//#include "vkWindow.h"  // requires vkUtils
//#include "CInstance.h" // requires vkUtils

const char *type[] {"up  ", "down", "move"};  // Action types for mouse, keyboard and touch-screen.

//-- EVENT HANDLERS --
//class MainWindow : public vkWindow {  // With Vulkan (requires vkUtils)
class MainWindow : public GWindow {  // Without Vulkan
    void onMouseEvent (eAction action, int16_t x, int16_t y, uint8_t btn) { printf("Mouse: %s %d x %d Btn:%d\n", type[action], x, y, btn); }
    void onTouchEvent (eAction action, float x, float y, uint8_t id) { printf("Touch: %s %.2f x %.2f id:%d\n", type[action], x, y, id); }
    void onKeyEvent   (eAction action, eKeycode keycode) { printf("Key: %s keycode:%d\n", type[action], keycode); }
    void onTextEvent  (const char *str) { printf("Text: '%s'\n", str); }
    void onMoveEvent  (int16_t x, int16_t y) { printf("Window Move: x=%d y=%d\n", x, y); }
    void onFocusEvent (bool hasFocus) { printf("Focus: %s\n", hasFocus ? "True" : "False"); }
    void onResizeEvent(uint16_t width, uint16_t height) { printf("Window Resize: width=%4d height=%4d\n", width, height); }
    void onGPadConnect(uint8_t pad, bool active){ printf("Gamepad %d %s\n", pad, active?"connected":"disconnected"); }
    void onGPadButton (uint8_t pad, uint8_t btn, bool down){printf("Gamepad %d button %d %s\n", pad, btn, down?"down":"up");}
    void onGPadAxis   (uint8_t pad, uint8_t axis, float val){printf("Gamepad %d axis %d : %.2f\n", pad, axis, val);}
    void onCloseEvent () { printf("Window Closing.\n"); }
};

int main(int argc, char *argv[]) {
    setvbuf(stdout, 0, _IONBF, 0);  // Prevent printf buffering in QtCreator

    //-------------------------Print text file--------------------------
    // Test fopen, and printf to ensure consistent behavior on Windows, Linux and Android.
    // On Android, the assets folder gets included in the APK file, and 
    // fopen uses the Android AssetManager to read from the assets folder.
    // printf prints to the desktop terminal, or the Android logcat.

    FILE* file = fopen("assets/ReadMe.txt", "r");
    if(!file) printf(" File not found: assets/ReadMe.txt\n");
    if(file) { 
        char line[1024] {}; 
        while(fgets(line, 1024, file)) { 
            printf("%s", line);
        } 
        fclose(file); 
    } 
    //------------------------------------------------------------------- 

    uint32_t image[256*256] = {};

    //CInstance instance(true);                                 // Create a Vulkan Instance
    //instance.DebugReport.SetFlags(14);                        // Select validation-message types (see: VkDebugReportFlagsEXT)

    MainWindow window;                                          // Create a window
    window.setScale(0.0f);                                      // Set Window Scale (0 for system default)
    window.setTitle("GWindow (header)");                        // Set the window title
    window.setSize(640, 480);                                   // Set the window size (Desktop)
    window.setPosition(0, 0);                                   // Set the window position to top-left
    window.showKeyboard(true);                                  // Show soft-keyboard (Android)

    //VkSurfaceKHR surface = window.CreateVkSurface(instance);  // Create the Vulkan surface (requires vkUtils)

    while(window.pollEvents()) {                                // Main event loop, runs until window is closed.
        bool key_pressed = window.getKeyState(eKEY_LeftShift);
        if (key_pressed) printf("LEFT SHIFT PRESSED\r");

        // Draw square image
        for(int y=0;y<256;++y) for(int x=0;x<256;++x) {
            uint32_t* pix = &image[x + y*256];
            *pix = (x<<16) + (y<<8) + (rand()&255);
        }
        if (!key_pressed) window.showImage(image,256,256);

        // test fullscreen mode (Desktop only)
        if(window.getKeyState(eKEY_1)) window.setFullscreen(true);
        if(window.getKeyState(eKEY_2)) window.setFullscreen(false);
    }
    return 0;
}
