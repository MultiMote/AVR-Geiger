#include "eep_config.h"
#include "easyavr.h"
#include "n3310.h"


uint8_t EEMEM EEP_CFG_MEASURE_TIME = CFG_MEASURE_TIME_DEFAULT;
uint8_t EEMEM EEP_CFG_CONTRAST = CFG_CONTRAST_DEFAULT;
uint8_t EEMEM EEP_CFG_LED = CFG_LED_DEFAULT;
uint8_t EEMEM EEP_CFG_BACKLIGHT = CFG_BACKLIGHT_DEFAULT;
uint8_t EEMEM EEP_CFG_SOUND = CFG_SOUND_DEFAULT;
uint8_t EEMEM EEP_CFG_SOUND_MEASURE = CFG_SOUND_MEASURE_DEFAULT;
uint8_t EEMEM EEP_CFG_SOUND_DETECT = CFG_SOUND_MEASURE_DEFAULT;
uint8_t EEMEM EEP_CFG_ALERT = CFG_ALERT_DEFAULT;
uint16_t EEMEM EEP_CFG_ALERT_VAL = CFG_ALERT_MIN_DEFAULT;
uint8_t EEMEM EEP_CFG_MINIMAL_GUI = CFG_MINIMAL_GUI_DEFAULT;
uint8_t EEMEM EEP_CFG_SOUND_CLICKS = CFG_SOUND_CLICKS_DEFAULT;
uint8_t EEMEM EEP_VERIFICATION_BYTES = BYTES_VERIFY;

void readCfg(bool verify) {
    if (verify && eeprom_read_byte(&EEP_VERIFICATION_BYTES) != BYTES_VERIFY) {
        resetCfg();
        readCfg(false);
        return;
    }

    CFG_SOUND = (bool) eeprom_read_byte(&EEP_CFG_SOUND);
    CFG_SOUND_MEASURE = (bool) eeprom_read_byte(&EEP_CFG_SOUND_MEASURE);
    CFG_SOUND_DETECT = (bool) eeprom_read_byte(&EEP_CFG_SOUND_DETECT);
    CFG_ALERT = (bool) eeprom_read_byte(&EEP_CFG_ALERT);
    CFG_LED = (bool) eeprom_read_byte(&EEP_CFG_LED);
    CFG_BACKLIGHT = (bool) eeprom_read_byte(&EEP_CFG_BACKLIGHT);
    CFG_MEASURE_TIME = eeprom_read_byte(&EEP_CFG_MEASURE_TIME);
    CFG_CONTRAST = eeprom_read_byte(&EEP_CFG_CONTRAST);
    CFG_ALERT_MIN = eeprom_read_word(&EEP_CFG_ALERT_VAL);
    CFG_MINIMAL_GUI = (bool) eeprom_read_byte(&EEP_CFG_MINIMAL_GUI);
    CFG_SOUND_CLICKS = (bool) eeprom_read_byte(&EEP_CFG_SOUND_CLICKS);
}

void saveCfg() {
    eeprom_update_byte(&EEP_CFG_SOUND, CFG_SOUND);
    eeprom_update_byte(&EEP_CFG_SOUND_MEASURE, CFG_SOUND_MEASURE);
    eeprom_update_byte(&EEP_CFG_SOUND_DETECT, CFG_SOUND_DETECT);
    eeprom_update_byte(&EEP_CFG_ALERT, CFG_ALERT);
    eeprom_update_byte(&EEP_CFG_LED, CFG_LED);
    eeprom_update_byte(&EEP_CFG_BACKLIGHT, CFG_BACKLIGHT);
    eeprom_update_byte(&EEP_CFG_MEASURE_TIME, CFG_MEASURE_TIME);
    eeprom_update_byte(&EEP_CFG_CONTRAST, CFG_CONTRAST);
    eeprom_update_word(&EEP_CFG_ALERT_VAL, CFG_ALERT_MIN);
    eeprom_update_byte(&EEP_CFG_MINIMAL_GUI, CFG_MINIMAL_GUI);
    eeprom_update_byte(&EEP_CFG_SOUND_CLICKS, CFG_SOUND_CLICKS);
    PIN_SET(LCD_LED_PORT, LCD_LED_PIN, CFG_BACKLIGHT);
    LcdContrast((unsigned char) CFG_CONTRAST);

}

void resetCfg() {
    eeprom_write_byte(&EEP_VERIFICATION_BYTES, BYTES_VERIFY);
    eeprom_write_byte(&EEP_CFG_SOUND, CFG_SOUND_DEFAULT);
    eeprom_write_byte(&EEP_CFG_SOUND_MEASURE, CFG_SOUND_MEASURE_DEFAULT);
    eeprom_write_byte(&EEP_CFG_SOUND_DETECT, CFG_SOUND_DETECT_DEFAULT);
    eeprom_write_byte(&EEP_CFG_ALERT, CFG_ALERT_DEFAULT);
    eeprom_write_byte(&EEP_CFG_LED, CFG_LED_DEFAULT);
    eeprom_write_byte(&EEP_CFG_BACKLIGHT, CFG_BACKLIGHT_DEFAULT);
    eeprom_write_byte(&EEP_CFG_MEASURE_TIME, CFG_MEASURE_TIME_DEFAULT);
    eeprom_write_byte(&EEP_CFG_CONTRAST, CFG_CONTRAST_DEFAULT);
    eeprom_write_word(&EEP_CFG_ALERT_VAL, CFG_ALERT_MIN_DEFAULT);
    eeprom_write_byte(&EEP_CFG_MINIMAL_GUI, CFG_MINIMAL_GUI_DEFAULT);
    eeprom_write_byte(&EEP_CFG_SOUND_CLICKS, CFG_SOUND_CLICKS_DEFAULT);
}
