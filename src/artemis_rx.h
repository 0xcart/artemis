///
/// @file artemis_rx.h
///

#ifndef ARTEMIS_RX_H
#define ARTEMIS_RX_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

void artemis_rx_initialize(void);
void artemis_rx_update(uint64_t elapsed_us);

#ifdef __cplusplus
}
#endif

#endif
