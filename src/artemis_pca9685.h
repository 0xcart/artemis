///
/// @file artemis_pca9685.h
///

#ifndef ARTEMIS_PCA9685_H
#define ARTEMIS_PCA9685_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

void artemis_pca9685_initialize(uint16_t frequency);
void artemis_pca9685_setpwm(uint8_t pin, uint16_t off);

#ifdef __cplusplus
}
#endif

#endif
