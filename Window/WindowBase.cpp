/*
*--------------------------------------------------------------------------
* Platform-specific event handlers call these functions to store input-device state,
* and package the event parameters into a platform-independent "EventType" struct.
*--------------------------------------------------------------------------
*/

#include "WindowBase.h"

//--Events--
EventType WindowBase::mouseEvent(eAction action, int16_t x, int16_t y, uint8_t btn) {
    mouse.pos = {x, y};
    if (action != eMOVE) mouse.btn[btn] = (action == eDOWN);  // Keep track of button state
    EventType e = {EventType::MOUSE, {action, x, y, btn}};
    return e;
}

EventType WindowBase::keyEvent(eAction action, uint8_t key) {
    keystate[key] = (action == eDOWN);
    EventType e   = {EventType::KEY};
    e.key         = {action, (eKeycode)key};
    return e;
}

EventType WindowBase::textEvent(const char* str) {
    EventType e = {EventType::TEXT};
    e.text.str  = str;
    return e;
}

EventType WindowBase::moveEvent(int16_t x, int16_t y) {
    shape.x     = x;
    shape.y     = y;
    EventType e = {EventType::MOVE};
    e.move      = {x, y};
    return e;
}

EventType WindowBase::resizeEvent(uint16_t width, uint16_t height) {
    this->is_resized = true;
    shape.width  = width;
    shape.height = height;
    //float scale = getScale();
    EventType e  = {EventType::RESIZE};
    e.resize     = {width, height};
    //e.resize.width = width / scale;
    //e.resize.height= height/ scale;
    return e;
}

EventType WindowBase::focusEvent(bool has_focus) {
    this->has_focus   = has_focus;
    EventType e       = {EventType::FOCUS};
    e.focus.has_focus = has_focus;
    return e;
}

EventType WindowBase::gpadConnect(uint8_t pad, bool active) {
    gamepad[pad].active = active;
    EventType e = {EventType::GPAD_CONNECT};
    e.gp_connect.pad = pad;
    e.gp_connect.active = active;
    return e;
}

EventType WindowBase::gpadButton(uint8_t pad, uint8_t btn, bool down) {
    gamepad[pad].buttons[btn] = down;
    EventType e = {EventType::GPAD_BUTTON};
    e.gp_button.pad = pad;
    e.gp_button.down = down;
    e.gp_button.btn = btn;
    return e;
}

EventType WindowBase::gpadAxis(uint8_t pad, uint8_t axis, float val) {
    gamepad[pad].axes[axis] = val;
    EventType e = {EventType::GPAD_AXIS};
    e.gp_axis.pad  = pad;
    e.gp_axis.axis = axis;
    e.gp_axis.val  = val;
    return e;
}

EventType WindowBase::closeEvent() {
    running = false;
    return {EventType::CLOSE};
}
//----------

bool WindowBase::processEvents(bool wait_for_event) {
    EventType e = getEvent(wait_for_event);
    //if(e.tag == EventType::NONE) onIdleEvent();
    while (e.tag != EventType::NONE) {
        running = processEvent(e);  // Call event handlers
        if(!running) return false;
        e = getEvent();
    }
    onFrame();
    return running;
}

bool WindowBase::processEvent(EventType e) {
    switch (e.tag) {
       case EventType::MOUSE       : onMouse      (e.mouse.action, e.mouse.x, e.mouse.y, e.mouse.btn);  break;
       case EventType::KEY         : onKey        (e.key.action, e.key.keycode);                        break;
       case EventType::TEXT        : onText       (e.text.str);                                         break;
       case EventType::MOVE        : onMove       (e.move.x, e.move.y);                                 break;
       case EventType::RESIZE      : onResize     (e.resize.width, e.resize.height);                    break;
       case EventType::FOCUS       : onFocus      (e.focus.has_focus);                                  break;
       case EventType::TOUCH       : onTouch      (e.touch.action, e.touch.x, e.touch.y, e.touch.id);   break;
       case EventType::GPAD_CONNECT: onGpadConnect(e.gp_connect.pad, e.gp_connect.active);              break;
       case EventType::GPAD_BUTTON : onGpadButton (e.gp_button.pad, e.gp_button.btn, e.gp_button.down); break;
       case EventType::GPAD_AXIS   : onGpadAxis   (e.gp_axis.pad, e.gp_axis.axis, e.gp_axis.val);       break;
       case EventType::CLOSE       : onClose      (); return false;
       default: break;
    }
    return true;
}


