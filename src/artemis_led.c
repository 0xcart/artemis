///
/// @file artemis_led.c
///

#include "artemis_debug.h"
#include "artemis_led.h"
#include "artemis_task.h"
#include <am_bsp.h>

typedef struct s_module_t
{
    bool state;
} module_t;

static module_t module;

///
///
///
void artemis_led_initialize(void)
{
    ARTEMIS_DEBUG_HALSTATUS(am_hal_gpio_pinconfig(AM_BSP_GPIO_LED_BLUE, g_AM_HAL_GPIO_OUTPUT));

    am_devices_led_off(am_bsp_psLEDs, AM_BSP_LED_BLUE);
}

///
///
///
void artemis_led_toggle(const char *name, uint64_t elapsed_us)
{
    ARTEMIS_TASK_TIME(name, elapsed_us);

    module.state = !module.state;

    if (module.state) {
        am_devices_led_on(am_bsp_psLEDs, AM_BSP_LED_BLUE);
    }
    else {
        am_devices_led_off(am_bsp_psLEDs, AM_BSP_LED_BLUE);
    }
}
