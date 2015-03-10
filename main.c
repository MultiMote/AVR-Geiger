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
// ����������
//***********************************************************
ISR(TIMER2_OVF_vect) {
    halfSecond = !halfSecond;
    if (!halfSecond) seconds++;
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

    if (halfSecond) {
        pushToStream(ticks - ticksMeasure);
        ticksMeasure = ticks;
    }

    measureByTime = convertCPM((uint32_t) ((float) (ticks - ticksTimeMeasure) * (60.0F / (float) CFG_MEASURE_TIME)));

    if (seconds - measureTimeStart >= DEFAULT_MEASURE_TIME) {
        measureTimeStart = seconds;
        ticksTimeMeasure = ticks;
        lastMeasure = measureByTime;
        measureFinished = true;
    }

    if (measureByStream >= CFG_ALARM_VAL /*|| lastMeasure >= max*/) alarm = true;
    else alarm = false;

    uint16_t sum = 0;
    for (int i = 0; i < MEASURE_STREAM_SIZE; i++) {
        sum += clicksStream[i];
    }

    measureByStream = convertCPM((uint32_t) ((float) sum * (60.0F / (float) MEASURE_STREAM_SIZE)));;

    if (alarm) {
        if (halfSecond) {
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
    sprintf(buf, "%04u", measureByStream);
    LcdGotoXYFont(1, 3);
    LcdStr(FONT_2X, buf, 6);

    sprintf(buf, "%04u", lastMeasure);
    LcdGotoXYFont(8, 3);
    LcdStr(FONT_2X, buf, 6);

    LcdGotoXYFont(1, 4);
    LcdStr(FONT_1X, MKR_H, DEFAULT_MARGIN);
    LcdGotoXYFont(8, 4);
    LcdStr(FONT_1X, MKR_H, DEFAULT_MARGIN);

}

void drawForeground() {
    LCDIcon(&resultFrame[0][0], 2, 19, 3, 45, true);
    LCDIcon(&lastFrame[0][0], 41, 19, 3, 39, true);


    LCDIcon(&led_icon[0][0], 4, 3, 1, 5, true);
    LCDIcon(&speaker_icon[0][0], 16, 1, 2, 6, true);
    LCDIcon(&display_icon[0][0], 35, 1, 2, 9, true);
    LCDIcon(&alarm_icon[0][0], 51, 3, 1, 7, true);

    if (CFG_LED)LCDIcon(&highlight[0][0], 1, 0, 1, 11, true);  // ���������
    if (CFG_BACKLIGHT)LCDIcon(&highlight[0][0], 32, 2, 1, 11, true); // ��������� �������
    if (CFG_ALARM)LCDIcon(&highlight[0][0], 49, 0, 1, 11, true); // ������


    if (CFG_SOUND)LCDIcon(&loud[0][0], 23, 3, 1, 8, true);
    else LCDIcon(&mute[0][0], 23, 4, 1, 4, true);


    uint8_t sec = halfSeconds + 1;
    byte pos1 = (((sec - 1) / 8)) % 8;

    LcdLine(CLOCK_CENTER_X, CLOCK_CENTER_Y, clockPosX[pos1], clockPosY[pos1], PIXEL_ON);
    byte pos2 = sec % 8;
    LcdLine(CLOCK_CENTER_X, CLOCK_CENTER_Y, clockPosX[pos2], clockPosY[pos2], PIXEL_ON);

    LcdRect(0, 12, 83, 13, PIXEL_ON);
    LcdRect(13, 0, 14, 11, PIXEL_ON);
    LcdRect(28, 0, 29, 11, PIXEL_ON);
    LcdRect(46, 0, 47, 11, PIXEL_ON);
    LcdRect(61, 0, 62, 11, PIXEL_ON);

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
    readCfg();
    LcdContrast((byte) CFG_CONTRAST);
    sei();
}

int main(void) {
    globalInit();
    while (1) {

        if (halfSeconds - lastRepaintTime >= IDLE_REPAINT_HALFSECONDS) {
            lastRepaintTime = halfSeconds;
            repaint();
        }

        if (measureFinished) {
            measureFinished = false;
            if (CFG_SOUND) beep(MEASURE_END_BEEP_FREQ);
        }

        if (ticksPrev != ticks) {
            ticksPrev = ticks;
            if (!alarm) {
                if (CFG_SOUND) {
                    if (CFG_LED) PIN_ON(LED_PORT, LED_PIN);
                    beep(DEFAULT_BEEP_FREQ);
                    if (CFG_LED) PIN_OFF(LED_PORT, LED_PIN);
                } else if (CFG_LED) {
                    PIN_ON(LED_PORT, LED_PIN);
                    _delay_ms(3);
                    PIN_OFF(LED_PORT, LED_PIN);
                }
            }
        }
    }
}