///
/// @file artemis_i2c.c
///

#include "artemis_i2c.h"
#include "artemis_iom.h"
#include <am_bsp.h>

typedef struct s_i2c_t
{
    artemis_iom_instance_t instance;
    am_hal_iom_transfer_t transfer;
} i2c_t;

typedef struct s_module_t
{
    i2c_t i2c[ARTEMIS_IOM_I2C_COUNT];
} module_t;

static module_t module;

///
///
///
void artemis_i2c_initialize(void)
{
    artemis_iom_instance_t *instance;

    instance = &module.i2c[ARTEMIS_IOM_I2C0].instance;
    instance->module = ARTEMIS_IOM_MODULE_I2C0;
    instance->config.eInterfaceMode = AM_HAL_IOM_I2C_MODE;
    instance->config.ui32ClockFreq = AM_HAL_IOM_400KHZ;
    artemis_iom_initialize(instance);
    am_hal_gpio_pinconfig(AM_BSP_GPIO_IOM4_SCL, g_AM_BSP_GPIO_IOM4_SCL);
    am_hal_gpio_pinconfig(AM_BSP_GPIO_IOM4_SDA, g_AM_BSP_GPIO_IOM4_SDA);

    instance = &module.i2c[ARTEMIS_IOM_I2C1].instance;
    instance->module = ARTEMIS_IOM_MODULE_I2C1;
    instance->config.eInterfaceMode = AM_HAL_IOM_I2C_MODE;
    instance->config.ui32ClockFreq = AM_HAL_IOM_400KHZ;
    artemis_iom_initialize(instance);
    am_hal_gpio_pinconfig(AM_BSP_GPIO_IOM3_SCL, g_AM_BSP_GPIO_IOM3_SCL);
    am_hal_gpio_pinconfig(AM_BSP_GPIO_IOM3_SDA, g_AM_BSP_GPIO_IOM3_SDA);
}
