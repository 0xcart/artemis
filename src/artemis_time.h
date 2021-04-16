///
/// @file artemis_time.h
///

#ifndef ARTEMIS_TIME_H
#define ARTEMIS_TIME_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#define ARTEMIS_TIME_HZ_TO_US(hz) \
(__extension__({ \
    __typeof__(hz) _hz = (hz); \
    (_hz > 0) ? (1000000 / _hz) : 0; \
}))

#define ARTEMIS_TIME_HZ_TO_MS(hz) \
(__extension__({ \
    __typeof__(hz) _hz = (hz); \
    (_hz > 0) ? (1000 / _hz) : 0; \
}))

void artemis_time_initialize(void);
uint64_t artemis_time_get(void);
uint64_t artemis_time_getus(void);
uint64_t artemis_time_getms(void);
uint64_t artemis_time_getss(void);
void artemis_time_delayus(uint32_t us);
void artemis_time_delayms(uint32_t ms);
void artemis_time_delayss(uint32_t ss);

#ifdef __cplusplus
}
#endif

#endif
