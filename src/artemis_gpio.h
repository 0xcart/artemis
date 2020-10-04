///
/// @file artemis_gpio.h
///

#ifndef ARTEMIS_GPIO_H
#define ARTEMIS_GPIO_H

#include <am_bsp.h>

#ifdef __cplusplus
extern "C" {
#endif

#define ARTEMIS_GPIO_UART0_TX (48)
#define ARTEMIS_GPIO_UART0_RX (49)
#define ARTEMIS_GPIO_UART1_TX (24)
#define ARTEMIS_GPIO_UART1_RX (25)

extern const am_hal_gpio_pincfg_t artemis_gpio_pincfg_uart0_tx;
extern const am_hal_gpio_pincfg_t artemis_gpio_pincfg_uart0_rx;
extern const am_hal_gpio_pincfg_t artemis_gpio_pincfg_uart1_tx;
extern const am_hal_gpio_pincfg_t artemis_gpio_pincfg_uart1_rx;

#ifdef __cplusplus
}
#endif

#endif
