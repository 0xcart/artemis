///
/// @file artemis_pca9685.c
///
/// Adafruit 16-Channel 12-bit PWM/Servo Driver
/// https://www.adafruit.com/product/815
///

#include "artemis_debug.h"
#include "artemis_i2c.h"
#include "artemis_math.h"
#include "artemis_pca9685.h"
#include "artemis_time.h"

// PCA9685 registers
#define ARTEMIS_PCA9685_REG_MODE1        (0x00) // mode register 1
#define ARTEMIS_PCA9685_REG_MODE2        (0x01) // mode register 2
#define ARTEMIS_PCA9685_REG_SUBADR1      (0x02) // I2C-bus subaddress 1
#define ARTEMIS_PCA9685_REG_SUBADR2      (0x03) // I2C-bus subaddress 2
#define ARTEMIS_PCA9685_REG_SUBADR3      (0x04) // I2C-bus subaddress 3
#define ARTEMIS_PCA9685_REG_ALLCALLADR   (0x05) // LED All Call I2C-bus address
#define ARTEMIS_PCA9685_REG_LED0_ON_L    (0x06) // LED0 output and brightness control byte 0
#define ARTEMIS_PCA9685_REG_LED0_ON_H    (0x07) // LED0 output and brightness control byte 1
#define ARTEMIS_PCA9685_REG_LED0_OFF_L   (0x08) // LED0 output and brightness control byte 2
#define ARTEMIS_PCA9685_REG_LED0_OFF_H   (0x09) // LED0 output and brightness control byte 3
#define ARTEMIS_PCA9685_REG_ALLLED_ON_L  (0xFA) // load all the LEDn_ON registers, byte 0
#define ARTEMIS_PCA9685_REG_ALLLED_ON_H  (0xFB) // load all the LEDn_ON registers, byte 1
#define ARTEMIS_PCA9685_REG_ALLLED_OFF_L (0xFC) // load all the LEDn_OFF registers, byte 0
#define ARTEMIS_PCA9685_REG_ALLLED_OFF_H (0xFD) // load all the LEDn_OFF registers, byte 1
#define ARTEMIS_PCA9685_REG_PRESCALE     (0xFE) // prescaler for PWM output frequency
#define ARTEMIS_PCA9685_REG_TESTMODE     (0xFF) // defines the test mode to be entered

// PCA9685 mode 1 bit masks
#define ARTEMIS_PCA9685_MODE1_RESTART    (0x80) // restart enabled
#define ARTEMIS_PCA9685_MODE1_EXTCLK     (0x40) // use EXTCLK pin clock
#define ARTEMIS_PCA9685_MODE1_AI         (0x20) // register Auto-Increment enabled
#define ARTEMIS_PCA9685_MODE1_SLEEP      (0x10) // low power mode; oscillator off
#define ARTEMIS_PCA9685_MODE1_SUB1       (0x08) // PCA9685 responds to I2C-bus subaddress 1
#define ARTEMIS_PCA9685_MODE1_SUB2       (0x04) // PCA9685 responds to I2C-bus subaddress 2
#define ARTEMIS_PCA9685_MODE1_SUB3       (0x02) // PCA9685 responds to I2C-bus subaddress 3
#define ARTEMIS_PCA9685_MODE1_ALLCALL    (0x01) // PCA9685 responds to LED All Call I2C-bus address

// PCA9685 mode 2 bit masks
#define ARTEMIS_PCA9685_MODE2_INVRT      (0x10) // output logic state inverted
#define ARTEMIS_PCA9685_MODE2_OCH        (0x08) // outputs change on ACK instead of STOP
#define ARTEMIS_PCA9685_MODE2_OUTDRV     (0x04) // the 16 LEDn outputs are configured with a totem pole structure
#define ARTEMIS_PCA9685_MODE2_OUTNE10    (0x02) // LEDn = high-impedance
#define ARTEMIS_PCA9685_MODE2_OUTNE01    (0x01) // LEDn = 1 when OUTDRV = 1; LEDn = high-impedance when OUTDRV = 0 (same as OUTNE10)

// PCA9685 attributes
#define ARTEMIS_PCA9685_I2C_ADDRESS      (0x40) // I2C default address for the Adafruit board
#define ARTEMIS_PCA9685_OSCILLATOR_FREQ  (25703000) // calibrated with an oscilloscope for this specific pca9685 chip
#define ARTEMIS_PCA9685_STEP_MINIMUM     (0)    // 12-bit resolution (0 to 4095)
#define ARTEMIS_PCA9685_STEP_MAXIMUM     (4095) // 12-bit resolution (0 to 4095)

