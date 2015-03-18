#include "init.h"


void initTimer1() {
    TCCR1A = 0x00;
    TCCR1B = _BV(CS10) | _BV(WGM12);
    TIMSK |= _BV(OCIE1A);
}

void timer1Tone(int freq) {
    initTimer1();
    OCR1AH = (char) (freq >> 8);
    OCR1AL = (char) (freq);
}

void timer1off() {
    TCCR1B &= ~_BV(CS10);
    OCR1AH = 0x00;
    OCR1AL = 0x00;
    PIN_OFF(SPEAKER_PORT, SPEAKER_PIN);
}


void initTimer2() {
    ASSR |= _BV(AS2); //асинхронный режим
    TCNT2 = 0x00;
    //TCCR2 = _BV(CS20) | _BV(CS22); //предделитель 128, одна секунда
    TCCR2 =_BV(CS22); //предделитель 64, половина секунды
    OCR2 = 0x00;
    TIMSK |= _BV(TOIE2);
}

void portsInit(){

    PIN_AS_OUTPUT(LCD_LED_DDR, LCD_LED_PIN);
    PIN_AS_OUTPUT(SPEAKER_DDR, SPEAKER_PIN);
    PIN_AS_OUTPUT(LED_DDR, LED_PIN);

    PIN_AS_INPUT(UP_BUTTON_DDR, UP_BUTTON_PIN);
    PIN_AS_INPUT(DOWN_BUTTON_DDR, DOWN_BUTTON_PIN);
    PIN_AS_INPUT(BACK_BUTTON_DDR, BACK_BUTTON_PIN);
    PIN_AS_INPUT(OK_BUTTON_DDR, OK_BUTTON_PIN);

    //внутренн€€ подт€жка
    PIN_ON(UP_BUTTON_PORT, UP_BUTTON_PIN);
    PIN_ON(DOWN_BUTTON_PORT, DOWN_BUTTON_PIN);
    PIN_ON(BACK_BUTTON_PORT, BACK_BUTTON_PIN);
    PIN_ON(OK_BUTTON_PORT, OK_BUTTON_PIN);


}

void initInterrupts() {
    MCUCR = (1 << ISC01) | (1 << ISC00);
    GICR = (1 << INT0);
}
