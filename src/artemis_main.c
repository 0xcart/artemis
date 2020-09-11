///
/// @file artemis_main.c
///

#include "artemis_cpu.h"
#include "artemis_scheduler.h"
#include "artemis_time.h"
#include <am_bsp.h>
#include <am_util_stdio.h>

///
///
///
int main(void)
{
    // Set the clock frequency
    am_hal_clkgen_control(AM_HAL_CLKGEN_CONTROL_SYSCLK_MAX, 0);

    // Set the default cache configuration
    am_hal_cachectrl_config(&am_hal_cachectrl_defaults);
    am_hal_cachectrl_enable();

    // Enable the floating point module
    am_hal_sysctrl_fpu_enable();

    // Configure the board for low power operation
    // am_bsp_low_power_init();

    // Enable Single Wire Output (SWO) using Instrumentation Trace Macrocell (ITM)
    am_bsp_itm_printf_enable();
    am_util_stdio_printf("Test artemis SWO\n");

    // Enable interrupts
    am_hal_interrupt_master_enable();

    // Initialize cpu features
    artemis_cpu_initialize();

    // Initialize time functions
    artemis_time_initialize();

    // Initialize the scheduler
    artemis_scheduler_initialize();

    // Run the application
    artemis_scheduler_run();

    return(0);
}
