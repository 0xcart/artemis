///
/// @file artemis_led.h
///

#ifndef ARTEMIS_LED_H
#define ARTEMIS_LED_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

void artemis_led_initialize(void);
void artemis_led_toggle(uint64_t elapsed_us);

#ifdef __cplusplus
}
#endif

#endif
