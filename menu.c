#include "menu.h"

#define ITEMS_COUNT 12


char *menus[ITEMS_COUNT * 2] = {
        "ÏÎÄÑÂÅÒÊÀ", (char *) M_BACKLIGHT,
        "ÊÎÍÒÐÀÑÒ", (char *) M_CONTRAST,
        "ÈÍÄÈÊÀÒÎÐ", (char *) M_LED,
        "ÇÂÓÊ", (char *) M_SOUND,
        "ÇÂÓÊ ÈÇÌÅÐ.", (char *) M_SOUND_MEASURE,
        "ÇÂÓÊ ÄÅÒÅÊÒ.", (char *) M_SOUND_DETECT,
        "ÇÂÓÊ-ÙÅË×ÊÈ", (char *) M_CLICKS,
        "ÑÈÃÍÀË", (char *) M_ALERT,
        "ÏÎÐÎÃ ÑÈÃÍÀËÀ", (char *) M_ALERT_MIN,
        "ÂÐÅÌß ÈÇÌÅÐ.", (char *) M_MEASURE_TIME,
        "ÌÈÍ. ÈÍÒÅÐÔ.", (char *) M_MINIMAL,
        "ÑÁÐÎÑ", (char *) M_RESET,
};


void cross(byte line) {
    LcdLine(5, 4 + 8 * line, LCD_X_RES - 1, 4 + 8 * line, PIXEL_ON);
}

void openMenu() {
    isMenuDisplayed = true;
    selected = 0;
    selectedSub = -1;
    repaintMenu();
}

void menuLoop() {
    if (BACK_PRESSED) {
        if (selectedSub == -1) {
            closeMenu();
        }
    } else if (DOWN_PRESSED) {
        downPressed();
        repaintMenu();
        _delay_ms(150);
        while (DOWN_PRESSED) {}

    } else if (UP_PRESSED) {
        upPressed();
        repaintMenu();
        _delay_ms(150);
        while (UP_PRESSED) {}

    } else if (OK_PRESSED) {
        if (actionPerformed()) saveCfg();
        repaintMenu();
        _delay_ms(150);
        while (OK_PRESSED) {}

    }
}


void downPressed() {
    if (isInMainMenu()) {
        selected++;
        if (selected > ITEMS_COUNT - 1) selected = 0;
    }
}

void upPressed() {
    if (isInMainMenu()) {
        selected--;
        if (selected < 0) selected = ITEMS_COUNT - 1;
    }
}

void repaintMenu() {
    unsigned char i;
    unsigned char currentIndex;
    unsigned char shift;
    LcdClear();
    if (isInMainMenu()) { //âûâîäèì ãëàâíîå ìåíþ
        for (i = 0; i < 6; i++) {
            shift = (unsigned char) (selected / 6);
            currentIndex = (unsigned char) ((shift * 6) + i);
            if (currentIndex > -1 && currentIndex < ITEMS_COUNT) {
                LcdGotoXYFont(1, i);
                LcdStr(FONT_1X, menus[currentIndex * 2], DEFAULT_MARGIN);

                if (currentIndex == selected) {
                    LcdGotoXYFont(0, i);
                    LcdStr(FONT_1X, ">", DEFAULT_MARGIN);
                }

                if (checkDisabled(currentIndex)) cross(i);
                if (checkStat(currentIndex)) {
                    LcdGotoXYFont(13, i);
                    LcdStr(FONT_1X, "*", DEFAULT_MARGIN);
                }
            }
        }

    } else { //åñëè âûáðàí ýëåìåíò, òî âûâîäèì ïîäìåíþ

    }

    LcdUpdate();
}

bool checkStat(unsigned char index) {
    int menuNum = (int) menus[(index * 2) + 1];
    if (menuNum == M_LED && CFG_LED) return true;
    if (menuNum == M_SOUND && CFG_SOUND) return true;
    if (menuNum == M_SOUND_DETECT && CFG_SOUND_DETECT) return true;
    if (menuNum == M_SOUND_MEASURE && CFG_SOUND_MEASURE) return true;
    if (menuNum == M_BACKLIGHT && CFG_BACKLIGHT) return true;
    if (menuNum == M_ALERT && CFG_ALERT) return true;
    if (menuNum == M_MINIMAL && CFG_MINIMAL_GUI) return true;
    if (menuNum == M_CLICKS && CFG_SOUND_CLICKS) return true;

    return false;
}

bool checkDisabled(unsigned char index) {
    int menuNum = (int) menus[(index * 2) + 1];

    if (menuNum == M_ALERT_MIN && !CFG_ALERT) return true;

    if ((menuNum == M_SOUND_DETECT || menuNum == M_SOUND_MEASURE || menuNum == M_CLICKS) && !CFG_SOUND) return true;

    return false;
}

bool actionPerformed() {
    int menuNum = (int) menus[(selected * 2) + 1];

    switch (menuNum) {
        case M_BACKLIGHT:
            CFG_BACKLIGHT = !CFG_BACKLIGHT;
            PIN_SET(LCD_LED_PORT, LCD_LED_PIN, CFG_BACKLIGHT);
            return true;
        case M_SOUND:
            CFG_SOUND = !CFG_SOUND;
            return true;
        case M_SOUND_DETECT:
            CFG_SOUND_DETECT = !CFG_SOUND_DETECT;
            return true;
        case M_SOUND_MEASURE:
            CFG_SOUND_MEASURE = !CFG_SOUND_MEASURE;
            return true;
        case M_LED:
            CFG_LED = !CFG_LED;
            return true;
        case M_ALERT:
            CFG_ALERT = !CFG_ALERT;
            return true;
        case M_MINIMAL:
            CFG_MINIMAL_GUI = !CFG_MINIMAL_GUI;
            return true;
        case M_CLICKS:
            CFG_SOUND_CLICKS = !CFG_SOUND_CLICKS;
            return true;
        default:
            break;
    }


    return false;
}

void closeMenu() {
    isMenuDisplayed = false;
}

bool isInMainMenu() {
    return isMenuDisplayed && selectedSub == -1;
}
