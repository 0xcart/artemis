///
/// @file artemis_iom.c
///

#include "artemis_debug.h"
#include "artemis_iom.h"

///
///
///
void artemis_iom_initialize(artemis_iom_t *iom)
{
    ARTEMIS_DEBUG_HALSTATUS(am_hal_iom_initialize(iom->module, &iom->handle));
    ARTEMIS_DEBUG_HALSTATUS(am_hal_iom_power_ctrl(iom->handle, AM_HAL_SYSCTRL_WAKE, false));
    ARTEMIS_DEBUG_HALSTATUS(am_hal_iom_configure(iom->handle, &iom->config));
    ARTEMIS_DEBUG_HALSTATUS(am_hal_iom_enable(iom->handle));
}

///
///
///
void artemis_iom_uninitialize(artemis_iom_t *iom)
{
    ARTEMIS_DEBUG_HALSTATUS(am_hal_iom_disable(iom->handle));
    ARTEMIS_DEBUG_HALSTATUS(am_hal_iom_power_ctrl(iom->handle, AM_HAL_SYSCTRL_DEEPSLEEP, false));
    ARTEMIS_DEBUG_HALSTATUS(am_hal_iom_uninitialize(iom->handle));
    iom->handle = 0;
}
