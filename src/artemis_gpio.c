///
/// @file artemis_pins.c
///

#include "artemis_gpio.h"

///
/// COM UART transmit pin 
///
const am_hal_gpio_pincfg_t artemis_gpio_pincfg_uart0_tx = {
    .uFuncSel       = AM_HAL_PIN_48_UART0TX,
    .eDriveStrength = AM_HAL_GPIO_PIN_DRIVESTRENGTH_2MA
};

///
/// COM UART receive pin
///
const am_hal_gpio_pincfg_t artemis_gpio_pincfg_uart0_rx = {
    .uFuncSel       = AM_HAL_PIN_49_UART0RX
};

///
/// general purpose UART transmit pin
///
const am_hal_gpio_pincfg_t artemis_gpio_pincfg_uart1_tx = {
    .uFuncSel       = AM_HAL_PIN_24_UART1TX,
    .eDriveStrength = AM_HAL_GPIO_PIN_DRIVESTRENGTH_2MA
};

///
/// general purpose UART receive pin
///
const am_hal_gpio_pincfg_t artemis_gpio_pincfg_uart1_rx = {
    .uFuncSel       = AM_HAL_PIN_25_UART1RX
};
