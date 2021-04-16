///
/// @file artemis_scheduler.c
///

#include "artemis_scheduler.h"
#include "artemis_task.h"
#include "artemis_time.h"
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <am_util_stdio.h>

#ifdef NTASKTIME
    #define ARTEMIS_SCHEDULER_TASKTIME(name, elapsed_us) ((void)0)
#else
    #define ARTEMIS_SCHEDULER_TASKTIME(name, elapsed_us) (am_util_stdio_printf("%s:\t\t%llu\n", name, elapsed_us))
#endif

///
///
///
void artemis_scheduler_initialize(void)
{
    artemis_task_t *task;

    for (size_t i = 0; i < ARTEMIS_TASK_ID_COUNT; i++) {
        task = artemis_task_get(i);
        task->previous_us = artemis_time_getus();
        task->initialize();
    }
}

///
///
///
void artemis_scheduler_run(void)
{
    artemis_task_t *task;
    uint64_t current_us;
    uint64_t elapsed_us;

    while (1)
    {
        for (size_t i = 0; i < ARTEMIS_TASK_ID_COUNT; i++) {
            task = artemis_task_get(i);
            current_us = artemis_time_getus();
            elapsed_us = current_us - task->previous_us;

            if (elapsed_us >= ARTEMIS_TIME_HZ_TO_US(task->period_hz)) {
                ARTEMIS_SCHEDULER_TASKTIME(task->name, elapsed_us);
                task->previous_us = current_us;
                task->run(elapsed_us);
            }
        }
    }
}
