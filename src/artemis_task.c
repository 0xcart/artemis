///
/// @file artemis_task.c
///

#include "artemis_led.h"
#include "artemis_task.h"
#include "artemis_time.h"

#define ARTEMIS_TASK_DEFINE_TASK(_name, _initialize, _run, _period_us) { \
    .name = _name, \
    .initialize = _initialize, \
    .run = _run, \
    .period_us = _period_us, \
    .previous_us = 0 \
}

typedef struct s_module_t
{
    artemis_task_t tasks[ARTEMIS_TASK_ID_COUNT];
} module_t;

static module_t module = {
    {
        [ARTEMIS_TASK_ID_LED] = ARTEMIS_TASK_DEFINE_TASK("LED", artemis_led_initialize, artemis_led_toggle, ARTEMIS_TIME_HZ_TO_US(2))
    }
};

///
///
///
artemis_task_t *artemis_task_get(artemis_task_id_t id)
{
    return(&module.tasks[id]);
}
