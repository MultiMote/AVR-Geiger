#include <string.h>
#include "main.h"

void beep(int freq) {
    timer1Tone(freq);
    _delay_ms(BEEP_LEN);
    timer1off();
}

uint32_t convertCPM(uint32_t clicks) {
    return (uint32_t) (clicks * SBM20_CONV_FACTOR * 100);
}


//***********************************************************
// ПРЕРЫВАНИЯ
//***********************************************************
ISR(TIMER2_OVF_vect) {
    isHalfSecond = !isHalfSecond;
    if (!isHalfSecond) seconds++;
    halfSeconds++;
    if (seconds > 59) {
        seconds = 0;
        halfSeconds = 0;
        minutes++;
    }
    onHalfSecond();
}


ISR(TIMER1_COMPA_vect) {
    PIN_SET(SPEAKER_PORT, SPEAKER_PIN, !CHECK_PIN(SPEAKER_PORT, SPEAKER_PIN));
}

ISR(INT0_vect) {
    ticks++;
}
//***********************************************************

void pushToStream(uint16_t clicks) {
    int i;
    for (i = 0; i < MEASURE_STREAM_SIZE - 1; i++) {
        clicksStream[i] = clicksStream[i + 1];
    }
    clicksStream[MEASURE_STREAM_SIZE - 1] = clicks;
}


void onHalfSecond() {

    if (isHalfSecond) {
        pushToStream(ticks - ticksMeasure);
        ticksMeasure = ticks;
    }

    measureByTime = convertCPM((uint32_t) ((float) (ticks - ticksTimeMeasure) * (60.0F / (float) CFG_MEASURE_TIME)));

    if (seconds - measureTimeStart >= CFG_MEASURE_TIME) {
        measureTimeStart = seconds;
        ticksTimeMeasure = ticks;
        lastMeasure = measureByTime;
        measureFinished = true;
    }

    if (measureByStream >= CFG_ALERT_MIN /*|| lastMeasure >= max*/) alert = true;
    else alert = false;

    uint16_t sum = 0;
    for (int i = 0; i < MEASURE_STREAM_SIZE; i++) {
        sum += clicksStream[i];
    }

    measureByStream = convertCPM((uint32_t) ((float) sum * (60.0F / (float) MEASURE_STREAM_SIZE)));;

    if (CFG_ALERT && alert) {
        if (isHalfSecond) {
            PIN_ON(LED_PORT, LED_PIN);
            timer1Tone(ALARM_TONE_1);
            _alarm = true;
        } else {
            PIN_OFF(LED_PORT, LED_PIN);
            timer1Tone(ALARM_TONE_2);
            _alarm = true;
        }
    } else {
        if (_alarm) {
            PIN_OFF(LED_PORT, LED_PIN);
            timer1off();
        }
    }

}

void drawBackground() {
    //  sprintf(buf, "%04u", measureByStream);

    byte shift = CFG_MINIMAL_GUI ? 1 : 0;

    memset(buf, 0, sizeof buf);
    itoa(measureByStream, buf, 10);

    LcdGotoXYFont(1, 3 - shift);
    LcdStr(FONT_2X, buf, 6);

    //   sprintf(buf, "%04u", lastMeasure);
    memset(buf, 0, sizeof buf);
    itoa(lastMeasure, buf, 10);
    LcdGotoXYFont(8, 3 - shift);
    LcdStr(FONT_2X, buf, 6);

    LcdGotoXYFont(1, 4 - shift);
    LcdStr(FONT_1X, MKR_H, DEFAULT_MARGIN);
    LcdGotoXYFont(8, 4 - shift);
    LcdStr(FONT_1X, MKR_H, DEFAULT_MARGIN);

}

