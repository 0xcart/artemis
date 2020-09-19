///
/// @file artemis_imu.c
///

#include "artemis_debug.h"
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
void artemis_imu_update(const char *name, uint64_t elapsed_us)
{
    ARTEMIS_DEBUG_TASKINFO(name, elapsed_us);
}
