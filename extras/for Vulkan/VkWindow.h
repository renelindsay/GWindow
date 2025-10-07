#ifndef VK_WINDOW_H
#define VK_WINDOW_H

#include "config.h"
#include "vexel.h"  // VEXEL: https://github.com/renelindsay/Vexel
#include "Window.h"

class VkWindow : public GWindow {
    VkInstance vkInstance = nullptr;
protected:
    VkSurfaceKHR vkSurface = nullptr;
public:
    VkWindow(const char *title="VkWindow", int width=640, int height=480);
    virtual ~VkWindow();

    VkSurfaceKHR CreateVkSurface(VkInstance instance);
    bool CanPresent(VkPhysicalDevice gpu, uint32_t queue_family) const;  // Checks if surface can present given queue type.
};

#endif
