///
/// @file artemis_imu.h
///

#ifndef ARTEMIS_IMU_H
#define ARTEMIS_IMU_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

void artemis_imu_initialize(void);
void artemis_imu_update(const char *name, uint64_t elapsed_us);

#ifdef __cplusplus
}
#endif

#endif
