#ifndef GAMEPAD_LINUX_H
#define GAMEPAD_LINUX_H

//#ifdef ENABLE_GAMEPAD
#include <libevdev/libevdev.h>    // libevdev-dev
#include <fcntl.h>                // open
#include <unistd.h>               // read, close, usleep
#include <sys/inotify.h>          // inotify
#include <dirent.h>               // For scanning /dev/input/
#include <sys/ioctl.h>
#include <linux/input.h>
#include "gamepads.h"

#define MAX_BTNS 16
#define MAX_AXIS 16

class GamepadLinux {
    WindowBase* win;              // owner window (gamepad[], eventFIFO, gpad*)
    int inotify_fd = -1;          // gamepad inotify descriptor
    int watch_fd   = -1;          // gamepad watch descriptor
    
    struct Evdev {                // gamepad handle and axis ranges
        int fd = -1;
        libevdev* dev = nullptr;
        char name[256] = {};      // gamepad model name
        char path[256] = {};      // eg. /dev/input/event240

        struct Btns{
            uint16_t BTN;         // BTN event code
            int8_t  eBTN;         // eGamepadBtn
        }b[MAX_BTNS]={};          // buttons

        struct Axes{
            uint16_t AXIS;        // ABS event code
            int8_t  eAXIS;        // eGamepadAxis
            int  min =0;          // Axis range min value
            int  max =0;          // Axis range max value
            int  fuzz=0;          // Noise level
            int  flat=0;          // Dead zone
            bool flip=false;      // Flip this axis
            int  prev=0;          // previous value
        }a[MAX_AXIS]={};          // axes

    } evdev[MAX_GAMEPADS];

    //void DetectGamepads();                                   // Detect connected gamepads
    //bool ConnectGamepad(const char* path);                   // eg. /dev/input/event240
    //void DisconnectGamepad(uint8_t id);                      // Disconnect gamepad by id (0-3)
    //void MapGamepad(uint8_t id);                             // Map gamepad btn/axis layout
    //void SetGamepadLEDs(uint8_t id, uint8_t state);          // pad-id(0-3), led-bitmask(0-15)
    //void ReadGamepadEvents();                                // Process all gamepad events
    //void GamepadBtnEvent(uint8_t id, input_event event);     // Button events
    //void GamepadAxisEvent(uint8_t id, input_event event);    // Axis events

    void onConnect(uint8_t pad, bool active)            {win->eventFIFO.push(win->gpadConnect(pad, active));}
    void onButton (uint8_t pad, uint8_t btn, bool down) {win->eventFIFO.push(win->gpadButton(pad,btn,down));}
    void onAxis   (uint8_t pad, uint8_t axis, float val){win->eventFIFO.push(win->gpadAxis(pad, axis, val));}

public:
    explicit GamepadLinux(WindowBase* window) : win(window) {}

    ~GamepadLinux() {
        for (int i = 0; i < MAX_GAMEPADS; ++i) { DisconnectGamepad(i); }
        if (watch_fd   != -1) { inotify_rm_watch(inotify_fd, watch_fd); watch_fd=-1;}
        if (inotify_fd != -1) { ::close(inotify_fd); inotify_fd=-1; }
    }

    void ReadGamepadEvents() {
        DetectGamepads();
        for (int i=0; i<MAX_GAMEPADS; ++i) {
            Evdev&   ev  = evdev[i];
            Gamepad& pad = win->gamepad[i];
            if (!pad.active) continue;

           int rc=0;
            struct input_event event;
            while ((rc=libevdev_next_event(ev.dev, LIBEVDEV_READ_FLAG_NORMAL, &event)) == 0) {
                if (event.type == EV_KEY) { GamepadBtnEvent (i, event); } else // Button press/release
                if (event.type == EV_ABS) { GamepadAxisEvent(i, event); }      // Analog axes and hat buttons
            }
            if(rc==-ENODEV) DisconnectGamepad(i);
        }
    }

private:
    void DetectGamepads() {
        if (inotify_fd == -1) {  // inotify not started yet
            // On first run, scan for already connected gamepads
            DIR* dir = opendir("/dev/input/");
            if(dir) {
                dirent* entry;
                while ((entry = readdir(dir))) {
                    if (strncmp(entry->d_name, "event", 5) == 0) { // Look for "event#"
                        char path[512]{};
                        snprintf(path, sizeof(path), "/dev/input/%s", entry->d_name);
                        ConnectGamepad(path);
                    }
                }
                closedir(dir);
            }
            // Watch for new device connections using inotify
            inotify_fd = inotify_init1(IN_NONBLOCK);
            if (inotify_fd < 0) { perror("inotify_init1"); return; }
            watch_fd = inotify_add_watch(inotify_fd, "/dev/input/", IN_CREATE);
        }
        // Process inotify events
        char buffer[1024];
        int len = read(inotify_fd, buffer, sizeof(buffer));
        if (len > 0) {
            for (char* ptr = buffer; ptr < buffer + len;) {
                struct inotify_event* event = (struct inotify_event*)ptr;
                ptr += sizeof(struct inotify_event) + event->len;
                if (event->mask & IN_CREATE) {
                    if (strncmp(event->name, "event", 5) == 0) {  // Look for "eventX"
                        char path[256]{};
                        snprintf(path, sizeof(path), "/dev/input/%s", event->name);
                        usleep(60000);         // Allow time for the device to appear
                        ConnectGamepad(path);  // Try to connect as Gamepad
                    }
                }
            }
        }
    }

