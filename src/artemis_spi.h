///
/// @file artemis_spi.h
///

#ifndef ARTEMIS_SPI_H
#define ARTEMIS_SPI_H

#include "artemis_iom.h"
#include "artemis_stream.h"
#include <stdbool.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#define ARTEMIS_SPI_READBIT_MSB (0x80)
#define ARTEMIS_SPI_READBIT_LSB (0x01)

typedef struct s_artemis_spi_t
{
    uint8_t chipselect;
    artemis_iom_t iom;
} artemis_spi_t;

void artemis_spi_send(artemis_spi_t *spi, bool stop, artemis_stream_t *txstream);
void artemis_spi_receive(artemis_spi_t *spi, bool stop, artemis_stream_t *rxstream, uint32_t rxnumber);
void artemis_spi_transfer(artemis_spi_t *spi, bool stop, artemis_stream_t *txstream, artemis_stream_t *rxstream);

#ifdef __cplusplus
}
#endif

#endif
