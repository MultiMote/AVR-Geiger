#include "menu.h"
#include <stdlib.h>
#include <string.h>

#define ITEMS_COUNT 12


char *menus[ITEMS_COUNT * 2] = {
        "ПОДСВЕТКА", (char *) M_BACKLIGHT,
        "КОНТРАСТ", (char *) M_CONTRAST,
        "ИНДИКАТОР", (char *) M_LED,
        "ЗВУК", (char *) M_SOUND,
        "ЗВУК ИЗМЕР.", (char *) M_SOUND_MEASURE,
        "ЗВУК ДЕТЕКТ.", (char *) M_SOUND_DETECT,
        "ЗВУК-ЩЕЛЧКИ", (char *) M_CLICKS,
        "СИГНАЛ", (char *) M_ALERT,
        "ПОРОГ СИГНАЛА", (char *) M_ALERT_MIN,
        "ВРЕМЯ ИЗМЕР.", (char *) M_MEASURE_TIME,
        "МИН. ИНТЕРФ.", (char *) M_MINIMAL,
        "СБРОС", (char *) M_RESET,
};


void cross(byte line) {
    LcdLine(5, 4 + 8 * line, LCD_X_RES - 1, 4 + 8 * line);
}

void openMenu() {
    isMenuDisplayed = true;
    selected = 0;
    returnBack();
    repaintMenu();
}

void menuLoop() {
    if (BACK_PRESSED) {
        if (isInMainMenu()) {
            closeMenu();
        } else {
            if (actionPerformed(B_BACK)) saveCfg();
            returnBack();
            repaintMenu();
        }
        _delay_ms(150);
        while (BACK_PRESSED) { }

    } else if (DOWN_PRESSED) {
        downPressed();
        repaintMenu();
        _delay_ms(150);
        while (DOWN_PRESSED) { }

    } else if (UP_PRESSED) {
        upPressed();
        repaintMenu();
        _delay_ms(150);
        while (UP_PRESSED) { }

    } else if (OK_PRESSED) {
        if (actionPerformed(B_OK)) saveCfg();
        repaintMenu();
        _delay_ms(150);
        while (OK_PRESSED) { }

    }
}

void returnBack() {
    selected_menu_type = MT_MAIN_MENU;
    selected_menu = M_BACKLIGHT;
}


void downPressed() {
    if (isInMainMenu()) {
        selected++;
        if (selected > ITEMS_COUNT - 1) selected = 0;
    } else if (actionPerformed(B_DOWN)) saveCfg();
}

void upPressed() {
    if (isInMainMenu()) {
        selected--;
        if (selected < 0) selected = ITEMS_COUNT - 1;
    } else if (actionPerformed(B_UP)) saveCfg();
}

void repaintMenu() {
    unsigned char i;
    unsigned char currentIndex;
    unsigned char shift;
    LcdClear();
    if (isInMainMenu()) { //выводим главное меню
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

    } else { //если выбран элемент, то выводим подменю
        if (selected_menu_type == MT_CONFIRM) {
            LcdGotoXYFont(0, 0);
            LcdStr(FONT_1X, menus[selected * 2], DEFAULT_MARGIN);
            LcdLine(0, 9, LCD_X_RES, 9);
            LcdGotoXYFont(1, 2);
            LcdStr(FONT_1X, "Подтвердить?", DEFAULT_MARGIN);
        } else if (selected_menu_type == MT_SLIDER) {
            LcdGotoXYFont(0, 0);
            LcdStr(FONT_1X, menus[selected * 2], DEFAULT_MARGIN);
            LcdLine(0, 9, LCD_X_RES, 9);

            LcdLine(0, 25, LCD_X_RES, 25);
            LcdLine(0, 30, LCD_X_RES, 30);

            byte pos = (byte) (LCD_X_RES * (1.0F / (float) (slider_max - slider_min)) * ((slider_val - slider_min)));

            LcdLine(pos, 25, pos, 30);
            LcdRect(pos - 3, 26, pos + 3, 29);


            memset(num_buf, 0, sizeof num_buf);
            itoa(slider_val, num_buf, 10);
            LcdGotoXYFont(7, 4);
            LcdStr(FONT_1X, num_buf, 6);

        }
    }

    LcdUpdate();
}

