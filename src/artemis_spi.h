///
/// @file artemis_spi.h
///

#ifndef ARTEMIS_SPI_H
#define ARTEMIS_SPI_H

#include "artemis_iom.h"
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct s_artemis_spi_t
{
    artemis_iom_t iom;
} artemis_spi_t;

bool artemis_spi_send(artemis_spi_t *spi);
bool artemis_spi_receive(artemis_spi_t *spi);
bool artemis_spi_transfer(artemis_spi_t *spi);

#ifdef __cplusplus
}
#endif

#endif
