///
/// @file artemis_scheduler.c
///

#include "artemis_led.h"
#include "artemis_scheduler.h"
#include "artemis_time.h"
#include <stddef.h>
#include <stdint.h>

#define ARTEMIS_SCHEDULER_DEFINE_TASK(_name, _initialize, _run, _period_us) { \
    .name = _name, \
    .initialize = _initialize, \
    .run = _run, \
    .period_us = _period_us, \
    .previous_us = 0 \
}

typedef enum e_taskid_t
{
    ARTEMIS_SCHEDULER_TASKID_LED,
    ARTEMIS_SCHEDULER_TASKID_COUNT
} taskid_t;

typedef struct s_task_t
{
    char *name;
    void (*initialize)(void);
    void (*run)(uint64_t elapsed_us);
    uint64_t period_us;
    uint64_t previous_us;
} task_t;

typedef struct s_module_t
{
    task_t tasks[ARTEMIS_SCHEDULER_TASKID_COUNT];
} module_t;

static module_t module = {
    {
        [ARTEMIS_SCHEDULER_TASKID_LED] = ARTEMIS_SCHEDULER_DEFINE_TASK("LED", artemis_led_initialize, artemis_led_toggle, ARTEMIS_TIME_HZ_TO_US(2))
    }
};

///
///
///
void artemis_scheduler_initialize(void)
{
    task_t *task;
    uint64_t current_us = artemis_time_getus();

    for (size_t i = 0; i < ARTEMIS_SCHEDULER_TASKID_COUNT; i++) {
        task = &module.tasks[i];
        task->previous_us = current_us;
        task->initialize();
    }
}

///
///
///
void artemis_scheduler_run(void)
{
    task_t *task;
    uint64_t current_us;
    uint64_t elapsed_us;

    while (1)
    {
        for (size_t i = 0; i < ARTEMIS_SCHEDULER_TASKID_COUNT; i++) {
            task = &module.tasks[i];
            current_us = artemis_time_getus();
            elapsed_us = current_us - task->previous_us;

            if (elapsed_us >= task->period_us) {
                task->previous_us = current_us;
                task->run(elapsed_us);
            }
        }
    }
}
