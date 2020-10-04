///
/// @file artemis_uart.c
///

#include "artemis_debug.h"
#include "artemis_uart.h"

#define ARTEMIS_UART_BUFFER_LENGTH (256) // transmit and receive buffer length

typedef uint8_t module_buffer_t[ARTEMIS_UART_BUFFER_LENGTH];

typedef struct s_module_t
{
    module_buffer_t txbuffer;
    module_buffer_t rxbuffer;
} module_t;

static module_t module;

///
///
///
void artemis_uart_initialize(artemis_uart_t *uart)
{
    if (!uart->config.pui8TxBuffer || !uart->config.ui32TxBufferSize) {
        uart->config.pui8TxBuffer = module.txbuffer;
        uart->config.ui32TxBufferSize = ARTEMIS_UART_BUFFER_LENGTH;
    }

    if (!uart->config.pui8RxBuffer || !uart->config.ui32RxBufferSize) {
        uart->config.pui8RxBuffer = module.rxbuffer;
        uart->config.ui32RxBufferSize = ARTEMIS_UART_BUFFER_LENGTH;
    }

    ARTEMIS_DEBUG_HALSTATUS(am_hal_uart_initialize(uart->module, &uart->handle));
    ARTEMIS_DEBUG_HALSTATUS(am_hal_uart_power_control(uart->handle, AM_HAL_SYSCTRL_WAKE, false));
    ARTEMIS_DEBUG_HALSTATUS(am_hal_uart_configure(uart->handle, &uart->config));
}

///
///
///
void artemis_uart_uninitialize(artemis_uart_t *uart)
{
    ARTEMIS_DEBUG_HALSTATUS(am_hal_uart_power_control(uart->handle, AM_HAL_SYSCTRL_DEEPSLEEP, false));
    ARTEMIS_DEBUG_HALSTATUS(am_hal_uart_deinitialize(uart->handle));
    uart->handle = 0;
}

///
///
///
void artemis_uart_flush(artemis_uart_t *uart)
{
    ARTEMIS_DEBUG_HALSTATUS(am_hal_uart_tx_flush(uart->handle));
}

///
///
///
void artemis_uart_send(artemis_uart_t *uart, artemis_stream_t *txstream)
{
    uint32_t read = 0;
    am_hal_uart_transfer_t transfer = {0};

    transfer.ui32Direction = AM_HAL_UART_WRITE;
    transfer.pui8Data = txstream->buffer;
    transfer.ui32NumBytes = txstream->written;
    transfer.ui32TimeoutMs = 0;
    transfer.pui32BytesTransferred = &read;

    ARTEMIS_DEBUG_HALSTATUS(am_hal_uart_transfer(uart->handle, &transfer));

    // update the number of bytes read from the txstream
    txstream->read = read;
}

///
///
///
void artemis_uart_receive(artemis_uart_t *uart, artemis_stream_t *rxstream, uint32_t rxnumber)
{
    uint32_t written = 0;
    am_hal_uart_transfer_t transfer = {0};

    transfer.ui32Direction = AM_HAL_UART_READ;
    transfer.pui8Data = rxstream->buffer;
    transfer.ui32NumBytes = rxnumber;
    transfer.ui32TimeoutMs = 0;
    transfer.pui32BytesTransferred = &written;

    ARTEMIS_DEBUG_HALSTATUS(am_hal_uart_transfer(uart->handle, &transfer));

    // update the number of bytes written to the rxstream
    rxstream->written = written;
}
