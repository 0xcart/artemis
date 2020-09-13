///
/// @file artemis_core.c
///

#include "artemis_core.h"
#include "artemis_debug.h"
#include "artemis_servo.h"
#include <stdbool.h>
#include <stddef.h>

typedef struct s_module_t
{
    bool state;
    uint16_t value;
} module_t;

static module_t module = {.state = true, .value = ARTEMIS_SERVO_PULSE_MINIMUM};

///
///
///
void artemis_core_initialize(void)
{
    artemis_servo_initialize();
}

///
///
///
void artemis_core_update(const char *name, uint64_t elapsed_us)
{
    artemis_servo_t *servo;

    ARTEMIS_DEBUG_TASKINFO(name, elapsed_us);

    servo = artemis_servo_get(0);
    servo->value = module.value;
    artemis_servo_update();

    if (module.state) {
        if (++module.value >= ARTEMIS_SERVO_PULSE_MAXIMUM) {
            module.state = false;
        }
    }
    else {
        if (--module.value <= ARTEMIS_SERVO_PULSE_MINIMUM) {
            module.state = true;
        }
    }
}
