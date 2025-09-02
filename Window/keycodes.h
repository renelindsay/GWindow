/*-----------------------------------------------------------------------
*
* These are the standard, platform-independent USB HID keyboard codes,
* as defined at: http://www.freebsddiary.org/APC/usb_hid_usages.php
*
* These Keycodes are returned by the onKeyEvent, in the Window class,
* whenever a key is pressed or released.
* In order to provide consistent results across all platforms, the Window_###
* class converts the native platform-specific scancodes to these cross-platform
* USB HID codes, before returning it in the onKeyEvent.
*
* The layout of these keycodes are fixed, and the names correspond to a
* US keyboard layout. Unlike the key symbols, these keycodes do not change
* with international keyboard layout settings.
*
* eg. KEY_Z corresponds to the lower left key, which produces a 'z' character
* on US and UK qwerty keyboards, but a 'y' on German keyboards, and a ';' on
* dvorak keyboards, even though it is the same physical key.
*
* Therefore, use these keycodes for game controls, to ensure a consistent layout,
* but when text input is required, use the onTextEvent instead, to get the correct
* text symbol, according to the current configured keyboard layout settings.
*
* Contacts for feedback:
-   rjklindsay@hotmail.com (Rene Lindsay)
*/
//-----------------------------------------------------------------------

#ifndef KEYCODE_H
#define KEYCODE_H

// clang-format off
enum eKeycode {
    eKEY_NONE          = 0, // Undefined. (No event)
    eKEY_A             = 4,
    eKEY_B             = 5,
    eKEY_C             = 6,
    eKEY_D             = 7,
    eKEY_E             = 8,
    eKEY_F             = 9,
    eKEY_G             = 10,
    eKEY_H             = 11,
    eKEY_I             = 12,
    eKEY_J             = 13,
    eKEY_K             = 14,
    eKEY_L             = 15,
    eKEY_M             = 16,
    eKEY_N             = 17,
    eKEY_O             = 18,
    eKEY_P             = 19,
    eKEY_Q             = 20,
    eKEY_R             = 21,
    eKEY_S             = 22,
    eKEY_T             = 23,
    eKEY_U             = 24,
    eKEY_V             = 25,
    eKEY_W             = 26,
    eKEY_X             = 27,
    eKEY_Y             = 28,
    eKEY_Z             = 29,
    eKEY_1             = 30, // 1 and !
    eKEY_2             = 31, // 2 and @
    eKEY_3             = 32, // 3 and #
    eKEY_4             = 33, // 4 and $
    eKEY_5             = 34, // 5 and %
    eKEY_6             = 35, // 6 and ^
    eKEY_7             = 36, // 7 and &
    eKEY_8             = 37, // 8 and *
    eKEY_9             = 38, // 9 and (
    eKEY_0             = 39, // 0 and )
    eKEY_Enter         = 40, // (Return)
    eKEY_Escape        = 41,
    eKEY_Delete        = 42, // (Backspace)
    eKEY_Tab           = 43,
    eKEY_Space         = 44,
    eKEY_Minus         = 45, // - and (underscore)
    eKEY_Equals        = 46, // = and +
    eKEY_LeftBracket   = 47, // [ and {
    eKEY_RightBracket  = 48, // ] and }
    eKEY_Backslash     = 49, // \ and |
 // eKEY_NonUSHash     = 50, // # and ~
    eKEY_Semicolon     = 51, // ; and :
    eKEY_Quote         = 52, // ' and "
    eKEY_Grave         = 53,
    eKEY_Comma         = 54, // , and <
    eKEY_Period        = 55, // . and >
    eKEY_Slash         = 56, // / and ?
    eKEY_CapsLock      = 57,
    eKEY_F1            = 58,
    eKEY_F2            = 59,
    eKEY_F3            = 60,
    eKEY_F4            = 61,
    eKEY_F5            = 62,
    eKEY_F6            = 63,
    eKEY_F7            = 64,
    eKEY_F8            = 65,
    eKEY_F9            = 66,
    eKEY_F10           = 67,
    eKEY_F11           = 68,
    eKEY_F12           = 69,
    eKEY_PrintScreen   = 70,
    eKEY_ScrollLock    = 71,
    eKEY_Pause         = 72,
    eKEY_Insert        = 73,
    eKEY_Home          = 74,
    eKEY_PageUp        = 75,
    eKEY_DeleteForward = 76, // Delete key
    eKEY_End           = 77,
    eKEY_PageDown      = 78,
    eKEY_Right         = 79, // Right arrow
    eKEY_Left          = 80, // Left arrow
    eKEY_Down          = 81, // Down arrow
    eKEY_Up            = 82, // Up arrow
    eKP_NumLock        = 83,
    eKP_Divide         = 84,
    eKP_Multiply       = 85,
    eKP_Subtract       = 86,
    eKP_Add            = 87,
    eKP_Enter          = 88,
    eKP_1              = 89,
    eKP_2              = 90,
    eKP_3              = 91,
    eKP_4              = 92,
    eKP_5              = 93,
    eKP_6              = 94,
    eKP_7              = 95,
    eKP_8              = 96,
    eKP_9              = 97,
    eKP_0              = 98,
    eKP_Point          = 99, // . and Del
    eKP_Equals         = 103,
    eKEY_F13           = 104,
    eKEY_F14           = 105,
    eKEY_F15           = 106,
    eKEY_F16           = 107,
    eKEY_F17           = 108,
    eKEY_F18           = 109,
    eKEY_F19           = 110,
    eKEY_F20           = 111,
    eKEY_F21           = 112,
    eKEY_F22           = 113,
    eKEY_F23           = 114,
    eKEY_F24           = 115,
 // eKEY_Help          = 117,
    eKEY_Menu          = 118,
    eKEY_Mute          = 127,
    eKEY_VolumeUp      = 128,
    eKEY_VolumeDown    = 129,
    eKEY_LeftControl   = 224, // WARNING : Android has no Ctrl keys.
    eKEY_LeftShift     = 225,
    eKEY_LeftAlt       = 226,
    eKEY_LeftGUI       = 227,
    eKEY_RightControl  = 228,
    eKEY_RightShift    = 229, // WARNING : Win32 fails to send a WM_KEYUP message if both shift keys are pressed, and one released.
    eKEY_RightAlt      = 230,
    eKEY_RightGUI      = 231
};

