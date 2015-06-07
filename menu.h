#ifndef __MENU_H_
#define __MENU_H_

#include <stdbool.h>
#include <util/delay.h>
#include "n3310.h"
#include "defines.h"
#include "easyavr.h"
#include "eep_config.h"

char num_buf[8];

enum e_menus {
    M_LED,
    M_SOUND,
    M_SOUND_MEASURE,
    M_SOUND_DETECT,
    M_BACKLIGHT,
    M_CLICKS,
    M_MEASURE_TIME,
    M_CONTRAST,
    M_ALERT,
    M_ALERT_MIN,
    M_MINIMAL,
    M_RESET
};

enum menu_types {
    MT_MAIN_MENU,
    MT_SLIDER,
    MT_CONFIRM
};

enum buttons {
    B_OK,
    B_UP,
    B_DOWN,
    B_BACK
};

bool isMenuDisplayed;
int selected;


enum menu_types selected_menu_type;
enum e_menus selected_menu;
int slider_min;
int slider_max;
int slider_val;

void openMenu();

void returnBack();

void closeMenu();

void menuLoop();

void repaintMenu();

void downPressed();

void upPressed();

bool isInMainMenu();

bool checkDisabled(unsigned char index);

bool checkStat(unsigned char index);

bool actionPerformed(enum buttons button);

#endif //__MENU_H_
