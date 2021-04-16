///
/// @file artemis_i2c.h
///

#ifndef ARTEMIS_I2C_H
#define ARTEMIS_I2C_H

#include "artemis_iom.h"
#include "artemis_stream.h"
#include <stdbool.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct s_artemis_i2c_t
{
    uint8_t address;
    artemis_iom_t iom;
} artemis_i2c_t;

void artemis_i2c_send(artemis_i2c_t *i2c, bool stop, artemis_stream_t *txstream);
void artemis_i2c_receive(artemis_i2c_t *i2c, bool stop, artemis_stream_t *rxstream, uint32_t rxnumber);

#ifdef __cplusplus
}
#endif

#endif