enum eGamepadBtn {
    eBTN_UNKNOWN,
    eBTN_A,       // 1
    eBTN_B,       // 2
    eBTN_X,       // 3
    eBTN_Y,       // 4
    eBTN_TL,      // 5
    eBTN_TR,      // 6
    eBTN_THUMBL,  // 7
    eBTN_THUMBR,  // 8
    eDPAD_UP,     // 9
    eDPAD_DOWN,   // 10
    eDPAD_LEFT,   // 11
    eDPAD_RIGHT,  // 12
    eBTN_SELECT,  // Warning: Maps to Android's Back button. (Closes keyboard/app.)
    eBTN_START,   // 14
    eBTN_MODE     // Warning: Windows/Linux: Launch Steam. Android: Maps to Home btn. (Closes app.)
};

enum eGamepadAxis {
    eAXIS_UNKNOWN,
    eAXIS_LX,  // 1: left thumbstick  -1to1 (right is positive)
    eAXIS_LY,  // 2: left thumbstick  -1to1 (up is positive)
    eAXIS_RX,  // 3: right thumbstick -1to1 (right is positive)
    eAXIS_RY,  // 4: right thumbstick -1to1 (up is positive)
    eAXIS_TL,  // 5: left trigger      0to1
    eAXIS_TR,  // 6: right trigger     0to1
};

// clang-format on
#endif
