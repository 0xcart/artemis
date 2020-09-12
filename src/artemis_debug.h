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

#ifdef ARTEMIS_NDEBUG
    #define ARTEMIS_DEBUG_TASKINFO(_name, _elapsed_us) ((void)0)
#else
    #define ARTEMIS_DEBUG_TASKINFO(_name, _elapsed_us) (am_util_stdio_printf("%s: %llu\n", _name, _elapsed_us))
#endif

#ifdef __cplusplus
}
#endif

#endif
