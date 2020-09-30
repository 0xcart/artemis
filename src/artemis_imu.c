///
/// @file artemis_imu.c
///

#include "artemis_icm20649.h"
#include "artemis_imu.h"
#include "artemis_task.h"

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
void artemis_imu_update(const char *name, uint64_t elapsed_us)
{
    artemis_icm20649_data_t accel;
    artemis_icm20649_data_t gyro;

    ARTEMIS_TASK_TIME(name, elapsed_us);

    artemis_icm20649_readaccel(&accel);
    artemis_icm20649_readgyro(&gyro);
}
