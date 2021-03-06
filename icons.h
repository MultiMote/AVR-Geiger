#ifndef __ICONS_H_
#define __ICONS_H_

#include <avr/pgmspace.h>


PROGMEM const unsigned char resultFrame[3][51] = {
        {0xfc, 0xfe, 0xff, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x7, 0x3, 0x1, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0},
        {0xff, 0xff, 0xff, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0xfc, 0xf8, 0xf0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0},
        {0x3f, 0x7f, 0xff, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xff, 0xff, 0x7f, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0}
};

PROGMEM const unsigned char lastFrame[3][44] = {
        {0xf8, 0xfc, 0x8e, 0x7, 0x3, 0x3, 0x3, 0x7, 0x8f, 0xff, 0xff, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0xf, 0xff, 0xff, 0xfe},
        {0x0, 0x1, 0x3, 0x7, 0xe, 0x3e, 0xfe, 0xff, 0xe7, 0x81, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0xff, 0xff},
        {0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x3, 0x3f, 0x7f, 0xfe, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xff, 0x7f}
};

PROGMEM const unsigned char highlight[1][11] = {
        {0x10, 0x12, 0x4, 0x0, 0x0, 0x3, 0x0, 0x0, 0x4, 0x12, 0x10}
};

PROGMEM const unsigned char sun[1][8]={
        { 0x2, 0x2, 0x8, 0x2, 0x35, 0x2, 0x0, 0x2 }
};

PROGMEM const unsigned char loud[1][8]={
        { 0x12, 0xc, 0x21, 0x1e, 0x0, 0x0, 0x0, 0x0 }
};

PROGMEM const unsigned char mute[1][4]={
        { 0x9, 0x6, 0x6, 0x9 }
};

PROGMEM const unsigned char led_icon[1][5] = {
        {0x1e, 0xf9, 0x11, 0xf9, 0x1e}
};

PROGMEM const unsigned char speaker_icon[2][6] = {
        {0x78, 0x48, 0x78, 0xcc, 0x2, 0xff},
        {0x0, 0x0, 0x0, 0x0, 0x1, 0x3}
};

PROGMEM const unsigned char display_icon[2][9]={
        { 0xe0, 0x90, 0x17, 0x91, 0xe1, 0x5, 0xa5, 0xa1, 0xff },
        { 0x1, 0x3, 0x1, 0x3, 0x1, 0x0, 0x1, 0x1, 0x0 }
};

PROGMEM const unsigned char alert_icon[1][7] = {
        {0x80, 0xfe, 0x81, 0xff, 0xff, 0xfe, 0x80}
};

PROGMEM const unsigned char alert1_icon[1][5] = {
        {0x6, 0x17, 0x1c, 0x17, 0x6}
};

PROGMEM const unsigned char alert2_icon[1][1] = {
        {0x17}
};

#define TIMER_POSITIONS 9
PROGMEM const unsigned char timer_progress_icons[TIMER_POSITIONS][5] = {
        {0x0, 0x0, 0x0, 0x0, 0x0},
        {0x0, 0x0, 0xe, 0x6, 0x0},
        {0x0, 0x0, 0xe, 0xe, 0xc},
        {0x0, 0x0, 0xe, 0x1e, 0x1c},
        {0x0, 0x0, 0x3e, 0x3e, 0x1c},
        {0x0, 0x30, 0x3e, 0x3e, 0x1c},
        {0x18, 0x38, 0x3e, 0x3e, 0x1c},
        {0x18, 0x3c, 0x3e, 0x3e, 0x1c},
        {0x1c, 0x3e, 0x3e, 0x3e, 0x1c}
};

#endif //__ICONS_H_
