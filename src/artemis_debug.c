///
/// @file artemis_debug.c
///

#include "artemis_debug.h"
#include <am_bsp.h>
#include <stdlib.h>

///
///
///
void artemis_debug_initialize(void)
{
    // enable Single Wire Output (SWO) using Instrumentation Trace Macrocell (ITM)
    am_bsp_itm_printf_enable();
    ARTEMIS_DEBUG_PRINTF("SWO ENABLED\n");
}

///
///
///
void artemis_debug_assert(const char *expr, const char *func, const char *file, uint32_t line)
{
    ARTEMIS_DEBUG_PRINTF("ASSERT FAILED:\n\texpr:\t%s\n\tfunc:\t%s\n\tfile:\t%s\n\tline:\t%u\n", expr, func, file, line);
    abort();
}
