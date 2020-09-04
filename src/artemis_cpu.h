///
/// @file artemis_cpu.h
///

#ifndef ARTEMIS_CPU_H
#define ARTEMIS_CPU_H

#ifdef __cplusplus
extern "C" {
#endif

void artemis_cpu_initialize(void);
void artemis_cpu_enableburst(void);
void artemis_cpu_disableburst(void);

#ifdef __cplusplus
}
#endif

#endif
