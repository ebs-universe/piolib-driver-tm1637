

#ifndef TM1637_LL_H
#define TM1637_LL_H

#include "tm1637.h"

#if TM1637_IMPL_SM
    #include "ll_sm.h"
#else 
    #include "ll_blocking.h"
#endif
#endif