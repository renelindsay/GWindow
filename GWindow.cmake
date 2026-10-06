# gwindow.cmake - CMake helper for gwindow single-header library
#
# Usage:
#   add_executable(myapp main.cpp)
#   include("path/to/GWindow.cmake")
#   add_gwindow(myapp)
#
# Linux options:
#   -DUSE_WAYLAND=ON
#   -DUSE_WAYLAND=OFF (default: XCB)

option(USE_WAYLAND "Use Wayland instead of XCB on Linux" OFF)

function(add_gwindow target)
    if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
        find_package(PkgConfig REQUIRED)

        if(USE_WAYLAND)
            # Wayland dependencies
            pkg_check_modules(WAYLAND REQUIRED wayland-client wayland-cursor wayland-egl)
            pkg_check_modules(LIBDECOR REQUIRED libdecor-0)
            pkg_check_modules(XKBCOMMON REQUIRED xkbcommon)
            #target_include_directories(${target} PUBLIC ${WAYLAND_INCLUDE_DIRS})
            #target_link_libraries(${target} PUBLIC ${WAYLAND_LIBRARIES})
            #target_compile_definitions(${target} PUBLIC USE_WAYLAND)

            target_include_directories(${target} PUBLIC
                ${WAYLAND_INCLUDE_DIRS}
                ${LIBDECOR_INCLUDE_DIRS}
                ${XKBCOMMON_INCLUDE_DIRS}
            )

            target_link_libraries(${target} PUBLIC
                ${WAYLAND_LIBRARIES}
                ${LIBDECOR_LIBRARIES}
                ${XKBCOMMON_LIBRARIES}
            )

            target_compile_definitions(${target} PUBLIC VK_USE_PLATFORM_WAYLAND_KHR)

        else()  # X11/XCB
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
        endif()

        # libevdev for gamepad support
        pkg_check_modules(LIBEVDEV REQUIRED libevdev)
        target_include_directories(${target} PUBLIC ${LIBEVDEV_INCLUDE_DIRS})
        target_link_libraries(${target} PUBLIC ${LIBEVDEV_LIBRARIES})

    elseif(CMAKE_SYSTEM_NAME STREQUAL "Windows")
        # Windows-specific libs  (eg. user32 gdi32 winmm)
        target_link_libraries(${target} PUBLIC xinput9_1_0) # for Gamepad
        target_compile_definitions(${PROJECT_NAME} PUBLIC VK_USE_PLATFORM_WIN32_KHR)

    elseif(CMAKE_SYSTEM_NAME STREQUAL "Android")
        # android_native_app_glue
        add_library(app_glue STATIC ${ANDROID_NDK}/sources/android/native_app_glue/android_native_app_glue.c)    
        target_include_directories(app_glue PUBLIC  ${ANDROID_NDK}/sources/android/native_app_glue)
    
        # Android-specific dependencies
        target_link_libraries(${target} PUBLIC android log app_glue)
        target_compile_definitions(${PROJECT_NAME} PUBLIC VK_USE_PLATFORM_ANDROID_KHR)
    endif()
endfunction()