    static void setGamepadLED(const char* devicePath, int LED_id, bool state) {
        int fd = open(devicePath, O_WRONLY);
        if(fd<0) return;
        struct input_event event;
        event.type = EV_LED;
        event.code = LED_id;
        event.value = state ? 1 : 0;
        ssize_t s=write(fd, &event, sizeof(event));
        close(fd);
    }

    void SetGamepadLEDs(uint8_t id, uint8_t state) {
        setGamepadLED(evdev[id].path, 0, !!(state&1));
        setGamepadLED(evdev[id].path, 1, !!(state&2));
        setGamepadLED(evdev[id].path, 2, !!(state&4));
        setGamepadLED(evdev[id].path, 3, !!(state&8));
    }

    bool ConnectGamepad(const char* path) {
        int fd = open(path, O_RDONLY | O_NONBLOCK);
        if(fd<0) return false;
        libevdev* dev = nullptr;
        if (libevdev_new_from_fd(fd, &dev) >= 0) {
            if(libevdev_has_event_type(dev, EV_ABS)
            && libevdev_has_event_type(dev, EV_KEY)
            && libevdev_has_event_code(dev, EV_KEY, BTN_SOUTH)
            && libevdev_has_event_code(dev, EV_KEY, BTN_NORTH)
            && libevdev_has_event_code(dev, EV_KEY, BTN_EAST )
            && libevdev_has_event_code(dev, EV_KEY, BTN_WEST )
            && libevdev_has_event_code(dev, EV_ABS, ABS_X)
            && libevdev_has_event_code(dev, EV_ABS, ABS_Y)) {  // make sure this is a gamepad
                for (int i = 0; i < MAX_GAMEPADS; ++i) {       // find a free slot
                    Evdev& ev = evdev[i];
                    if(ev.fd < 0) {
                        ev.fd = fd;
                        ev.dev= dev;
                        strncpy(ev.name, libevdev_get_name(dev), sizeof(ev.name)-1);  // query gamepad name
                        strncpy(ev.path, path, sizeof(ev.path)-1);                    // get gamepad event file path
                        //printf("Gamepad %d found: %s at %s\n", i, ev.name, path);
                        MapGamepad(i);           // Detect gamepad button layout
                        SetGamepadLEDs(i,1<<i);  // Set Gamepad LEDs to indicate which slot its in.
                        onConnect(i, true);
                        return true;
                    }
                }
            }
            libevdev_free(dev); ::close(fd);
        } else ::close(fd);
        return false;
    }

    void DisconnectGamepad(uint8_t id) {
        Evdev& ev = evdev[id];
        if(ev.fd==-1) return;
        onConnect(id, false);
        //SetGamepadLEDs(id, 0);  // Does not restore blinking :(
        libevdev_free(ev.dev);
        ::close(ev.fd);
        memset(&ev, 0, sizeof(ev));
        ev.fd = -1;
        ev.dev = 0;
        ev.name[0] = '\0';
        ev.path[0] = '\0';
        //printf("Gamepad %d disconnected.\n", id);
    }

