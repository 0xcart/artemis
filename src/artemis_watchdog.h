///
/// @file artemis_watchdog.h
///

#ifndef ARTEMIS_WATCHDOG_H
#define ARTEMIS_WATCHDOG_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

void artemis_watchdog_initialize(void);
void artemis_watchdog_restart(const char *name, uint64_t elapsed_us);

#ifdef __cplusplus
}
#endif

#endif
