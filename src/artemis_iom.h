///
/// @file artemis_iom.h
///

#ifndef ARTEMIS_IOM_H
#define ARTEMIS_IOM_H

#include <am_bsp.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef enum e_artemis_iom_i2c_t
{
    ARTEMIS_IOM_I2C0, // QWIIC
    ARTEMIS_IOM_I2C1, // Pins: 17/SCL, 16/SDA
    ARTEMIS_IOM_I2C_COUNT
} artemis_iom_i2c_t;

typedef enum e_artemis_iom_spi_t
{
    ARTEMIS_IOM_SPI0, // Pins: 13/SCK, 12/MISO, 11/MOSI, A2/CS
    ARTEMIS_IOM_SPI_COUNT
} artemis_iom_spi_t;

typedef enum e_artemis_iom_module_t
{
    ARTEMIS_IOM_MODULE_I2C0 = 4,
    ARTEMIS_IOM_MODULE_I2C1 = 3,
    ARTEMIS_IOM_MODULE_SPI0 = 0
} artemis_iom_module_t;

typedef struct s_artemis_iom_instance_t
{
    void *handle;
    artemis_iom_module_t module;
    am_hal_iom_config_t config;
} artemis_iom_instance_t;

void artemis_iom_initialize(artemis_iom_instance_t *instance);
void artemis_iom_uninitialize(artemis_iom_instance_t *instance);

#ifdef __cplusplus
}
#endif

#endif