    void MapGamepad(uint8_t id) {
        Evdev& pad = evdev[id];
        auto dev = pad.dev;
        const char* name = libevdev_get_name(dev);
        uint BUS=libevdev_get_id_bustype(dev);
        uint VID=libevdev_get_id_vendor(dev);
        uint PID=libevdev_get_id_product(dev);
        printf("Gamepad %d: \"%s\"\n",id , name);
        //printf("bus:%#x vendor:%#x product:%#x\n",bus, VID, PID);
        //-----------------------------------------------------------------
        int hatx_inx=0;  // HAT0X axis-index
        int haty_inx=0;  // HAT0Y axis-index
        int axis_cnt=0;  // not used

        // get the button event code list
        for (int code=0,b=0; code<KEY_MAX && b<MAX_BTNS; code++)
            if(libevdev_has_event_code(dev, EV_KEY, code)) pad.b[b++].BTN=code;

        // get axis event code and limits
        for (int code=0,a=0; code<KEY_MAX && a<MAX_AXIS; code++) {
            if(libevdev_has_event_code(dev, EV_ABS, code)) {
                if(code==ABS_HAT0X) hatx_inx=a;  // save the hatx index for later
                if(code==ABS_HAT0Y) haty_inx=a;  // save the haty index for later
                auto& axis = pad.a[a++];
                axis.AXIS = code;
                axis.min  = libevdev_get_abs_minimum(dev, code);  // axis min value
                axis.max  = libevdev_get_abs_maximum(dev, code);  // axis max value
                axis.fuzz = libevdev_get_abs_fuzz   (dev, code);  // noise level
                axis.flat = libevdev_get_abs_flat   (dev, code);  // dead zone
            }
            axis_cnt=a;
        }
        //for(auto& line : gamepad_layouts){for(uint8_t val : line) printf("%02X ", val); printf("\n");}

        auto* p_layout = get_gamepad_layout(VID, PID, BUS, name);  // Find layout by gamepad ID

        if(p_layout) {  // Gamepad found. Decode layout tokens.
            int8_t eBTN [] = {1,2,3,4,5,6,7,8,9,10,11,12,-1,-2,-3,-4,-5,-6,13,14};  // pos-to-eBTN
            int8_t eAXIS[] = {0,0,0,0,0,0,0,0,0, 0, 0, 0, 1, 2, 3, 4, 5, 6, 0, 0};  // pos-to-eAxis
            auto layout = *p_layout;
            for(int i=0; i<layout.size(); ++i) {
                uint8_t code = layout[i];
                //printf("%02x ",code);
                if(code==0xff) continue;                         // not mapped
                uint8_t num = code &0x1F;                        // extract number
                bool flip   = code &0x80;                        // extract flip flag
                bool isBtn  =!(code&0x60);                       // No flag for button
                bool isHat  = code &0x40;                        // extract hat flag
                bool isAxis = code &0x20;                        // extract axis flag
                if(isBtn)  pad.b[num].eBTN  = eBTN[i];           // button event code to eGamepadBtn map
                if(isAxis) pad.a[num].eAXIS = eAXIS[i];          // axis event code to eGamepadAxis map
                if(isAxis) pad.a[num].flip  = flip;              // flip axis
                if(isHat && i==8)  pad.a[hatx_inx].flip = flip;  // flip HAT0X
                if(isHat && i==10) pad.a[haty_inx].flip = flip;  // flip HAT0Y
                //printf("i=%d num=%d isBtn=%d isHat=%d isAxis=%d filp=%d\n",i ,num, isBtn, isHat, isAxis, flip);
            }
        } else {  // Gamepad not listed.  Use heuristics to guess layout.
            bool hasHAT=!!(hatx_inx & haty_inx);
            bool hasTrigger=false;

            auto& a = pad.a;
            a[0].eAXIS = eAXIS_LX;                                                                                 // left stick X (always first listed axis)
            a[1].eAXIS = eAXIS_LY;                                                                                 // left stick Y (always second listed axis)
            for(auto& ax : a) if(ax.AXIS==ABS_HAT0X) {ax.eAXIS=7; hasHAT=true; break;}                             // Hat X (always ABS_HAT0X, or a button)
            for(auto& ax : a) if(ax.AXIS==ABS_HAT0Y) {ax.eAXIS=8; hasHAT=true; break;}                             // Hat Y (always ABS_HAT0Y, or a button)
            for(auto& ax : a) if(!ax.eAXIS && ax.min==a[0].min && ax.max==a[0].max) {ax.eAXIS=eAXIS_RX; break;}    // right stick X (min/max should match left stick)
            for(auto& ax : a) if(!ax.eAXIS && ax.min==a[0].min && ax.max==a[0].max) {ax.eAXIS=eAXIS_RY; break;}    // right stick Y
            for(auto& ax : a) if(!ax.eAXIS && ax.min==0 && ax.max>1) {ax.eAXIS=eAXIS_TL; hasTrigger=true; break;}  // left trigger (Usually has min=0)
            for(auto& ax : a) if(!ax.eAXIS && ax.min==0 && ax.max>1) {ax.eAXIS=eAXIS_TR; hasTrigger=true; break;}  // right trigger
            //for(int i=0; i<axis_cnt; ++i) { auto& ax = a[i]; printf("i:%d  AXIS=%2d eAXIS=%2d  min=%5d  max=%5d  fuzz=%5d  flat=%5d  flip=%d\n", i, ax.AXIS, ax.eAXIS, ax.min, ax.max, ax.fuzz, ax.flat, ax.flip);}

            bool HID_style      = ( hasHAT &&  hasTrigger);  // HID and XInput compliant (XBox)
            bool Nintendo_style = ( hasHAT && !hasTrigger);  // Nintendo uses button triggers
            bool Sony_style     = (!hasHAT &&  hasTrigger);  // Sony uses DPad instead of HAT

            auto& btns = pad.b;
            if(HID_style) {
                for(auto& b : btns) {
                    if(b.BTN==BTN_A)       b.eBTN=eBTN_A;
                    if(b.BTN==BTN_B)       b.eBTN=eBTN_B;
                    if(b.BTN==BTN_X)       b.eBTN=eBTN_X;
                    if(b.BTN==BTN_Y)       b.eBTN=eBTN_Y;
                    if(b.BTN==BTN_TL)      b.eBTN=eBTN_TL;
                    if(b.BTN==BTN_TR)      b.eBTN=eBTN_TR;
                    if(b.BTN==BTN_THUMBL)  b.eBTN=eBTN_THUMBL;
                    if(b.BTN==BTN_THUMBR)  b.eBTN=eBTN_THUMBR;
                    if(b.BTN==BTN_SELECT)  b.eBTN=eBTN_SELECT;
                    if(b.BTN==BTN_START)   b.eBTN=eBTN_START;
                }
            }

            if(Nintendo_style) {
                for(auto& b : btns) {
                    if(b.BTN==0x130) b.eBTN=eBTN_A;
                    if(b.BTN==0x131) b.eBTN=eBTN_B;
                    if(b.BTN==0x132) b.eBTN=eBTN_X;
                    if(b.BTN==0x133) b.eBTN=eBTN_Y;
                    if(b.BTN==0x134) b.eBTN=eBTN_TL;
                    if(b.BTN==0x135) b.eBTN=eBTN_TR;
                    if(b.BTN==0x136) b.eBTN=-eAXIS_TL;  // button trigger
                    if(b.BTN==0x137) b.eBTN=-eAXIS_TR;  // button trigger
                    if(b.BTN==0x13a) b.eBTN=eBTN_THUMBL;
                    if(b.BTN==0x13b) b.eBTN=eBTN_THUMBR;
                    if(b.BTN==0x138) b.eBTN=eBTN_SELECT;
                    if(b.BTN==0x139) b.eBTN=eBTN_START;
                }
            }

            if(Sony_style) {  // untested
                for(auto& b : btns) {
                    if(b.BTN==0x130) b.eBTN=eBTN_A;
                    if(b.BTN==0x131) b.eBTN=eBTN_B;
                    if(b.BTN==0x132) b.eBTN=eBTN_X;
                    if(b.BTN==0x133) b.eBTN=eBTN_Y;
                    if(b.BTN==0x134) b.eBTN=eBTN_TL;
                    if(b.BTN==0x135) b.eBTN=eBTN_TR;
                    if(b.BTN==0x13d) b.eBTN=eBTN_THUMBL;
                    if(b.BTN==0x13e) b.eBTN=eBTN_THUMBR;
                    if(b.BTN==0x138) b.eBTN=eBTN_SELECT;
                    if(b.BTN==0x139) b.eBTN=eBTN_START;
                }
            }

            for(auto& b : btns) {  // DPad buttons (Sony?)
                if(b.BTN==BTN_DPAD_UP)    b.eBTN=eDPAD_UP;
                if(b.BTN==BTN_DPAD_DOWN)  b.eBTN=eDPAD_DOWN;
                if(b.BTN==BTN_DPAD_LEFT)  b.eBTN=eDPAD_LEFT;
                if(b.BTN==BTN_DPAD_RIGHT) b.eBTN=eDPAD_RIGHT;
            }

            //for(int i=0; i<MAX_BTNS; ++i) {auto& b = pad.map.b[i]; printf("BTN=%d eBTN=%d\n", b.BTN, b.eBTN);}
        }
    }

