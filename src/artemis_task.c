///
/// @file artemis_task.c
///

#include "artemis_core.h"
#include "artemis_imu.h"
#include "artemis_led.h"
#include "artemis_rx.h"
#include "artemis_servo.h"
#include "artemis_task.h"
#include "artemis_watchdog.h"

#define ARTEMIS_TASK_ENTRY(_name, _initialize, _run, _period_hz) { \
    .name = _name, \
    .initialize = _initialize, \
    .run = _run, \
    .period_hz = _period_hz, \
    .previous_us = 0 \
}

typedef struct s_module_t
{
    artemis_task_t tasks[ARTEMIS_TASK_ID_COUNT];
} module_t;

static module_t module = {
    {
        [ARTEMIS_TASK_ID_IMU] = ARTEMIS_TASK_ENTRY("IMU", artemis_imu_initialize, artemis_imu_update, 200),
        [ARTEMIS_TASK_ID_RX] = ARTEMIS_TASK_ENTRY("RX", artemis_rx_initialize, artemis_rx_update, 100),
        [ARTEMIS_TASK_ID_CORE] = ARTEMIS_TASK_ENTRY("CORE", artemis_core_initialize, artemis_core_update, 100),
        [ARTEMIS_TASK_ID_SERVO] = ARTEMIS_TASK_ENTRY("SERVO", artemis_servo_initialize, artemis_servo_update, 50),
        [ARTEMIS_TASK_ID_LED] = ARTEMIS_TASK_ENTRY("LED", artemis_led_initialize, artemis_led_toggle, 2),
        // [ARTEMIS_TASK_ID_WATCHDOG] = ARTEMIS_TASK_ENTRY("WATCHDOG", artemis_watchdog_initialize, artemis_watchdog_restart, 1)
    }
};

///
///
///
artemis_task_t *artemis_task_get(artemis_task_id_t id)
{
    return(&module.tasks[id]);
}
