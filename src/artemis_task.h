///
/// @file artemis_task.h
///

#ifndef ARTEMIS_TASK_H
#define ARTEMIS_TASK_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef enum e_artemis_task_id_t
{
    ARTEMIS_TASK_ID_IMU,
    ARTEMIS_TASK_ID_CORE,
    ARTEMIS_TASK_ID_SERVO,
    ARTEMIS_TASK_ID_LED,
    // ARTEMIS_TASK_ID_WATCHDOG,
    ARTEMIS_TASK_ID_COUNT
} artemis_task_id_t;

typedef struct s_artemis_task_t
{
    const char *name;
    void (*initialize)(void);
    void (*run)(uint64_t elapsed_us);
    uint16_t period_hz;
    uint64_t previous_us;
} artemis_task_t;

artemis_task_t *artemis_task_get(artemis_task_id_t id);

#ifdef __cplusplus
}
#endif

#endif
