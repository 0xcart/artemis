///
/// @file artemis_servo.h
///

#ifndef ARTEMIS_SERVO_H
#define ARTEMIS_SERVO_H

#include <stdbool.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef enum e_artemis_servo_index_t
{
    ARTEMIS_SERVO_INDEX_HIPX_R,
    ARTEMIS_SERVO_INDEX_HIPZ_R,
    ARTEMIS_SERVO_INDEX_KNEE_R,
    ARTEMIS_SERVO_INDEX_SHLDX_R,
    ARTEMIS_SERVO_INDEX_SHLDZ_R,
    ARTEMIS_SERVO_INDEX_ELBOW_R,
    ARTEMIS_SERVO_INDEX_SHLDX_L,
    ARTEMIS_SERVO_INDEX_SHLDZ_L,
    ARTEMIS_SERVO_INDEX_ELBOW_L,
    ARTEMIS_SERVO_INDEX_HIPX_L,
    ARTEMIS_SERVO_INDEX_HIPZ_L,
    ARTEMIS_SERVO_INDEX_KNEE_L,
    ARTEMIS_SERVO_INDEX_COUNT
} artemis_servo_index_t;

typedef struct s_artemis_servo_t
{
    uint16_t value;
    uint16_t minimum;
    uint16_t maximum;
    uint16_t center;
    bool invert;
} artemis_servo_t;

void artemis_servo_initialize(void);

#ifdef __cplusplus
}
#endif

#endif
