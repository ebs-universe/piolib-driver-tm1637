

#ifndef TM1637_CONFIG_H
#define TM1637_CONFIG_H

#include <application.h>
#include <iomap.h>

#ifdef APP_ENABLE_TM1637
    #define TM1637_ENABLED          APP_ENABLE_TM1637
#else
    #define TM1637_ENABLED          0
#endif

#if TM1637_ENABLED

#ifndef TM1637_ENABLE_DISPLAY
    #define TM1637_ENABLE_DISPLAY       1
#endif

#ifndef TM1637_NUM_DIGITS
    #define TM1637_NUM_DIGITS           4
#endif

#ifndef TM1637_IMPL_SM
    #define TM1637_IMPL_SM              0
#endif

#ifndef TM1637_SINGLETON
    #define TM1637_SINGLETON            1
#endif

#ifndef GPIO_TM1637_CLK
    #error "TM1637 Clock GPIO not defined"
#endif

#ifndef GPIO_TM1637_DAT
    #error "TM1637 Data GPIO not defined"
#endif

#endif
#endif