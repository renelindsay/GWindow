# gwindow.cmake - CMake helper for gwindow single-header library
#
# Usage:
#   add_executable(myapp main.cpp)
#   include("path/to/GWindow.cmake")
#   add_gwindow(myapp)
#
# Linux options:
#   -DGWINDOW_USE_X11=ON
#   -DGWINDOW_USE_X11=OFF (default)

option(GWINDOW_USE_X11 "Use the X11/XCB backend instead of Wayland" OFF)

function(add_gwindow target)
    if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
        find_package(PkgConfig REQUIRED)

        if(GWINDOW_USE_X11)  # X11/XCB backend
            # X11 and related dependencies
            find_package(X11 REQUIRED)
            target_include_directories(${target} PUBLIC ${X11_INCLUDE_DIR})
            target_link_libraries(${target} PUBLIC
                ${X11_LIBRARIES}
                ${X11_xcb_LIB}
                ${X11_X11_xcb_LIB}
                ${X11_xkbcommon_LIB}
                ${X11_Xinput_LIB}
                ${X11_xcb_image_LIB}
                ${X11_xcb_cursor_LIB}
            )
            target_compile_definitions(${target} PUBLIC VK_USE_PLATFORM_XCB_KHR)
        else()  # Wayland
            pkg_check_modules(WAYLAND REQUIRED IMPORTED_TARGET wayland-client wayland-cursor wayland-egl libdecor-0 xkbcommon)
            target_link_libraries(${target} PUBLIC PkgConfig::WAYLAND)
            target_compile_definitions(${target} PUBLIC VK_USE_PLATFORM_WAYLAND_KHR)
        endif()

        # libevdev for gamepad support
        pkg_check_modules(LIBEVDEV REQUIRED IMPORTED_TARGET libevdev)
        target_link_libraries(${target} PUBLIC PkgConfig::LIBEVDEV)

    elseif(CMAKE_SYSTEM_NAME STREQUAL "Windows")
        # Windows-specific libs  (eg. user32 gdi32 winmm)
        target_link_libraries(${target} PUBLIC xinput9_1_0) # for Gamepad
        target_compile_definitions(${target} PUBLIC VK_USE_PLATFORM_WIN32_KHR)

    elseif(CMAKE_SYSTEM_NAME STREQUAL "Android")
        # android_native_app_glue
        add_library(app_glue STATIC ${ANDROID_NDK}/sources/android/native_app_glue/android_native_app_glue.c)    
        target_include_directories(app_glue PUBLIC  ${ANDROID_NDK}/sources/android/native_app_glue)
    
        # Android-specific dependencies
        target_link_libraries(${target} PUBLIC android log app_glue)
        target_compile_definitions(${target} PUBLIC VK_USE_PLATFORM_ANDROID_KHR)
    endif()
endfunction()

