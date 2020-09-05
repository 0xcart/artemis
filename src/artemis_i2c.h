///
/// @file artemis_i2c.h
///

#ifndef ARTEMIS_I2C_H
#define ARTEMIS_I2C_H

#include "artemis_iom.h"
#include "artemis_stream.h"
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct s_artemis_i2c_t
{
    uint8_t address;
    bool stop;
    artemis_stream_t *txstream;
    artemis_stream_t *rxstream;
} artemis_i2c_t;

bool artemis_i2c_send(artemis_i2c_t *i2c, artemis_iom_t *iom);
bool artemis_i2c_request(artemis_i2c_t *i2c, artemis_iom_t *iom);

#ifdef __cplusplus
}
#endif

#endif
