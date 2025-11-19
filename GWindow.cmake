# gwindow.cmake - CMake helper for gwindow single-header library
#
# Usage:
#   add_executable(myapp main.cpp)
#   include("path/to/GWindow.cmake")
#   add_gwindow(myapp)

function(add_gwindow target)
    if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
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

        # libevdev for gamepad support
        find_package(PkgConfig REQUIRED)
        pkg_check_modules(LIBEVDEV REQUIRED libevdev)
        target_include_directories(${target} PUBLIC ${LIBEVDEV_INCLUDE_DIRS})
        target_link_libraries(${target} PUBLIC ${LIBEVDEV_LIBRARIES})

    elseif(CMAKE_SYSTEM_NAME STREQUAL "Windows")
        # Windows-specific libs  (eg. user32 gdi32 winmm)
        target_link_libraries(${target} PUBLIC xinput9_1_0) # for Gamepad

    elseif(CMAKE_SYSTEM_NAME STREQUAL "Android")
        # android_native_app_glue
        add_library(app_glue STATIC ${ANDROID_NDK}/sources/android/native_app_glue/android_native_app_glue.c)    
        target_include_directories(app_glue PUBLIC  ${ANDROID_NDK}/sources/android/native_app_glue)
    
        # Android-specific dependencies
        target_link_libraries(${target} PUBLIC android log app_glue)
    endif()
endfunction()

