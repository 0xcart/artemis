///
/// @file artemis_debug.h
///

#ifndef ARTEMIS_DEBUG_H
#define ARTEMIS_DEBUG_H

#include <stdbool.h>
#include <stdint.h>
#include <am_util_stdio.h>
#include <hal/am_hal_status.h>

#ifdef __cplusplus
extern "C" {
#endif

#ifdef NDEBUG
    #define ARTEMIS_DEBUG_ASSERT(expr) ((void)0)
    #define ARTEMIS_DEBUG_PRINTF(...) ((void)0)
    #define ARTEMIS_DEBUG_HALSTATUS(hfunc) (hfunc)
#else
    #define ARTEMIS_DEBUG_ASSERT(expr) (!!(expr) || (artemis_debug_assert(#expr, __FUNCTION__, __FILE__, __LINE__), 0))
    #define ARTEMIS_DEBUG_PRINTF(...) (am_util_stdio_printf(__VA_ARGS__))

    #define ARTEMIS_DEBUG_HALSTATUS(hfunc) \
    do { \
        uint32_t artemis_debug_halstatus = (hfunc); \
        if (artemis_debug_halstatus != AM_HAL_STATUS_SUCCESS) { \
            artemis_debug_halerror(#hfunc, artemis_debug_halstatus, __FUNCTION__, __FILE__, __LINE__); \
        } \
    } while(0)
#endif

void artemis_debug_initialize(void);
void artemis_debug_assert(const char *expr, const char *func, const char *file, uint32_t line);
void artemis_debug_halerror(const char *hfunc, uint32_t error, const char *func, const char *file, uint32_t line);

#ifdef __cplusplus
}
#endif

#endif
