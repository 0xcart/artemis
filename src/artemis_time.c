///
/// @file artemis_time.c
///

#include "artemis_time.h"
#include <am_bsp.h>
#include <am_util_delay.h>

#define ARTEMIS_TIME_STIMER_HFRC_HZ  (3000000)
#define ARTEMIS_TIME_STIMER_HFRC_KHZ (ARTEMIS_TIME_STIMER_HFRC_HZ / 1000)
#define ARTEMIS_TIME_STIMER_HFRC_MHZ (ARTEMIS_TIME_STIMER_HFRC_HZ / 1000000)

typedef struct s_module_t
{
    uint64_t time;
    volatile uint32_t overflow;
} module_t;

static module_t module;

static void module_time_update(void);

///
///
///
void artemis_time_initialize(void)
{
    am_hal_stimer_int_enable(AM_HAL_STIMER_INT_OVERFLOW);

    NVIC_EnableIRQ(STIMER_IRQn);

    am_hal_stimer_config(AM_HAL_STIMER_CFG_CLEAR | AM_HAL_STIMER_CFG_FREEZE);
    am_hal_stimer_config(AM_HAL_STIMER_HFRC_3MHZ);
}

///
///
///
uint64_t artemis_time_get(void)
{
    module_time_update();
    return(module.time);
}

///
///
///
uint64_t artemis_time_getus(void)
{
    module_time_update();
    return(module.time / ARTEMIS_TIME_STIMER_HFRC_MHZ);
}

///
///
///
uint64_t artemis_time_getms(void)
{
    module_time_update();
    return(module.time / ARTEMIS_TIME_STIMER_HFRC_KHZ);
}

///
///
///
uint64_t artemis_time_getss(void)
{
    module_time_update();
    return(module.time / ARTEMIS_TIME_STIMER_HFRC_HZ);
}

///
///
///
void artemis_time_delayus(uint32_t us)
{
    am_util_delay_us(us);
}

///
///
///
void artemis_time_delayms(uint32_t ms)
{
    am_util_delay_ms(ms);
}

///
///
///
void artemis_time_delayss(uint32_t ss)
{
    am_util_delay_ms(ss * 1000);
}

///
///
///
static void module_time_update(void)
{
    module.time = module.overflow;
    module.time <<= 32;
    module.time |= (am_hal_stimer_counter_get() & 0xFFFFFFFF);
}

///
/// Define stimer interrupt; see artemis_startup.c
///
void am_stimer_isr(void)
{
    am_hal_stimer_int_clear(AM_HAL_STIMER_INT_OVERFLOW);
    module.overflow++;
}
