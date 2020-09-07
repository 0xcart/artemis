///
/// @file artemis_math.h
///

#ifndef ARTEMIS_MATH_H
#define ARTEMIS_MATH_H

#include <math.h>

#ifdef __cplusplus
extern "C" {
#endif

#define ARTEMIS_MATH_MAX(a, b) \
(__extension__({ \
    __typeof__(a) _a = (a); \
    __typeof__(b) _b = (b); \
    _a > _b ? _a : _b; \
}))

#define ARTEMIS_MATH_MIN(a, b) \
(__extension__({ \
    __typeof__(a) _a = (a); \
    __typeof__(b) _b = (b); \
    _a < _b ? _a : _b; \
}))

#ifdef __cplusplus
}
#endif

#endif
