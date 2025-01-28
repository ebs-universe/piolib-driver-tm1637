
#include "tm1637.h"

#if TM1637_ENABLED && !TM1637_SINGLETON

#include "protocol.h"
#include "ll.h"
#include <string.h>
#include <stdlib.h>

static void tm1637_write_command(tm1637_t * tm1637, uint8_t command) {
    _tm1637_ll_start(tm1637);
    _tm1637_ll_write_byte(tm1637, command);
    _tm1637_ll_stop(tm1637);
}

void tm1637_set_display_content(tm1637_t * tm1637, uint8_t content[]) {
    uint_fast8_t cursor;
    if (tm1637->hwif.remap) { tm1637->hwif.remap(content); };

    tm1637_write_command(tm1637, TM1637_CMD_PREFIX_DATA | TM1637_CMD_DATA_WRITE | TM1637_CMD_DATA_INCADDR);
    _tm1637_ll_start(tm1637);
    _tm1637_ll_write_byte(tm1637, TM1637_CMD_PREFIX_ADDRESS | TM1637_ADDR_C0H);

    cursor = 0;
    while (cursor < TM1637_NUM_DIGITS){
        _tm1637_ll_write_byte(tm1637, content[cursor]);
        cursor++;
    }   

    _tm1637_ll_stop(tm1637);
}

void tm1637_set_display_number(tm1637_t * tm1637, int16_t number, seg7_pos_spec_t pos_spec) {
    uint8_t buffer[TM1637_NUM_DIGITS] = {SEG7_CLEAR};
    if (seg7_prepbuf_number(buffer, tm1637->hwif.len, number, pos_spec)){
        tm1637_set_display_content(tm1637, buffer);        
    };
}

int tm1637_vprintf(tm1637_t * tm1637, const char *format, va_list args) {
    uint8_t buffer[TM1637_NUM_DIGITS] = {SEG7_CLEAR};
    int rval = seg7_vprintf((char *)buffer, TM1637_NUM_DIGITS, format, args);
    tm1637_set_display_content(tm1637_t * tm1637, buffer);
    return rval;
}

static inline void tm1637_write_display_settings(tm1637_t * tm1637) {
    uint8_t cmd = TM1637_CMD_PREFIX_DISPLAY | tm1637->state.brightness;
    if (tm1637->state.display_on) {
        cmd |= TM1637_CMD_DISP_ON;
    } else {
        cmd |= TM1637_CMD_DISP_OFF;
    }
    tm1637_write_command(tm1637, cmd);
}

void tm1637_set_display_state(tm1637_t * tm1637, bool state) {
    if (state != tm1637->state.display_on) {
        tm1637->state.display_on = state;
        tm1637_write_display_settings(tm1637);
    }
}

bool tm1637_get_display_state(tm1637_t * tm1637) {
    return tm1637->state.display_on;
}

void tm1637_set_display_brightness(tm1637_t * tm1637, tm1637_brightness_t brightness) {
    if (brightness != tm1637->state.brightness) {
        tm1637->state.brightness = brightness;
        tm1637_write_display_settings(tm1637);
    }
}

tm1637_brightness_t tm1637_get_display_brightness(tm1637_t * tm1637) {
    return tm1637->state.brightness;
}

void tm1637_init(tm1637_t * tm1637) {
    _tm1637_ll_init(tm1637);
    uint8_t *_content = (uint8_t *)malloc(tm1637->hwif.len * sizeof(uint8_t));
    memset(_content, 0xFF, tm1637->hwif.len * sizeof(uint8_t));
    tm1637_set_display_content(tm1637, _content);
    tm1637_write_display_settings(tm1637);
    free(_content);
}

#endif
