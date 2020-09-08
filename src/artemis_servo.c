///
/// @file artemis_servo.c
///

#include "artemis_pca9685.h"
#include "artemis_servo.h"
#include <stddef.h>

#define ARTEMIS_SERVO_PWM_FREQUENCY (50)
#define ARTEMIS_SERVO_PULSE_MINIMUM (204)
#define ARTEMIS_SERVO_PULSE_MAXIMUM (408)
#define ARTEMIS_SERVO_PULSE_CENTER  (306)

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
    artemis_pca9685_initialize(ARTEMIS_SERVO_PWM_FREQUENCY);

    for (size_t i = 0; i < ARTEMIS_SERVO_INDEX_COUNT; i++) {
        module.servos[i].value = ARTEMIS_SERVO_PULSE_CENTER;
        module.servos[i].minimum = ARTEMIS_SERVO_PULSE_MINIMUM;
        module.servos[i].maximum = ARTEMIS_SERVO_PULSE_MAXIMUM;
        module.servos[i].center = ARTEMIS_SERVO_PULSE_CENTER;
        module.servos[i].invert = false;
    }
}
