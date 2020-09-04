///
/// @file artemis_iom.c
///

#include "artemis_iom.h"

///
///
///
void artemis_iom_initialize(artemis_iom_instance_t *instance)
{
    am_hal_iom_initialize(instance->module, &instance->handle);
    am_hal_iom_power_ctrl(instance->handle, AM_HAL_SYSCTRL_WAKE, false);
    am_hal_iom_configure(instance->handle, &instance->config);
    am_hal_iom_enable(instance->handle);
}

///
///
///
void artemis_iom_uninitialize(artemis_iom_instance_t *instance)
{
    am_hal_iom_disable(instance->handle);
    am_hal_iom_power_ctrl(instance->handle, AM_HAL_SYSCTRL_DEEPSLEEP, false);
    am_hal_iom_uninitialize(instance->handle);
    instance->handle = 0;
}
