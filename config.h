#ifndef __CONFIG_H_
#define __CONFIG_H_
#include <avr/eeprom.h>
#include <stdbool.h>
#include "defines.h"

uint8_t CFG_MEASURE_TIME;
uint8_t CFG_CONTRAST;
bool CFG_LED;
bool CFG_BACKLIGHT;
bool CFG_SOUND;
bool CFG_ALARM;
uint16_t CFG_ALARM_VAL;

void readCfg();
void saveCfg();

#endif //__CONFIG_H_
