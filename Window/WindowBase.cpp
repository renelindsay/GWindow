/*
*--------------------------------------------------------------------------
* Platform-specific event handlers call these functions to store input-device state,
* and package the event parameters into a platform-independent "EventType" struct.
*--------------------------------------------------------------------------
*/

#include "WindowBase.h"

//--Events--
EventType WindowBase::MouseEvent(eAction action, int16_t x, int16_t y, uint8_t btn) {
    mouse.pos = {x, y};
    if (action != eMOVE) mouse.btn[btn] = (action == eDOWN);  // Keep track of button state
    EventType e = {EventType::MOUSE, {action, x, y, btn}};
    return e;
}

EventType WindowBase::KeyEvent(eAction action, uint8_t key) {
    keystate[key] = (action == eDOWN);
    EventType e   = {EventType::KEY};
    e.key         = {action, (eKeycode)key};
    return e;
}

EventType WindowBase::TextEvent(const char* str) {
    EventType e = {EventType::TEXT};
    e.text.str  = str;
    return e;
}

EventType WindowBase::MoveEvent(int16_t x, int16_t y) {
    shape.x     = x;
    shape.y     = y;
    EventType e = {EventType::MOVE};
    e.move      = {x, y};
    return e;
}

EventType WindowBase::ResizeEvent(uint16_t width, uint16_t height) {
    this->resized = true;
    shape.width  = width;
    shape.height = height;
    //float scale = GetScale();
    EventType e  = {EventType::RESIZE};
    e.resize     = {width, height};
    //e.resize.width = width / scale;
    //e.resize.height= height/ scale;
    return e;
}

EventType WindowBase::FocusEvent(bool has_focus) {
    this->has_focus   = has_focus;
    EventType e       = {EventType::FOCUS};
    e.focus.has_focus = has_focus;
    return e;
}

EventType WindowBase::GPadConnect(uint8_t pad, bool active) {
    gamepad[pad].active = active;
    EventType e = {EventType::GPAD_CONNECT};
    e.gp_connect.pad = pad;
    e.gp_connect.active = active;
    return e;
}

EventType WindowBase::GPadButton(uint8_t pad, uint8_t btn, bool down) {
    gamepad[pad].buttons[btn] = down;
    EventType e = {EventType::GPAD_BUTTON};
    e.gp_button.pad = pad;
    e.gp_button.down = down;
    e.gp_button.btn = btn;
    return e;
}

EventType WindowBase::GPadAxis(uint8_t pad, uint8_t axis, float val) {
    gamepad[pad].axes[axis] = val;
    EventType e = {EventType::GPAD_AXIS};
    e.gp_axis.pad  = pad;
    e.gp_axis.axis = axis;
    e.gp_axis.val  = val;
    return e;
}

EventType WindowBase::CloseEvent() {
    running = false;
    return {EventType::CLOSE};
}
//----------

bool WindowBase::ProcessEvents(bool wait_for_event) {
    EventType e = GetEvent(wait_for_event);
    while (e.tag != EventType::NONE) {
        running = ProcessEvent(e);  // Call event handlers
        if(!running) return false;
        e = GetEvent();
    }
    return running;
}

bool WindowBase::ProcessEvent(EventType e) {
    switch (e.tag) {
       case EventType::MOUSE       : OnMouseEvent (e.mouse.action, e.mouse.x, e.mouse.y, e.mouse.btn);  break;
       case EventType::KEY         : OnKeyEvent   (e.key.action, e.key.keycode);                        break;
       case EventType::TEXT        : OnTextEvent  (e.text.str);                                         break;
       case EventType::MOVE        : OnMoveEvent  (e.move.x, e.move.y);                                 break;
       case EventType::RESIZE      : OnResizeEvent(e.resize.width, e.resize.height);                    break;
       case EventType::FOCUS       : OnFocusEvent (e.focus.has_focus);                                  break;
       case EventType::TOUCH       : OnTouchEvent (e.touch.action, e.touch.x, e.touch.y, e.touch.id);   break;
       case EventType::GPAD_CONNECT: OnGPadConnect(e.gp_connect.pad, e.gp_connect.active);              break;
       case EventType::GPAD_BUTTON : OnGPadButton (e.gp_button.pad, e.gp_button.btn, e.gp_button.down); break;
       case EventType::GPAD_AXIS   : OnGPadAxis   (e.gp_axis.pad, e.gp_axis.axis, e.gp_axis.val);       break;
       case EventType::CLOSE       : OnCloseEvent (); return false;
       default: break;
    }
    return true;
}


