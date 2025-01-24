
#include "tm1637.h"

#if TM1637_ENABLED && TM1637_SINGLETON

#include "protocol.h"
#include "ll.h"

tm1637_t tm1637 = {
    .state = {
        .brightness = TM1637_DISP_BR10,
        .display_on = true
    }
};

static void tm1637_write_command(uint8_t command) {
    _tm1637_ll_start();
    _tm1637_ll_write_byte(command);
    _tm1637_ll_stop();
}

__weak void tm1637_remap(uint8_t content[]) {
    ;
}

void tm1637_set_display_content(uint8_t content[]) {
    uint_fast8_t cursor;
    tm1637_remap(content);
    tm1637_write_command(TM1637_CMD_PREFIX_DATA | TM1637_CMD_DATA_WRITE | TM1637_CMD_DATA_INCADDR);
    _tm1637_ll_start();
    _tm1637_ll_write_byte(TM1637_CMD_PREFIX_ADDRESS | TM1637_ADDR_C0H);

    cursor = 0;
    while (cursor < TM1637_NUM_DIGITS){
        _tm1637_ll_write_byte(content[cursor]);
        cursor++;
    }   

    _tm1637_ll_stop();
}

void tm1637_set_display_number(int16_t number, seg7_pos_spec_t pos_spec) {
    uint8_t buffer[TM1637_NUM_DIGITS] = {SEG7_CLEAR};
    if (seg7_prep_display_buffer(number, pos_spec, buffer, TM1637_NUM_DIGITS)){
        tm1637_set_display_content(buffer);        
    };
}

static inline void tm1637_write_display_settings(void) {
    uint8_t cmd = TM1637_CMD_PREFIX_DISPLAY | tm1637.state.brightness;
    if (tm1637.state.display_on) {
        cmd |= TM1637_CMD_DISP_ON;
    } else {
        cmd |= TM1637_CMD_DISP_OFF;
    }
    tm1637_write_command(cmd);
}

void tm1637_set_display_state(bool state) {
    if (state != tm1637.state.display_on) {
        tm1637.state.display_on = state;
        tm1637_write_display_settings();
    }
}

bool tm1637_get_display_state(void) {
    return tm1637.state.display_on;
}

void tm1637_set_display_brightness(tm1637_brightness_t brightness) {
    if (brightness != tm1637.state.brightness) {
        tm1637.state.brightness = brightness;
        tm1637_write_display_settings();
    }
}

tm1637_brightness_t tm1637_get_display_brightness(void) {
    return tm1637.state.brightness;
}

void tm1637_init(void) {
    _tm1637_ll_init();
    uint8_t _content[TM1637_NUM_DIGITS] = { [0 ... TM1637_NUM_DIGITS - 1] = 0xFF };
    tm1637_set_display_content(_content);
    tm1637_write_display_settings();
}

#endif
