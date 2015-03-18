#include "eep_config.h"

uint8_t EEMEM EEP_CFG_MEASURE_TIME = DEFAULT_MEASURE_TIME;
uint8_t EEMEM EEP_CFG_CONTRAST = DEFAULT_CONTRAST;
uint8_t EEMEM EEP_CFG_LED = true;
uint8_t EEMEM EEP_CFG_BACKLIGHT = false;
uint8_t EEMEM EEP_CFG_SOUND = true;
uint8_t EEMEM EEP_CFG_SOUND_MEASURE = true;
uint8_t EEMEM EEP_CFG_SOUND_DETECT = true;
uint8_t EEMEM EEP_CFG_ALARM = true;
uint16_t EEMEM EEP_CFG_ALARM_VAL = 200;
uint8_t EEMEM EEP_CFG_MINIMAL_GUI = false;

void readCfg() {
    CFG_SOUND = (bool) eeprom_read_byte(&EEP_CFG_SOUND);
    CFG_SOUND_MEASURE = (bool) eeprom_read_byte(&EEP_CFG_SOUND_MEASURE);
    CFG_SOUND_DETECT = (bool) eeprom_read_byte(&EEP_CFG_SOUND_DETECT);
    CFG_ALERT = (bool) eeprom_read_byte(&EEP_CFG_ALARM);
    CFG_LED = (bool) eeprom_read_byte(&EEP_CFG_LED);
    CFG_BACKLIGHT = (bool) eeprom_read_byte(&EEP_CFG_BACKLIGHT);
    CFG_MEASURE_TIME = eeprom_read_byte(&EEP_CFG_MEASURE_TIME);
    CFG_CONTRAST = eeprom_read_byte(&EEP_CFG_CONTRAST);
    CFG_ALERT_VAL = eeprom_read_word(&EEP_CFG_ALARM_VAL);
    CFG_MINIMAL_GUI = (bool) eeprom_read_byte(&EEP_CFG_MINIMAL_GUI);
}

void saveCfg() {
    eeprom_update_byte(&EEP_CFG_SOUND, CFG_SOUND);
    eeprom_update_byte(&EEP_CFG_SOUND_MEASURE, CFG_SOUND_MEASURE);
    eeprom_update_byte(&EEP_CFG_SOUND_DETECT, CFG_SOUND_DETECT);
    eeprom_update_byte(&EEP_CFG_ALARM, CFG_ALERT);
    eeprom_update_byte(&EEP_CFG_LED, CFG_LED);
    eeprom_update_byte(&EEP_CFG_BACKLIGHT, CFG_BACKLIGHT);
    eeprom_update_byte(&EEP_CFG_MEASURE_TIME, CFG_MEASURE_TIME);
    eeprom_update_byte(&EEP_CFG_CONTRAST, CFG_CONTRAST);
    eeprom_update_word(&EEP_CFG_ALARM_VAL, CFG_ALERT_VAL);
    eeprom_update_byte(&EEP_CFG_MINIMAL_GUI, CFG_MINIMAL_GUI);

}
