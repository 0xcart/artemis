///
/// @file artemis_icm20649.c
///

#include "artemis_icm20649.h"
#include "artemis_spi.h"

// application specific
#define ARTEMIS_ICM20649_SPIBUFFER_LENGTH (8) // transmit and receive buffer length

typedef uint8_t buffer_t[ARTEMIS_ICM20649_SPIBUFFER_LENGTH];

typedef struct s_module_t
{
    artemis_spi_t spi;
    buffer_t txbuffer;
    buffer_t rxbuffer;
} module_t;

static module_t module;

///
///
///
void artemis_icm20649_initialize(void)
{
    artemis_spi_t *spi = &module.spi;

    spi->bitorder = ARTEMIS_UTIL_BITORDER_MSB;
    spi->chipselect = ARTEMIS_IOM_CHIPSELECT_SPI0;

    spi->iom.module = ARTEMIS_IOM_MODULE_SPI0;
    spi->iom.config.eInterfaceMode = AM_HAL_IOM_SPI_MODE;
    spi->iom.config.ui32ClockFreq = AM_HAL_IOM_1MHZ;
    spi->iom.config.eSpiMode = AM_HAL_IOM_SPI_MODE_0;
    artemis_iom_initialize(&spi->iom);
    am_hal_gpio_pinconfig(AM_BSP_GPIO_IOM0_SCK, g_AM_BSP_GPIO_IOM0_SCK);
    am_hal_gpio_pinconfig(AM_BSP_GPIO_IOM0_MISO, g_AM_BSP_GPIO_IOM0_MISO);
    am_hal_gpio_pinconfig(AM_BSP_GPIO_IOM0_MOSI, g_AM_BSP_GPIO_IOM0_MOSI);
    am_hal_gpio_pinconfig(AM_BSP_GPIO_IOM0_CS, g_AM_BSP_GPIO_IOM0_CS);
}
