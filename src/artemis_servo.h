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

#ifdef __cplusplus
}
#endif

#endif
