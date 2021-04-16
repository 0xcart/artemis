///
/// @file artemis_main.c
///

#include "artemis_debug.h"
#include "artemis_mcu.h"
#include "artemis_scheduler.h"
#include "artemis_time.h"
#include <stdlib.h>

///
///
///
int main(void)
{
    // initialize mcu features
    artemis_mcu_initialize();

    // initialize debug features
    artemis_debug_initialize();

    // initialize time functions
    artemis_time_initialize();

    // initialize the scheduler
    artemis_scheduler_initialize();

    // run the application
    artemis_scheduler_run();

    return(EXIT_SUCCESS);
}
