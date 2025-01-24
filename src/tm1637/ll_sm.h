
#ifndef TM1637_LL_SM_H
#define TM1637_LL_SM_H

#include "ll_common.h"

#if TM1637_IMPL_SM

#pragma GCC push_options 
#pragma GCC optimize("O0")

#define TM1637_LLOP_WBYTE   0xFC
#define TM1637_LLOP_START   0xFD
#define TM1637_LLOP_STOP    0xFE

#if TM1637_SINGLETON
static inline void _tm1637_ll_init(void) {
    _tm1637_ll_init_common();
    bytebuf_vInit(tm1637.state.op_queue_buffer, 
                  TM1637_OP_QUEUE_BUFFER_LEN, 
                  &tm1637.state.op_queue);
}
#else
static inline void _tm1637_ll_init(tm1637_t * tm1637) {
    _tm1637_ll_init_common(tm1637);
    bytebuf_vInit(tm1637->state.op_queue_buffer, 
                  TM1637_OP_QUEUE_BUFFER_LEN, 
                  &tm1637->state.op_queue);
}
#endif

#if TM1637_SINGLETON
static inline void _tm1637_ll_start(void)
#else
static inline void _tm1637_ll_start(tm1637_t * tm1637)
#endif
{
    if (bytebuf_cPushReqLock(&_TM1637_STATE.op_queue, 1, 0x01)){
        bytebuf_cPushByte(&_TM1637_STATE.op_queue, TM1637_LLOP_START, 0x01);
    }
}

#if TM1637_SINGLETON
static inline void _tm1637_ll_stop(void)
#else 
static inline void _tm1637_ll_stop(tm1637_t * tm1637)
#endif
{
    if (bytebuf_cPushReqLock(&_TM1637_STATE.op_queue, 1, 0x01)){
        bytebuf_cPushByte(&_TM1637_STATE.op_queue, TM1637_LLOP_STOP, 0x01);
    }
}   

#if TM1637_SINGLETON
static inline void _tm1637_ll_write_byte(uint8_t data)
#else
static inline void _tm1637_ll_write_byte(tm1637_t * tm1637, uint8_t data)
#endif
{
    if (bytebuf_cPushReqLock(&_TM1637_STATE.op_queue, 2, 0x01)){
        if (data >= 0xFC && data <= 0xFE){
            bytebuf_cPushByte(&_TM1637_STATE.op_queue, TM1637_LLOP_WBYTE, 0x01);    
        }
        bytebuf_cPushByte(&_TM1637_STATE.op_queue, data, 0x01);
        bytebuf_cPushRelinquishLock(&_TM1637_STATE.op_queue, 0x01);
    }
}

#pragma GCC pop_options 
#endif
#endif