///
/// @file artemis_uart.h
///

#ifndef ARTEMIS_UART_H
#define ARTEMIS_UART_H

#include "artemis_stream.h"
#include <stdint.h>
#include <am_bsp.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef enum e_artemis_uart_module_t
{
    ARTEMIS_UART_MODULE_0, // COM: TX0, RX0
    ARTEMIS_UART_MODULE_1  // Pins: D1/TX1, D0/RX1
} artemis_uart_module_t;

typedef struct s_artemis_uart_t
{
    void *handle;
    artemis_uart_module_t module;
    am_hal_uart_config_t config;
} artemis_uart_t;

void artemis_uart_initialize(artemis_uart_t *uart);
void artemis_uart_uninitialize(artemis_uart_t *uart);
void artemis_uart_flush(artemis_uart_t *uart);
void artemis_uart_send(artemis_uart_t *uart, artemis_stream_t *txstream);
void artemis_uart_receive(artemis_uart_t *uart, artemis_stream_t *rxstream, uint32_t rxnumber);

#ifdef __cplusplus
}
#endif

#endif
