///
/// @file artemis_task.c
///

#include "artemis_led.h"
#include "artemis_task.h"
#include "artemis_time.h"
#include "artemis_watchdog.h"

#define ARTEMIS_TASK_DEFINE_TASK(_name, _initialize, _run, _period_hz) { \
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
        [ARTEMIS_TASK_ID_LED] = ARTEMIS_TASK_DEFINE_TASK("LED", artemis_led_initialize, artemis_led_toggle, 2),
        // [ARTEMIS_TASK_ID_WATCHDOG] = ARTEMIS_TASK_DEFINE_TASK("WATCHDOG", artemis_watchdog_initialize, artemis_watchdog_restart, 1)
    }
};

///
///
///
artemis_task_t *artemis_task_get(artemis_task_id_t id)
{
    return(&module.tasks[id]);
}
