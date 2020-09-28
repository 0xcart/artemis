///
/// @file artemis_mcu.c
///

#include "artemis_debug.h"
#include "artemis_mcu.h"
#include <am_bsp.h>

///
///
///
void artemis_mcu_initialize(void)
{
    am_hal_burst_avail_e burst_avail;

    // set the clock frequency
    ARTEMIS_DEBUG_HALSTATUS(am_hal_clkgen_control(AM_HAL_CLKGEN_CONTROL_SYSCLK_MAX, 0));

    // set the default cache configuration
    ARTEMIS_DEBUG_HALSTATUS(am_hal_cachectrl_config(&am_hal_cachectrl_defaults));
    ARTEMIS_DEBUG_HALSTATUS(am_hal_cachectrl_enable());

    // enable the floating point module
    am_hal_sysctrl_fpu_enable();

    // initialize mcu for burst mode operations
    ARTEMIS_DEBUG_HALSTATUS(am_hal_burst_mode_initialize(&burst_avail));

    // configure the board for low power operation
    // am_bsp_low_power_init();

    // enable interrupts
    ARTEMIS_DEBUG_HALSTATUS(am_hal_interrupt_master_enable());
}

///
///
///
void artemis_mcu_enableburst(void)
{
    am_hal_burst_mode_e burst_mode;

    ARTEMIS_DEBUG_HALSTATUS(am_hal_burst_mode_enable(&burst_mode));
}

///
///
///
void artemis_mcu_disableburst(void)
{
    am_hal_burst_mode_e burst_mode;

    ARTEMIS_DEBUG_HALSTATUS(am_hal_burst_mode_disable(&burst_mode));
}
