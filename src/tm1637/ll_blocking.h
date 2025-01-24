
#ifndef TM1637_LL_BLOCKING_H
#define TM1637_LL_BLOCKING_H

#include "ll_common.h"

#if !TM1637_IMPL_SM

#pragma GCC push_options 
#pragma GCC optimize("O0")

#if TM1637_SINGLETON
static inline void _tm1637_ll_init(void) {
    _tm1637_ll_init_common();
}
#else
static inline void _tm1637_ll_init(tm1637_t * tm1637) {
    _tm1637_ll_init_common(tm1637);
}
#endif

#if TM1637_SINGLETON
static inline void _tm1637_ll_start(void)
#else
static inline void _tm1637_ll_start(tm1637_t * tm1637)
#endif
{
    gpio_set_output_high(_GPIO_CLK);
    gpio_set_output_high(_GPIO_DAT);
    qdelay_us(1);
    gpio_set_output_low(_GPIO_DAT);
}

#if TM1637_SINGLETON
static inline void _tm1637_ll_stop(void)
#else 
static inline void _tm1637_ll_stop(tm1637_t * tm1637)
#endif
{
    gpio_set_output_low(_GPIO_CLK);
    qdelay_us(1);
    gpio_set_output_low(_GPIO_DAT);
    qdelay_us(1);
    gpio_set_output_high(_GPIO_CLK);
    qdelay_us(1);
    gpio_set_output_high(_GPIO_DAT);  
    // qdelay_us(5);      // DEBUG ONLY  
}

#if TM1637_SINGLETON
static inline void _tm1637_ll_write_byte(uint8_t data)
#else
static inline void _tm1637_ll_write_byte(tm1637_t * tm1637, uint8_t data)
#endif
{
    uint_fast8_t i;

    for (i = 0; i < 8; i++) {
        gpio_set_output_low(_GPIO_CLK);
        if (data & 1)
            gpio_set_output_high(_GPIO_DAT);
        else
            gpio_set_output_low(_GPIO_DAT);
        qdelay_us(1);
        data >>= 1;
        gpio_set_output_high(_GPIO_CLK);
        qdelay_us(1);
    }

    gpio_conf_input(_GPIO_DAT);
    gpio_set_output_low(_GPIO_CLK);
    qdelay_us(1);
    
    i = 8;
    while (gpio_get_input(_GPIO_DAT) && i) {
        qdelay_us(1);
        i --;
        if (!i) { die(); };
    }
    
    gpio_set_output_high(_GPIO_CLK);
    qdelay_us(1);
    gpio_set_output_low(_GPIO_CLK);

    gpio_conf_output(_GPIO_DAT);
    gpio_set_output_high(_GPIO_DAT);
    // qdelay_us(5);      // DEBUG ONLY  
}

#pragma GCC pop_options 
#endif
#endif