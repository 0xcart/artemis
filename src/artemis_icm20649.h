///
/// @file artemis_icm20649.h
///

#ifndef ARTEMIS_ICM20649_H
#define ARTEMIS_ICM20649_H

#include "artemis_imu.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef int16_t artemis_icm20649_data_t[ARTEMIS_IMU_AXIS_COUNT];

void artemis_icm20649_initialize(void);
void artemis_icm20649_read(artemis_icm20649_data_t *accel, artemis_icm20649_data_t *gyro);

#ifdef __cplusplus
}
#endif

#endif