void drawForeground() {
    if (!CFG_MINIMAL_GUI) {
        LCDIcon(&resultFrame[0][0], 2, 19, 3, 51, true); // рамка непрерывного измерения
        LCDIcon(&lastFrame[0][0], 36, 19, 3, 44, true); // рамка измерения по таймеру


        LCDIcon(&led_icon[0][0], 4, 3, 1, 5, true); // индикатор
        LCDIcon(&speaker_icon[0][0], 16, 1, 2, 6, true); // звук
        LCDIcon(&display_icon[0][0], 35, 1, 2, 9, true); // подсветка
        LCDIcon(&alert_icon[0][0], 51, 3, 1, 7, true); // сигнал

        if (CFG_LED)LCDIcon(&highlight[0][0], 1, 0, 1, 11, true);  // индикатор
        if (CFG_BACKLIGHT)LCDIcon(&highlight[0][0], 32, 2, 1, 11, true); // подсветка дисплея
        if (CFG_ALERT)LCDIcon(&highlight[0][0], 49, 0, 1, 11, true); // сигнал


        if (!CFG_SOUND || (!CFG_SOUND_MEASURE && !CFG_SOUND_DETECT))LCDIcon(&mute[0][0], 23, 4, 1, 4, true);
        else LCDIcon(&loud[0][0], 23, 3, 1, 8, true);

    }

    float progress = 1.0F / (float) CFG_MEASURE_TIME * (float) (seconds - measureTimeStart);
    unsigned char pos = (unsigned char) ((float) TIMER_POSITIONS * progress);

    if (!CFG_MINIMAL_GUI) {
        if (!alert) {
            LCDIcon(&timer_progress_icons[pos][0], 39, 21, 1, 5, true);
        } else {
            if (seconds % 3 == 0) {
                LCDIcon(&timer_progress_icons[pos][0], 39, 21, 1, 5, true);
            }
            else if (seconds % 2 == 0) {
                LCDIcon(&alert1_icon[0][0], 39, 22, 1, 5, true);
            } else {
                LCDIcon(&alert2_icon[0][0], 41, 22, 1, 1, true);
            }
        }

        LcdRect(0, 12, 62, 13);
        LcdRect(13, 0, 14, 11);
        LcdRect(28, 0, 29, 11);
        LcdRect(46, 0, 47, 11);
        LcdRect(61, 0, 62, 11);
    } else {
        LcdLine(40, 0, 40, LCD_Y_RES);
    }
}

void repaint() {
    LcdClear();
    drawBackground();
    drawForeground();
    LcdUpdate();
}


void globalInit() {
    portsInit();
    LcdInit();
    initTimer2();
    initInterrupts();
    readCfg(true);
    PIN_SET(LCD_LED_PORT, LCD_LED_PIN, CFG_BACKLIGHT);
    LcdContrast((byte) CFG_CONTRAST);
    sei();
}

int main(void) {
    globalInit();
    while (1) {

        if (ticksPrev != ticks) {
            ticksPrev = ticks;
            if (!(CFG_ALERT && alert) && !isMenuDisplayed) {
                if (CFG_SOUND && CFG_SOUND_DETECT) {
                    if (CFG_LED) PIN_ON(LED_PORT, LED_PIN);
                    if (CFG_SOUND_CLICKS) {
                        PIN_ON(SPEAKER_PORT, SPEAKER_PIN);
                        _delay_ms(1);
                        PIN_OFF(SPEAKER_PORT, SPEAKER_PIN);
                    } else {
                        beep(DEFAULT_BEEP_FREQ);
                    }
                    if (CFG_LED) PIN_OFF(LED_PORT, LED_PIN);
                } else if (CFG_LED) {
                    PIN_ON(LED_PORT, LED_PIN);
                    _delay_ms(3);
                    PIN_OFF(LED_PORT, LED_PIN);
                }
            }
        }

        if (measureFinished) {
            measureFinished = false;
            if (CFG_SOUND && CFG_SOUND_MEASURE && !isMenuDisplayed) beep(MEASURE_END_BEEP_FREQ);
        }

        if (isMenuDisplayed) {
            menuLoop();
            continue;
        }


        if (halfSeconds - lastRepaintTime >= IDLE_REPAINT_HALFSECONDS) {
            lastRepaintTime = halfSeconds;
            repaint();
        }

        if (OK_PRESSED) {
            while (OK_PRESSED) { }
            openMenu();
            _delay_ms(200);
        }
    }
}