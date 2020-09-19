///
/// @file artemis_util.h
///

#ifndef ARTEMIS_UTIL_H
#define ARTEMIS_UTIL_H

#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#define ARTEMIS_UTIL_ARRAY_SIZE(a)    (sizeof(a) / sizeof(a[0]))
#define ARTEMIS_UTIL_OFFSET(s, m)     (offsetof(s, m))
#define ARTEMIS_UTIL_OFFSET_PTR(s, m) ((uint8_t*)0 + offsetof(s, m))

typedef enum s_artemis_util_bitorder_t
{
    ARTEMIS_UTIL_BITORDER_MSB,
    ARTEMIS_UTIL_BITORDER_LSB
} artemis_util_bitorder_t;

#ifdef __cplusplus
}
#endif

#endif
