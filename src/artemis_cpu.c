///
/// @file artemis_cpu.c
///

#include "artemis_cpu.h"
#include <am_bsp.h>

///
///
///
void artemis_cpu_initialize(void)
{
    am_hal_burst_avail_e burst_avail;

    am_hal_burst_mode_initialize(&burst_avail);
}

///
///
///
void artemis_cpu_enableburst(void)
{
    am_hal_burst_mode_e burst_mode;

    am_hal_burst_mode_enable(&burst_mode);
}

///
///
///
void artemis_cpu_disableburst(void)
{
    am_hal_burst_mode_e burst_mode;

    am_hal_burst_mode_disable(&burst_mode);
}
