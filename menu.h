#ifndef __MENU_H_
#define __MENU_H_

#include <stdbool.h>
#include <util/delay.h>
#include "n3310.h"
#include "defines.h"
#include "easyavr.h"
#include "eep_config.h"


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


bool isMenuDisplayed;
int selected;
int selectedSub;

void openMenu();

void menuLoop();

void repaintMenu();

void closeMenu();

void downPressed();

void upPressed();

bool isInMainMenu();

bool checkDisabled(unsigned char index);

bool checkStat(unsigned char index);

bool actionPerformed();

#endif //__MENU_H_
