///
/// @file artemis_pca9685.h
///

#ifndef ARTEMIS_PCA9685_H
#define ARTEMIS_PCA9685_H

#include <stdbool.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

void artemis_pca9685_initialize(void);
void artemis_pca9685_pwm(uint8_t pin, uint16_t value, bool invert);

#ifdef __cplusplus
}
#endif

#endif
