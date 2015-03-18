#ifndef __MAIN_H_
#define __MAIN_H_

#include <avr/io.h>
#include <avr/eeprom.h>
#include <util/delay.h>
//#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <avr/interrupt.h>

#include "n3310.h"
#include "easyavr.h"
#include "defines.h"
#include "icons.h"
#include "init.h"
#include "eep_config.h"
#include "menu.h"


char buf[16];
uint16_t seconds;
uint16_t halfSeconds;
uint16_t minutes;
uint16_t lastRepaintTime;

uint32_t ticks;
uint32_t ticksMeasure;
uint32_t ticksTimeMeasure;
uint32_t ticksPrev;

uint32_t measureByStream;
uint32_t measureByTime;
uint32_t lastMeasure;
uint32_t measureTimeStart;

bool isHalfSecond;
bool alert;
bool _alarm;
bool measureFinished;

uint16_t clicksStream[MEASURE_STREAM_SIZE];

void globalInit();
void repaint();
void onHalfSecond();
void drawForeground();
void drawBackground();
void beep(int freq);
void pushToStream(uint16_t clicks);
int main(void);

uint32_t convertCPM(uint32_t clicks);

#endif



