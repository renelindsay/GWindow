/*
*--------------------------------------------------------------------------
*
*  The CWindow class creates a window and main event processing loop.
*  It provides functions for querying the current state of the window, keyboard,
*  and mouse. Also, events may be processed via either polling or callbacks.
*
*  For polling, use the "getEvent" function to return one event at a time,
*  and process, using a case statement.  For an example, see the "processEvents" implementation.
*
*  For callbacks, use the "processEvents" function to dispatch all queued events to their
*  appropriate event handlers.  To create event handlers, derive your class from Window,
*  and override the virtual event handler functions. (See WindowBase.h)
*
*--------------------------------------------------------------------------
*/

// NOTE: GWindow.h MUST be #included BEFORE stdio.h, for printf to work correctly on Android.
// TODO: Add Wayland support


#ifndef CWINDOW_H
#define CWINDOW_H

#include "config.h"
#include "window_xcb.h"
#include "window_win32.h"
#include "window_android.h"

#if defined(VK_USE_PLATFORM_XCB_KHR)
    typedef Window_xcb GWindow;
#elif defined(VK_USE_PLATFORM_WIN32_KHR)
    typedef Window_win32 GWindow;
#elif defined(VK_USE_PLATFORM_ANDROID_KHR)
    typedef Window_android GWindow;
#endif

#endif
