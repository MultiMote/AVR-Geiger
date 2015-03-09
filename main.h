#ifndef __MAIN_H_
#define __MAIN_H_

#include <avr/io.h>
#include <avr/pgmspace.h>
#include <avr/eeprom.h>
#include <util/delay.h>
#include <stdio.h>
#include <stdbool.h>
#include <avr/interrupt.h>
#include <string.h>

#include "n3310.h"
#include "easyavr.h"
#include "defines.h"
#include "icons.h"
#include "init.h"

char buf[16];
uint16_t seconds;
uint16_t halfSeconds;
uint16_t minutes;
uint16_t lastRepaintTime;

uint32_t ticks;
uint32_t ticksPrev;

uint32_t measure;
uint32_t lastMeasure;
uint32_t measureTimeStart;

bool halfSecond;
bool alarm;
bool _alarm;

uint8_t EEMEM measureTime=15;

void globalInit();
void repaint();
void onHalfSecond();
void drawForeground();
void drawBackground();
void beep(int freq);
int main(void);

uint32_t convertCPM(uint32_t clicks);

#endif



