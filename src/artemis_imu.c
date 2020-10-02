///
/// @file artemis_imu.c
///

#include "artemis_icm20649.h"
#include "artemis_imu.h"

///
///
///
void artemis_imu_initialize(void)
{
    artemis_icm20649_initialize();
}

///
///
///
void artemis_imu_update(uint64_t elapsed_us)
{
    artemis_icm20649_data_t accel;
    artemis_icm20649_data_t gyro;

    artemis_icm20649_readaccel(&accel);
    artemis_icm20649_readgyro(&gyro);
}
