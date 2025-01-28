/* 
   Copyright (C) 2025 Chintalagiri Shashank
   Copyright (C) 2020 Daniel Efimenko
    
   This library is free software: you can redistribute it and/or modify
   it under the terms of the GNU Lesser General Public License as published
   by the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.
   
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.
   
   You should have received a copy of the GNU Lesser General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>. 
*/

/**
 * @file tm1637.h
 * @brief TM1637 7 segment LCD driver
 *
 * This implementation is based on the MIT licensed implementation by 
 * Daniel Efimenko published at https://github.com/Danya0x07/tm1637-simple-library
 * 
 * Time for writing a 6 digit display:  
 *  - in blocking mode: ~440us
 *  - in sm mode with no other load : ~720us
 * 
 */

#ifndef TM1637_H
#define TM1637_H

#include "config.h"

#if TM1637_ENABLED

#if !TM1637_SINGLETON
    #include <hal/uc/gpio.h>
#endif

#if TM1637_IMPL_SM
    #include <ds/bytebuf.h>
    #include <hal/uc/qdelay.h>
    #ifndef TM1637_OP_QUEUE_BUFFER_LEN
    #define TM1637_OP_QUEUE_BUFFER_LEN  32
    #endif
#endif

#include <seg7/seg7.h>
#include "protocol.h"

#if !TM1637_SINGLETON
typedef struct TM1637_HWIF_t {
    const uint8_t len;
    const bool has_display;
    const bool has_keys;
    const gpio_t gpio_clk;
    const gpio_t gpio_dio;
    void (*const remap)(uint8_t []);
} tm1637_hwif_t;
#endif

typedef struct TM1637_STATE_t {
    tm1637_brightness_t brightness;
    bool display_on;
    bool keys_on;
    #if TM1637_IMPL_SM
    uint8_t current_op;
    uint8_t current_op_stage;
    uint8_t current_op_byte;
    qdelay_t delay;
    bytebuf op_queue;
    uint8_t op_queue_buffer[TM1637_OP_QUEUE_BUFFER_LEN];
    #endif
} tm1637_state_t;

typedef struct TM1637_t {
    #if !TM1637_SINGLETON
    tm1637_hwif_t hwif;
    #endif
    tm1637_state_t state;
} tm1637_t;

#if TM1637_SINGLETON

extern tm1637_t tm1637;

void tm1637_init(void);
void tm1637_set_display_number(int16_t number, seg7_pos_spec_t pos_spec);
int tm1637_vprintf(const char *format, va_list args);

static inline int tm1637_printf(const char *format, ...){
    int rval;
    va_list args;
    va_start( args, format );
    rval = tm1637_vprintf(format, args);
    va_end(args);       
    return rval;
}

void tm1637_set_display_content(uint8_t content[]);
void tm1637_set_display_state(bool state);
bool tm1637_get_display_state(void);
void tm1637_set_display_brightness(tm1637_brightness_t brightness);
tm1637_brightness_t tm1637_get_display_brightness(void);

#if TM1637_IMPL_SM
void tm1637_state_machine(void);
#endif

#else 

void tm1637_init(tm1637_t * tm1637);
void tm1637_set_display_number(tm1637_t * tm1637, int16_t number, seg7_pos_spec_t pos_spec);
void tm1637_vprintf(tm1637_t * tm1637, const char *format, va_list args);

static inline void tm1637_printf(tm1637_t * tm1637, const char *format, ...){
    va_list args;
    va_start( args, format );
    tm1637_vprintf(tm1637_t * tm1637, format, args);
    va_end(args);       
}

void tm1637_set_display_content(tm1637_t * tm1637, uint8_t content[]);

void tm1637_set_display_state(tm1637_t * tm1637, bool state);
bool tm1637_get_display_state(tm1637_t * tm1637);
void tm1637_set_display_brightness(tm1637_t * tm1637, tm1637_brightness_t brightness);
tm1637_brightness_t tm1637_get_display_brightness(tm1637_t * tm1637);

#if TM1637_IMPL_SM
void tm1637_state_machine(tm1637_t * tm1637);
#endif 

#endif

#endif
#endif