///
/// @file artemis_mcu.h
///

#ifndef ARTEMIS_MCU_H
#define ARTEMIS_MCU_H

#ifdef __cplusplus
extern "C" {
#endif

void artemis_mcu_initialize(void);
void artemis_mcu_enableburst(void);
void artemis_mcu_disableburst(void);

#ifdef __cplusplus
}
#endif

#endif
