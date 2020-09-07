///
/// @file artemis_scheduler.c
///

#include "artemis_scheduler.h"
#include "artemis_task.h"
#include "artemis_time.h"
#include <stddef.h>
#include <stdint.h>

///
///
///
void artemis_scheduler_initialize(void)
{
    artemis_task_t *task;
    uint64_t current_us = artemis_time_getus();

    for (size_t i = 0; i < ARTEMIS_TASK_ID_COUNT; i++) {
        task = artemis_task_get(i);
        task->previous_us = current_us;
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

            if (elapsed_us >= task->period_us) {
                task->previous_us = current_us;
                task->run(elapsed_us);
            }
        }
    }
}
