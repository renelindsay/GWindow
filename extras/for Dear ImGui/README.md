# Dear ImGui Backend

This folder contains the `Dear_ImGui` platform binding for `GWindow`.

`Dear_ImGui` is a bloat-free Graphical User Interface for C++, and pairs very nicely with `GWindow`.  
In fact, several `GWindow` features were specifically added and tested for use with `Dear_ImGui`.  

`GWindow` provides the input events from keyboard, mouse, touch-screen and gamepad events.  
Simply pass these events on to ImGui, to make ImGui work on Windows, Linux and Android.  
Now you just need to bring your own graphics renderer. (Vulkan/OpenGL/pixbuf)

For a working example, see: [vkSamples: (05_ImGui)](https://github.com/renelindsay/vkSamples)

## Dependencies:

[Dear ImGui](https://github.com/ocornut/imgui) is a lightweight GUI toolkit which works with GWindow.