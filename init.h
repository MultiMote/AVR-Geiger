#ifndef __INIT_H_
#define __INIT_H_

#include <avr/io.h>
#include "easyavr.h"
#include "defines.h"

void portsInit();
void initTimer1();
void initTimer2();
void timer1Tone(int freq);
void timer1off();
void initInterrupts();
#endif //__INIT_H_
