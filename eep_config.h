#ifndef __CONFIG_H_
#define __CONFIG_H_

#include <avr/eeprom.h>
#include <stdbool.h>
#include "defines.h"

#define BYTES_VERIFY 0xB3

#define DEFAULT_BEEP_FREQ 3980
#define MEASURE_END_BEEP_FREQ 2500
#define CFG_MEASURE_TIME_DEFAULT 20
#define CFG_CONTRAST_DEFAULT 0x3F
#define CFG_LED_DEFAULT true
#define CFG_BACKLIGHT_DEFAULT false
#define CFG_SOUND_DEFAULT true
#define CFG_SOUND_MEASURE_DEFAULT true
#define CFG_SOUND_DETECT_DEFAULT true
#define CFG_ALERT_DEFAULT true
#define CFG_ALERT_MIN_DEFAULT 200
#define CFG_MINIMAL_GUI_DEFAULT false
#define CFG_SOUND_CLICKS_DEFAULT false


uint8_t CFG_MEASURE_TIME;
uint8_t CFG_CONTRAST;
bool CFG_LED;
bool CFG_BACKLIGHT;
bool CFG_SOUND;
bool CFG_SOUND_MEASURE;
bool CFG_SOUND_DETECT;
bool CFG_ALERT;
uint16_t CFG_ALERT_MIN;
bool CFG_MINIMAL_GUI;
bool CFG_SOUND_CLICKS;


void readCfg(bool verify);

void saveCfg();

void resetCfg();

#endif //__CONFIG_H_
