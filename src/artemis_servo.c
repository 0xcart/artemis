///
/// @file artemis_servo.c
///

#include "artemis_pca9685.h"
#include "artemis_servo.h"
#include "artemis_task.h"
#include <stddef.h>

typedef struct s_module_t
{
    artemis_servo_t servos[ARTEMIS_SERVO_INDEX_COUNT];
} module_t;

static module_t module;

///
///
///
void artemis_servo_initialize(void)
{
    artemis_servo_t *servo;

    artemis_pca9685_initialize(ARTEMIS_SERVO_PWM_FREQUENCY);

    for (size_t i = 0; i < ARTEMIS_SERVO_INDEX_COUNT; i++) {
        servo = &module.servos[i];
        servo->value = ARTEMIS_SERVO_PULSE_CENTER;
        servo->minimum = ARTEMIS_SERVO_PULSE_MINIMUM;
        servo->maximum = ARTEMIS_SERVO_PULSE_MAXIMUM;
        servo->center = ARTEMIS_SERVO_PULSE_CENTER;
    }
}

///
///
///
void artemis_servo_update(const char *name, uint64_t elapsed_us)
{
    artemis_servo_t *servo;

    ARTEMIS_TASK_TIME(name, elapsed_us);

    for (size_t i = 0; i < ARTEMIS_SERVO_INDEX_COUNT; i++) {
        servo = &module.servos[i];
        artemis_pca9685_setpwm(i, servo->value);
    }
}

///
///
///
artemis_servo_t *artemis_servo_get(artemis_servo_index_t index)
{
    return(&module.servos[index]);
}
