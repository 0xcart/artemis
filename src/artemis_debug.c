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
    ARTEMIS_DEBUG_PRINTF("SWO ENABLED\n");
}

///
///
///
void artemis_debug_assert(const char *expr, const char *func, const char *file, uint32_t line)
{
    ARTEMIS_DEBUG_PRINTF("ASSERT FAILED: {\n");
    ARTEMIS_DEBUG_PRINTF("\texpr:\t%s\n", expr);
    ARTEMIS_DEBUG_PRINTF("\tfunc:\t%s\n", func);
    ARTEMIS_DEBUG_PRINTF("\tfile:\t%s\n", file);
    ARTEMIS_DEBUG_PRINTF("\tline:\t%u\n", line);
    ARTEMIS_DEBUG_PRINTF("}\n");

    while(1);
}

///
///
///
void artemis_debug_halerror(const char *hfunc, uint32_t error, const char *func, const char *file, uint32_t line)
{
    ARTEMIS_DEBUG_PRINTF("AM HAL ERROR: {\n");
    ARTEMIS_DEBUG_PRINTF("\thfunc:\t%s\n", hfunc);
    ARTEMIS_DEBUG_PRINTF("\terror:\t%u\n", error);
    ARTEMIS_DEBUG_PRINTF("\tfunc:\t%s\n", func);
    ARTEMIS_DEBUG_PRINTF("\tfile:\t%s\n", file);
    ARTEMIS_DEBUG_PRINTF("\tline:\t%u\n", line);
    ARTEMIS_DEBUG_PRINTF("}\n");

    while(1);
}
