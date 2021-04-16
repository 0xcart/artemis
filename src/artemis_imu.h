///
/// @file artemis_imu.h
///

#ifndef ARTEMIS_IMU_H
#define ARTEMIS_IMU_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef enum e_artemis_imu_axis_t
{
    ARTEMIS_IMU_AXIS_X,
    ARTEMIS_IMU_AXIS_Y,
    ARTEMIS_IMU_AXIS_Z,
    ARTEMIS_IMU_AXIS_COUNT
} artemis_imu_axis_t;

void artemis_imu_initialize(void);
void artemis_imu_update(uint64_t elapsed_us);

#ifdef __cplusplus
}
#endif

#endif
