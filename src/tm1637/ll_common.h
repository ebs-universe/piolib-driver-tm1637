
#ifndef TM1637_LL_COMMON_H
#define TM1637_LL_COMMON_H

#include "tm1637.h"
#include <hal/uc/gpio.h>
#include <hal/uc/qdelay.h>

#if TM1637_SINGLETON

static inline void _tm1637_ll_init();
static inline void _tm1637_ll_start();
static inline void _tm1637_ll_stop();
static inline void _tm1637_ll_write_byte(uint8_t byte);

#define _GPIO_CLK       GPIO_TM1637_CLK
#define _GPIO_DAT       GPIO_TM1637_DAT
#define _TM1637_STATE   tm1637.state
#define _TM1637p        &tm1637

#else 

static inline void _tm1637_ll_init(tm1637_t * tm1637);
static inline void _tm1637_ll_start(tm1637_t * tm1637);
static inline void _tm1637_ll_stop(tm1637_t * tm1637);
static inline void _tm1637_ll_write_byte(tm1637_t * tm1637, uint8_t byte);

#define _GPIO_CLK   tm1637->hwif.gpio_clk.port, tm1637->hwif.gpio_clk.pin 
#define _GPIO_DAT   tm1637->hwif.gpio_dio.port, tm1637->hwif.gpio_dio.pin
#define _TM1637_STATE   tm1637->state
#define _TM1637p        tm1637

#endif

#if TM1637_SINGLETON
static inline void _tm1637_ll_init_common(){
#else 
static inline void _tm1637_ll_init_common(tm1637_t * tm1637){
#endif
    gpio_set_output_high(_GPIO_CLK);
    gpio_conf_output(_GPIO_CLK);
    gpio_conf_speed(_GPIO_CLK, GPIO_SPEED_FREQ_LOW);
    gpio_conf_outmode_pp(_GPIO_CLK);
    
    gpio_set_output_high(_GPIO_DAT);
    gpio_conf_output(_GPIO_DAT);
    gpio_conf_speed(_GPIO_DAT, GPIO_SPEED_FREQ_LOW);
    gpio_conf_outmode_pp(_GPIO_DAT);
}

#endif