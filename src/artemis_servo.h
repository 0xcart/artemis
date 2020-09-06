///
/// @file artemis_servo.h
///

#ifndef ARTEMIS_SERVO_H
#define ARTEMIS_SERVO_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

void artemis_servo_initialize(void);
void artemis_servo_update(uint64_t elapsed_us);
void artemis_servo_pwm(uint8_t pin, uint16_t value);

#ifdef __cplusplus
}
#endif

#endif