/*
 * Проверяет, нужно ли поставить звёздочку (включено) у пункта меню
 */
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

/*
 * Проверяет, нужно ли зачеркнуть пункт меню
 */
bool checkDisabled(unsigned char index) {
    int menuNum = (int) menus[(index * 2) + 1];

    if (menuNum == M_ALERT_MIN && !CFG_ALERT) return true;

    if ((menuNum == M_SOUND_DETECT || menuNum == M_SOUND_MEASURE || menuNum == M_CLICKS) && !CFG_SOUND) return true;

    return false;
}

/*
 * Вызывается при нажатии кнопки OK.
 * При возвращении true конфиг сохраняется.
 */
bool actionPerformed(enum buttons button) {
    int menuNum = (int) menus[(selected * 2) + 1];
    bool save = false;

    switch (menuNum) {
        case M_BACKLIGHT:
            CFG_BACKLIGHT = !CFG_BACKLIGHT;
            save = true;
            break;
        case M_SOUND:
            CFG_SOUND = !CFG_SOUND;
            save = true;
            break;
        case M_SOUND_DETECT:
            CFG_SOUND_DETECT = !CFG_SOUND_DETECT;
            save = true;
            break;
        case M_SOUND_MEASURE:
            CFG_SOUND_MEASURE = !CFG_SOUND_MEASURE;
            save = true;
            break;
        case M_LED:
            CFG_LED = !CFG_LED;
            save = true;
            break;
        case M_ALERT:
            CFG_ALERT = !CFG_ALERT;
            save = true;
            break;
        case M_MINIMAL:
            CFG_MINIMAL_GUI = !CFG_MINIMAL_GUI;
            save = true;
            break;
        case M_CLICKS:
            CFG_SOUND_CLICKS = !CFG_SOUND_CLICKS;
            save = true;
            break;
        case M_CONTRAST:
            if (selected_menu != M_CONTRAST && button == B_OK) {
                selected_menu_type = MT_SLIDER;
                slider_min = 30;
                slider_max = 90;
                slider_val = CFG_CONTRAST;
            }
            break;
        case M_ALERT_MIN:
            if (selected_menu != M_ALERT_MIN && button == B_OK) {
                selected_menu_type = MT_SLIDER;
                slider_min = 10;
                slider_max = 5000;
                slider_val = CFG_ALERT_MIN;
            }
            break;
        case M_MEASURE_TIME:
            if (selected_menu != M_MEASURE_TIME && button == B_OK) {
                selected_menu_type = MT_SLIDER;
                slider_min = 10;
                slider_max = 240;
                slider_val = CFG_MEASURE_TIME;
            }
            break;
        case M_RESET:
            if (selected_menu != M_RESET) {
                selected_menu_type = MT_CONFIRM;
            } else {
                resetCfg();
                readCfg(false);
                save = true;
                returnBack();
            }
            break;
        default:
            break;
    }

    selected_menu = (enum e_menus) menuNum;

    byte step = 1;
    if (selected_menu == M_MEASURE_TIME) step = 5;
    else if (selected_menu == M_ALERT_MIN) step = 10;

    if (selected_menu_type == MT_SLIDER) {
        if (button == B_UP) {
            if (slider_val + step < slider_max)
                slider_val += step;
        } else if (button == B_DOWN) {
            if (slider_val - step > slider_min)
                slider_val -= step;
        }

        if (selected_menu == M_CONTRAST) CFG_CONTRAST = (byte) slider_val;
        else if (selected_menu == M_MEASURE_TIME) CFG_MEASURE_TIME = (byte) slider_val;
        else if (selected_menu == M_ALERT_MIN) CFG_ALERT_MIN = (byte) slider_val;
        save = true;
    }

    return save;
}

void closeMenu() {
    isMenuDisplayed = false;
}

bool isInMainMenu() {
    return isMenuDisplayed && selected_menu_type == MT_MAIN_MENU;
}
