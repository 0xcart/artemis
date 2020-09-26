///
/// @file artemis_debug.h
///

#ifndef ARTEMIS_DEBUG_H
#define ARTEMIS_DEBUG_H

#include <stdbool.h>
#include <stdint.h>
#include <am_util_stdio.h>

#ifdef __cplusplus
extern "C" {
#endif

#ifdef NDEBUG
    #define ARTEMIS_DEBUG_ASSERT(expr) ((void)0)
    #define ARTEMIS_DEBUG_PRINTF(...) ((void)0)
    #define ARTEMIS_DEBUG_TASKINFO(name, elapsed_us) ((void)0)
#else
    #define ARTEMIS_DEBUG_ASSERT(expr) (!!(expr) || (artemis_debug_assert(#expr, __FUNCTION__, __FILE__, __LINE__), 0))
    #define ARTEMIS_DEBUG_PRINTF(...) (am_util_stdio_printf(__VA_ARGS__))
    #define ARTEMIS_DEBUG_TASKINFO(name, elapsed_us) (am_util_stdio_printf("%s:\t\t%llu\n", name, elapsed_us))
#endif

void artemis_debug_initialize(void);
void artemis_debug_assert(const char *expr, const char *func, const char *file, uint32_t line);

#ifdef __cplusplus
}
#endif

#endif
