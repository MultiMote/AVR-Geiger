#include "config.h"

uint8_t EEMEM EEP_CFG_MEASURE_TIME = DEFAULT_MEASURE_TIME;
uint8_t EEMEM EEP_CFG_CONTRAST = DEFAULT_CONTRAST;
uint8_t EEMEM EEP_CFG_LED = true;
uint8_t EEMEM EEP_CFG_BACKLIGHT = false;
uint8_t EEMEM EEP_CFG_SOUND = true;
uint8_t EEMEM EEP_CFG_ALARM = false;
uint16_t EEMEM EEP_CFG_ALARM_VAL = 200;

void readCfg() {
    CFG_SOUND = (bool)eeprom_read_byte(&EEP_CFG_SOUND);
    CFG_ALARM = (bool)eeprom_read_byte(&EEP_CFG_ALARM);
    CFG_LED = (bool)eeprom_read_byte(&EEP_CFG_LED);
    CFG_BACKLIGHT = (bool)eeprom_read_byte(&EEP_CFG_BACKLIGHT);
    CFG_MEASURE_TIME = eeprom_read_byte(&EEP_CFG_MEASURE_TIME);
    CFG_CONTRAST = eeprom_read_byte(&EEP_CFG_CONTRAST);
    CFG_ALARM_VAL = eeprom_read_word(&EEP_CFG_ALARM_VAL);
}