// application specific
#define ARTEMIS_PCA9685_BUFFER_LENGTH    (8)    // transmit and receive buffer length

typedef uint8_t module_buffer_t[ARTEMIS_PCA9685_BUFFER_LENGTH];

typedef struct s_module_t
{
    artemis_i2c_t i2c;
    module_buffer_t txbuffer;
    module_buffer_t rxbuffer;
} module_t;

static module_t module;

static void module_pca9685_reset(void);
static void module_pca9685_setfrequency(uint16_t frequency);

///
///
///
void artemis_pca9685_initialize(uint16_t frequency)
{
    artemis_i2c_t *i2c = &module.i2c;

    i2c->address = ARTEMIS_PCA9685_I2C_ADDRESS;

    i2c->iom.module = ARTEMIS_IOM_MODULE_I2C0;
    i2c->iom.config.eInterfaceMode = AM_HAL_IOM_I2C_MODE;
    i2c->iom.config.ui32ClockFreq = AM_HAL_IOM_400KHZ;
    artemis_iom_initialize(&i2c->iom);

    ARTEMIS_DEBUG_HALSTATUS(am_hal_gpio_pinconfig(AM_BSP_GPIO_IOM4_SCL, g_AM_BSP_GPIO_IOM4_SCL));
    ARTEMIS_DEBUG_HALSTATUS(am_hal_gpio_pinconfig(AM_BSP_GPIO_IOM4_SDA, g_AM_BSP_GPIO_IOM4_SDA));

    module_pca9685_reset();
    module_pca9685_setfrequency(frequency);
}

///
///
///
void artemis_pca9685_setpwm(uint8_t pin, uint16_t off)
{
    artemis_stream_t txstream = {0};

    off = ARTEMIS_MATH_MIN(off, ARTEMIS_PCA9685_STEP_MAXIMUM);

    artemis_stream_setbuffer(&txstream, module.txbuffer, ARTEMIS_PCA9685_BUFFER_LENGTH);
    artemis_stream_put(&txstream, ARTEMIS_PCA9685_REG_LED0_OFF_L + (pin * 4));
    artemis_stream_put(&txstream, off);
    artemis_stream_put(&txstream, off >> 8);
    artemis_i2c_send(&module.i2c, true, &txstream);
}

///
///
///
static void module_pca9685_reset(void)
{
    artemis_stream_t txstream = {0};

    artemis_stream_setbuffer(&txstream, module.txbuffer, ARTEMIS_PCA9685_BUFFER_LENGTH);
    artemis_stream_put(&txstream, ARTEMIS_PCA9685_REG_MODE1);
    artemis_stream_put(&txstream, ARTEMIS_PCA9685_MODE1_RESTART);
    artemis_i2c_send(&module.i2c, true, &txstream);

    artemis_time_delayus(500);
}

///
///
///
static void module_pca9685_setfrequency(uint16_t frequency)
{
    uint8_t prescale;
    artemis_stream_t txstream = {0};

    prescale = (uint8_t)(((ARTEMIS_PCA9685_OSCILLATOR_FREQ / (frequency * 4096.0f)) + 0.5f) - 1.0f);

    // prescale can only be set when in sleep mode
    artemis_stream_setbuffer(&txstream, module.txbuffer, ARTEMIS_PCA9685_BUFFER_LENGTH);
    artemis_stream_put(&txstream, ARTEMIS_PCA9685_REG_MODE1);
    artemis_stream_put(&txstream, ARTEMIS_PCA9685_MODE1_SLEEP);
    artemis_i2c_send(&module.i2c, true, &txstream);

    artemis_stream_reset(&txstream);
    artemis_stream_put(&txstream, ARTEMIS_PCA9685_REG_PRESCALE);
    artemis_stream_put(&txstream, prescale);
    artemis_i2c_send(&module.i2c, true, &txstream);

    artemis_stream_reset(&txstream);
    artemis_stream_put(&txstream, ARTEMIS_PCA9685_REG_MODE1);
    artemis_stream_put(&txstream, ARTEMIS_PCA9685_MODE1_AI);
    artemis_i2c_send(&module.i2c, true, &txstream);

    artemis_time_delayus(500);
}