    void GamepadBtnEvent(uint8_t id, input_event event) {
        auto& ev = evdev[id];
        uint keycode = event.code;
        //printf("keycode=%d (0x%3x) %d\n", keycode, keycode, event.value);
        if(event.value>1) return;  // ignore repeats (0=up 1=down 2=repeat)
        for(auto& b : ev.b) if(keycode==b.BTN) {
            if(b.eBTN>0) onButton(id, b.eBTN, event.value);
            if(b.eBTN<0) onAxis  (id,-b.eBTN, event.value);
        }
    }

    void GamepadAxisEvent(uint8_t id, input_event event) {
        Gamepad& pad = win->gamepad[id];
        Evdev&   ev  = evdev[id];

        //------------------------------------------------------------------------------
        auto find_axis = [&](uint axiscode) -> Evdev::Axes& {
            for(auto& a : ev.a) if(axiscode==a.AXIS) return a;
            return ev.a[0];
        };

        auto Hat = [&](int val, int btnNeg, int btnPos) { // convert hat axis values to button events
            if((val!=-1) && ( pad.buttons[btnNeg])) onButton(id, btnNeg, 0);
            if((val!= 1) && ( pad.buttons[btnPos])) onButton(id, btnPos, 0);
            if((val==-1) && (!pad.buttons[btnNeg])) onButton(id, btnNeg, 1);
            if((val== 1) && (!pad.buttons[btnPos])) onButton(id, btnPos, 1);
        };

        auto isFuzz = [](int value, auto& a) -> bool { // detect fuzz events
            int delta = abs(a.prev - value);
            if(delta<a.fuzz) return true;
            a.prev = value;
            return false;
        };

        auto Trigger = [](int value, auto& a) -> float { // Apply dead-zone, normalize
            int val   = std::max(value-a.min-a.flat,0);
            int range = std::max(a.max-a.min-a.flat,1);
            return val / (float)range;
        };

        auto Thumb = [](int value, auto& a) -> float { // Apply dead-zone, normalize
            int center = (a.min + a.max) / 2;
            int centered = value - center;
            int sign  = centered<0 ? -1:1;
            int val   = std::max(std::abs(centered) - a.flat, 0);
            int range = a.max - center - a.flat;
            return (val / (float)range) * sign;
        };
        //------------------------------------------------------------------------------

        auto& a = find_axis(event.code);
        int val = a.flip ? -event.value : event.value;
        if(event.code == ABS_HAT0X) {Hat(val, eDPAD_LEFT, eDPAD_RIGHT); return;}
        if(event.code == ABS_HAT0Y) {Hat(val, eDPAD_UP,   eDPAD_DOWN);  return;}
        if(event.code > 10) return;         // Ignore HAT1+

        if(isFuzz(event.value, a)) return;  // defuzz
        bool isTrigger = (a.eAXIS==eAXIS_TL || a.eAXIS==eAXIS_TR);
        float fval = isTrigger? Trigger(event.value, a)
                              : Thumb  (event.value, a);

        if(pad.axes[a.eAXIS] == fval) return;  // deadzone
        if(a.eAXIS==eAXIS_LY || a.eAXIS==eAXIS_RY) fval=-fval; // flip y axis
        if(a.flip) fval=-fval;
        onAxis(id, a.eAXIS, fval);
    }

    /*
    // eg. SetGamepadRumble(0, 20000, 0);
    void SetGamepadRumble(int index, uint16_t weak, uint16_t strong) {  // TODO
        if (index < 0 || index >= MAX_GAMEPADS || evdev[index].fd < 0) return;

        struct ff_effect effect = {};
        effect.type = FF_RUMBLE;
        effect.id = -1;
        effect.u.rumble.strong_magnitude = strong;
        effect.u.rumble.weak_magnitude = weak;
        if (ioctl(evdev[index].fd, EVIOCSFF, &effect) < 0) return;

        struct input_event play = {};
        play.type = EV_FF;
        play.code = effect.id;
        play.value = 1;

        write(evdev[index].fd, &play, sizeof(play));  // Start rumble
        usleep(500000);                                  // Let it run for 500ms
        ioctl(evdev[index].fd, EVIOCRMFF, effect.id); // Remove the effect
    }
    */
};

//#endif  // ENABLE_GAMEPAD
#endif  // GAMEPAD_LINUX_H
