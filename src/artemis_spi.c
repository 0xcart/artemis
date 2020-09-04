///
/// @file artemis_spi.c
///

#include "artemis_spi.h"
#include "artemis_iom.h"
#include <am_bsp.h>

typedef struct s_spi_t
{
    artemis_iom_instance_t instance;
    am_hal_iom_transfer_t transfer;
} spi_t;

typedef struct s_module_t
{
    spi_t spi[ARTEMIS_IOM_SPI_COUNT];
} module_t;

static module_t module;

///
///
///
void artemis_spi_initialize(void)
{
    artemis_iom_instance_t *instance;

    instance = &module.spi[ARTEMIS_IOM_SPI0].instance;
    instance->module = ARTEMIS_IOM_MODULE_SPI0;
    instance->config.eInterfaceMode = AM_HAL_IOM_SPI_MODE;
    instance->config.ui32ClockFreq = AM_HAL_IOM_4MHZ;
    instance->config.eSpiMode = AM_HAL_IOM_SPI_MODE_3;
    artemis_iom_initialize(instance);
    am_hal_gpio_pinconfig(AM_BSP_GPIO_IOM0_SCK, g_AM_BSP_GPIO_IOM0_SCK);
    am_hal_gpio_pinconfig(AM_BSP_GPIO_IOM0_MISO, g_AM_BSP_GPIO_IOM0_MISO);
    am_hal_gpio_pinconfig(AM_BSP_GPIO_IOM0_MOSI, g_AM_BSP_GPIO_IOM0_MOSI);
    am_hal_gpio_pinconfig(AM_BSP_GPIO_IOM0_CS, g_AM_BSP_GPIO_IOM0_CS);
}
