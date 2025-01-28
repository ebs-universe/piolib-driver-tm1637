

#ifdef _LL_COMPILATION_UNIT 
#if TM1637_IMPL_SM

// Executor will be called when the previous delay, if any, 
// is complete. The executor should execute the next stage, 
// increment the stage, and setup the appropriate delay. 
// If the last stage is complete, the executor should clear 
// the operation and return with no pending delay.

#if TM1637_SINGLETON
static void _tm1637_sm_executor_start(void)
#else
static void _tm1637_sm_executor_start(tm1637_t * tm1637)
#endif
{
    switch (_TM1637_STATE.current_op_stage)
    {
    case 0:
        gpio_set_output_high(_GPIO_CLK);
        gpio_set_output_high(_GPIO_DAT);
        qdelay_start(&_TM1637_STATE.delay, 1);
        _TM1637_STATE.current_op_stage = 1;
        break;
    case 1:
        gpio_set_output_low(_GPIO_DAT);
        _TM1637_STATE.current_op = 0;
        break;
    default:
        die();
    }
}

#if TM1637_SINGLETON
static void _tm1637_sm_executor_stop(void)
#else
static void _tm1637_sm_executor_stop(tm1637_t * tm1637)
#endif
{
    switch (_TM1637_STATE.current_op_stage)
    {
    case 0:
        gpio_set_output_low(_GPIO_CLK);
        qdelay_start(&_TM1637_STATE.delay, 1);
        _TM1637_STATE.current_op_stage = 1;
        break;
    case 1:
        gpio_set_output_low(_GPIO_DAT);
        qdelay_start(&_TM1637_STATE.delay, 1);
        _TM1637_STATE.current_op_stage = 2;
        break;
    case 2:
        gpio_set_output_high(_GPIO_CLK);
        qdelay_start(&_TM1637_STATE.delay, 1);
        _TM1637_STATE.current_op_stage = 3;
        break;
    case 3:
        gpio_set_output_high(_GPIO_DAT);  
        _TM1637_STATE.current_op = 0;
        break;
    default:
        die();
    }
}

#if TM1637_SINGLETON
static void _tm1637_sm_executor_wbyte(void)
#else
static void _tm1637_sm_executor_wbyte(tm1637_t * tm1637)
#endif
{
    if (_TM1637_STATE.current_op_stage < 16U) {
        if (_TM1637_STATE.current_op_stage % 2) {
            gpio_set_output_high(_GPIO_CLK);
            qdelay_start(&_TM1637_STATE.delay, 1);
            _TM1637_STATE.current_op_stage += 1;
        } else {
            gpio_set_output_low(_GPIO_CLK);
            if (_TM1637_STATE.current_op_byte & (1U << (_TM1637_STATE.current_op_stage / 2))) {
                gpio_set_output_high(_GPIO_DAT);
            } else {
                gpio_set_output_low(_GPIO_DAT);
            }
            qdelay_start(&_TM1637_STATE.delay, 1);
            _TM1637_STATE.current_op_stage += 1;
        }
        return;
    } 

    switch (_TM1637_STATE.current_op_stage)
    {
    case 16:
        gpio_conf_input(_GPIO_DAT);
        gpio_set_output_low(_GPIO_CLK);
        qdelay_start(&_TM1637_STATE.delay, 1);
        _TM1637_STATE.current_op_stage += 1;
        break;
    case 17:
        if (!gpio_get_input(_GPIO_DAT)) {
            _TM1637_STATE.current_op_stage += 1;
        } else {
            qdelay_start(&_TM1637_STATE.delay, 1);
        }
        break;
    case 18:
        gpio_set_output_high(_GPIO_CLK);
        qdelay_start(&_TM1637_STATE.delay, 1);
        _TM1637_STATE.current_op_stage += 1;
        break;
    case 19:
        gpio_set_output_low(_GPIO_CLK);
        gpio_conf_output(_GPIO_DAT);
        gpio_set_output_high(_GPIO_DAT);
        _TM1637_STATE.current_op = 0;
        break;
    default:
        die();
        break;
    }
}


#if TM1637_SINGLETON
void tm1637_state_machine(void)
#else
void tm1637_state_machine(tm1637_t * tm1637)
#endif
{
    uint8_t cmd;
    if (_TM1637_STATE.current_op && !qdelay_check_done(&_TM1637_STATE.delay)){
        return;
    }
    if (!_TM1637_STATE.current_op){
        if (!bytebuf_cPopulation(&_TM1637_STATE.op_queue)){
            return;
        }
        cmd = bytebuf_cPopByte(&_TM1637_STATE.op_queue);
        switch (cmd)
        {
            case TM1637_LLOP_START:
            case TM1637_LLOP_STOP:
                _TM1637_STATE.current_op = cmd;
                break;
            case TM1637_LLOP_WBYTE:
                if (!bytebuf_cPopulation(&_TM1637_STATE.op_queue)){
                    die();
                }
                cmd = bytebuf_cPopByte(&_TM1637_STATE.op_queue);
            default:
                _TM1637_STATE.current_op = TM1637_LLOP_WBYTE;
                _TM1637_STATE.current_op_byte = cmd;
                break;
        }
        _TM1637_STATE.current_op_stage = 0;
    } 
    if (_TM1637_STATE.current_op) {
        #if TM1637_SINGLETON
        switch (_TM1637_STATE.current_op)
        {
            case TM1637_LLOP_START:
                _tm1637_sm_executor_start();
                break;
            case TM1637_LLOP_STOP:
                _tm1637_sm_executor_stop();
                break;
            case TM1637_LLOP_WBYTE:
                _tm1637_sm_executor_wbyte();
                break;
            default:
                die();
                break;
        }
        #else 
        switch (_TM1637_STATE.current_op)
        {
            case TM1637_LLOP_START:
                _tm1637_sm_executor_start(_TM1637p);
                
                break;
            case TM1637_LLOP_STOP:
                _tm1637_sm_executor_stop(_TM1637p);
                break;
            case TM1637_LLOP_WBYTE:
                _tm1637_sm_executor_wbyte(_TM1637p);
                break;
            default:
                die();
                break;
        }
        #endif
    }

    return;
}

#endif
#endif
