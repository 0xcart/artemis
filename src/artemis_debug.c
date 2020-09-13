///
/// @file artemis_debug.c
///

#include "artemis_debug.h"
#include <am_bsp.h>

///
///
///
void artemis_debug_initialize(void)
{
    // enable Single Wire Output (SWO) using Instrumentation Trace Macrocell (ITM)
    am_bsp_itm_printf_enable();
    am_util_stdio_printf("Test artemis SWO\n");
}
