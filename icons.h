#ifndef __ICONS_H_
#define __ICONS_H_

#include <avr/pgmspace.h>

#define CLOCK_CENTER_X 41
#define CLOCK_CENTER_Y 23

unsigned char clockPosX [8] = { 41, 43, 43, 43, 41, 39, 39, 39 };
unsigned char clockPosY [8] = { 21, 21, 23, 25, 25, 25, 23, 21 };


PROGMEM const unsigned char resultFrame[3][45] = {
        {0xfc, 0xfe, 0xff, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x7, 0xff, 0xff, 0xff, 0x83, 0x1, 0x1, 0x1, 0x1, 0x1, 0x83, 0xfe, 0xfc},
        {0xff, 0xff, 0xff, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0xf, 0x7f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x7f, 0x3f, 0x1f, 0x7},
        {0x3f, 0x7f, 0xff, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xe0, 0xff, 0x7f, 0x1f, 0x7, 0x1, 0x0, 0x0, 0x0, 0x0, 0x0}
};

PROGMEM const unsigned char lastFrame[3][39]={
        { 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x1, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0xf, 0xff, 0xff, 0xfe },
        { 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0xff, 0xff },
        { 0x10, 0x38, 0x7c, 0xf8, 0xf0, 0xe0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xff, 0x7f }
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

PROGMEM const unsigned char alarm_icon[1][7] = {
        {0x80, 0xfe, 0x81, 0xff, 0xff, 0xfe, 0x80}
};


#endif //__ICONS_H_
