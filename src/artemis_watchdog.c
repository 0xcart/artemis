///
/// @file artemis_watchdog.c
///

#include "artemis_debug.h"
#include "artemis_watchdog.h"
#include <am_bsp.h>

#define ARTEMIS_WATCHDOG_LFRC_16HZ         (16) // 8-bit counter; 2^8 / 16Hz = 16 second max timeout
#define ARTEMIS_WATCHDOG_INTERRUPT_TIMEOUT (ARTEMIS_WATCHDOG_LFRC_16HZ * (3 / 2)) // 1.5 second interrupt timeout
#define ARTEMIS_WATCHDOG_RESET_TIMEOUT     (ARTEMIS_WATCHDOG_LFRC_16HZ * (2)) // 2.0 second reset timeout

typedef struct s_module_t
{
    am_hal_wdt_config_t watchdog;
    volatile uint8_t interrupt;
} module_t;

static module_t module;

///
///
///
void artemis_watchdog_initialize(void)
{
    am_hal_clkgen_control(AM_HAL_CLKGEN_CONTROL_LFRC_START, 0);
    am_hal_reset_control(AM_HAL_RESET_CONTROL_STATUSCLEAR, 0);

    NVIC_EnableIRQ(WDT_IRQn);

    module.watchdog.ui32Config = _VAL2FLD(WDT_CFG_CLKSEL, WDT_CFG_CLKSEL_16HZ) | AM_HAL_WDT_ENABLE_INTERRUPT | AM_HAL_WDT_ENABLE_RESET;
    module.watchdog.ui16InterruptCount = ARTEMIS_WATCHDOG_INTERRUPT_TIMEOUT;
    module.watchdog.ui16ResetCount = ARTEMIS_WATCHDOG_RESET_TIMEOUT;
    am_hal_wdt_init(&module.watchdog);

    am_hal_wdt_start();
}

///
///
///
void artemis_watchdog_restart(const char *name, uint64_t elapsed_us)
{
    ARTEMIS_DEBUG_TASKINFO(name, elapsed_us);

    am_hal_wdt_restart();
}

///
/// Define watchdog interrupt; see artemis_startup.c
///
void am_watchdog_isr(void)
{
    am_hal_wdt_int_clear();
    module.interrupt++;
}
