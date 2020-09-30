///
/// @file artemis_core.c
///

#include "artemis_core.h"
#include "artemis_servo.h"
#include "artemis_task.h"
#include <stdbool.h>
#include <stddef.h>

typedef struct s_module_t
{
    bool increment;
    uint16_t value;
} module_t;

static module_t module = {.increment = true, .value = ARTEMIS_SERVO_PULSE_MINIMUM};

///
///
///
void artemis_core_initialize(void)
{
}

///
///
///
void artemis_core_update(const char *name, uint64_t elapsed_us)
{
    artemis_servo_t *servo;

    ARTEMIS_TASK_TIME(name, elapsed_us);

    for (size_t i = 0; i < ARTEMIS_SERVO_INDEX_COUNT; i++) {
        servo = artemis_servo_get(i);
        servo->value = module.value;
    }

    if (module.increment) {
        if (++module.value >= ARTEMIS_SERVO_PULSE_MAXIMUM) {
            module.increment = false;
        }
    }
    else {
        if (--module.value <= ARTEMIS_SERVO_PULSE_MINIMUM) {
            module.increment = true;
        }
    }
}
