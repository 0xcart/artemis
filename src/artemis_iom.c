///
/// @file artemis_iom.c
///

#include "artemis_iom.h"

///
///
///
void artemis_iom_initialize(artemis_iom_t *iom)
{
    am_hal_iom_initialize(iom->module, &iom->handle);
    am_hal_iom_power_ctrl(iom->handle, AM_HAL_SYSCTRL_WAKE, false);
    am_hal_iom_configure(iom->handle, &iom->config);
    am_hal_iom_enable(iom->handle);
}

///
///
///
void artemis_iom_uninitialize(artemis_iom_t *iom)
{
    am_hal_iom_disable(iom->handle);
    am_hal_iom_power_ctrl(iom->handle, AM_HAL_SYSCTRL_DEEPSLEEP, false);
    am_hal_iom_uninitialize(iom->handle);
    iom->handle = 0;
}
