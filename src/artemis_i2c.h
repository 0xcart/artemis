///
/// @file artemis_i2c.h
///

#ifndef ARTEMIS_I2C_H
#define ARTEMIS_I2C_H

#include "artemis_iom.h"
#include <stdbool.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct s_artemis_i2c_t
{
    artemis_iom_t iom;
    uint8_t address;
    bool stop;
} artemis_i2c_t;

bool artemis_i2c_send(artemis_i2c_t *i2c);
bool artemis_i2c_receive(artemis_i2c_t *i2c, uint32_t rxnumber);

#ifdef __cplusplus
}
#endif

#endif
