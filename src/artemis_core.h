///
/// @file artemis_core.h
///

#ifndef ARTEMIS_CORE_H
#define ARTEMIS_CORE_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

void artemis_core_initialize(void);
void artemis_core_update(uint64_t elapsed_us);

#ifdef __cplusplus
}
#endif

#endif
