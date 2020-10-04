///
/// @file artemis_jetiexbus.c
///

#include "artemis_debug.h"
#include "artemis_gpio.h"
#include "artemis_jetiexbus.h"
#include "artemis_uart.h"
#include <stdint.h>

#define ARTEMIS_JETIEXBUS_BAUDRATE      (125000) // exbus baudrate
#define ARTEMIS_JETIEXBUS_BUFFER_LENGTH (48)     // transmit and receive buffer length

typedef uint8_t module_buffer_t[ARTEMIS_JETIEXBUS_BUFFER_LENGTH];

typedef struct s_module_t
{
    artemis_uart_t uart;
    module_buffer_t txbuffer;
    module_buffer_t rxbuffer;
} module_t;

static module_t module;

///
///
///
void artemis_jetiexbus_initialize(void)
{
    artemis_uart_t *uart = &module.uart;

    uart->module = ARTEMIS_UART_MODULE_1;
    uart->config.ui32BaudRate = ARTEMIS_JETIEXBUS_BAUDRATE;
    uart->config.ui32DataBits = AM_HAL_UART_DATA_BITS_8;
    uart->config.ui32Parity = AM_HAL_UART_PARITY_NONE;
    uart->config.ui32StopBits = AM_HAL_UART_ONE_STOP_BIT;
    uart->config.ui32FlowControl = AM_HAL_UART_FLOW_CTRL_NONE;
    uart->config.ui32FifoLevels = 0;
    artemis_uart_initialize(uart);

    ARTEMIS_DEBUG_HALSTATUS(am_hal_gpio_pinconfig(ARTEMIS_GPIO_UART1_TX, artemis_gpio_pincfg_uart1_tx));
    ARTEMIS_DEBUG_HALSTATUS(am_hal_gpio_pinconfig(ARTEMIS_GPIO_UART1_RX, artemis_gpio_pincfg_uart1_rx));
}
