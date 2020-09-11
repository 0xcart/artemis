///
/// @file artemis_pca9685.c
///
/// Adafruit 16-Channel 12-bit PWM/Servo Driver
/// https://www.adafruit.com/product/815
///

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

// PCA9685 mode 1
#define ARTEMIS_PCA9685_MODE1_RESTART    (0x80) // restart enabled
#define ARTEMIS_PCA9685_MODE1_EXTCLK     (0x40) // use EXTCLK pin clock
#define ARTEMIS_PCA9685_MODE1_AI         (0x20) // register Auto-Increment enabled
#define ARTEMIS_PCA9685_MODE1_SLEEP      (0x10) // low power mode; oscillator off
#define ARTEMIS_PCA9685_MODE1_SUB1       (0x08) // PCA9685 responds to I2C-bus subaddress 1
#define ARTEMIS_PCA9685_MODE1_SUB2       (0x04) // PCA9685 responds to I2C-bus subaddress 2
#define ARTEMIS_PCA9685_MODE1_SUB3       (0x02) // PCA9685 responds to I2C-bus subaddress 3
#define ARTEMIS_PCA9685_MODE1_ALLCALL    (0x01) // PCA9685 responds to LED All Call I2C-bus address

// PCA9685 mode 2
#define ARTEMIS_PCA9685_MODE2_INVRT      (0x10) // output logic state inverted
#define ARTEMIS_PCA9685_MODE2_OCH        (0x08) // outputs change on ACK instead of STOP
#define ARTEMIS_PCA9685_MODE2_OUTDRV     (0x04) // the 16 LEDn outputs are configured with a totem pole structure
#define ARTEMIS_PCA9685_MODE2_OUTNE10    (0x02) // LEDn = high-impedance
#define ARTEMIS_PCA9685_MODE2_OUTNE01    (0x01) // LEDn = 1 when OUTDRV = 1; LEDn = high-impedance when OUTDRV = 0 (same as OUTNE10)

// PCA9685 attributes
#define ARTEMIS_PCA9685_I2C_ADDRESS      (0x40)
#define ARTEMIS_PCA9685_OSCILLATOR_FREQ  (25000000)
#define ARTEMIS_PCA9685_STEP_MINIMUM     (0)    // 12-bit resolution (0 to 4095)
#define ARTEMIS_PCA9685_STEP_MAXIMUM     (4095) // 12-bit resolution (0 to 4095)
#define ARTEMIS_PCA9685_STEP_FULL        (4096) // signal fully on/off

// application specific
#define ARTEMIS_PCA9685_I2CBUFFER_SIZE   (8)   // bytes; must be a multiple of sizeof(uint32_t)

// while i2c operates on bytes the hal operates on 32-bit words
typedef uint32_t buffer_t[ARTEMIS_PCA9685_I2CBUFFER_SIZE / sizeof(uint32_t)];

typedef struct s_module_t
{
    artemis_i2c_t i2c;
    buffer_t txbuffer;
    buffer_t rxbuffer;
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
    i2c->stop = true;

    i2c->iom.module = ARTEMIS_IOM_MODULE_I2C0; // QWIIC
    i2c->iom.config.eInterfaceMode = AM_HAL_IOM_I2C_MODE;
    i2c->iom.config.ui32ClockFreq = AM_HAL_IOM_400KHZ;
    artemis_iom_initialize(&i2c->iom);
    am_hal_gpio_pinconfig(AM_BSP_GPIO_IOM4_SCL, g_AM_BSP_GPIO_IOM4_SCL);
    am_hal_gpio_pinconfig(AM_BSP_GPIO_IOM4_SDA, g_AM_BSP_GPIO_IOM4_SDA);

    module_pca9685_reset();
    module_pca9685_setfrequency(frequency);
    module_pca9685_reset();
}

///
///
///
void artemis_pca9685_setpwm(uint8_t pin, uint16_t value, bool invert)
{
    uint16_t on;
    uint16_t off;
    artemis_stream_t txstream;

    value = ARTEMIS_MATH_MIN(value, ARTEMIS_PCA9685_STEP_MAXIMUM);

    if (invert) {
        if (ARTEMIS_PCA9685_STEP_MAXIMUM == value) {
            on = 0;
            off = ARTEMIS_PCA9685_STEP_FULL;
        }
        else if (ARTEMIS_PCA9685_STEP_MINIMUM == value) {
            on = ARTEMIS_PCA9685_STEP_FULL;
            off = 0;
        }
        else {
            on = 0;
            off = ARTEMIS_PCA9685_STEP_MAXIMUM - value;
        }
    }
    else {
        if (ARTEMIS_PCA9685_STEP_MAXIMUM == value) {
            on = ARTEMIS_PCA9685_STEP_FULL;
            off = 0;
        }
        else if (ARTEMIS_PCA9685_STEP_MINIMUM == value) {
            on = 0;
            off = ARTEMIS_PCA9685_STEP_FULL;
        }
        else {
            on = 0;
            off = value;
        }
    }

    artemis_stream_setbuffer(&txstream, (uint8_t *)module.txbuffer, ARTEMIS_PCA9685_I2CBUFFER_SIZE);
    artemis_stream_put(&txstream, ARTEMIS_PCA9685_REG_LED0_ON_L + (pin * 4));
    artemis_stream_put(&txstream, on);
    artemis_stream_put(&txstream, on >> 8);
    artemis_stream_put(&txstream, off);
    artemis_stream_put(&txstream, off >> 8);
    artemis_i2c_send(&module.i2c, &txstream);
}

///
///
///
static void module_pca9685_reset(void)
{
    artemis_stream_t txstream;

    artemis_stream_setbuffer(&txstream, (uint8_t *)module.txbuffer, ARTEMIS_PCA9685_I2CBUFFER_SIZE);
    artemis_stream_put(&txstream, ARTEMIS_PCA9685_REG_MODE1);
    artemis_stream_put(&txstream, ARTEMIS_PCA9685_MODE1_RESTART);
    artemis_i2c_send(&module.i2c, &txstream);

    artemis_time_delayus(500);
}

///
///
///
static void module_pca9685_setfrequency(uint16_t frequency)
{
    uint8_t prescale;
    uint8_t oldmode = 0;
    uint8_t newmode = 0;
    artemis_stream_t txstream;
    artemis_stream_t rxstream;

    prescale = (uint8_t)(((ARTEMIS_PCA9685_OSCILLATOR_FREQ / (frequency * 4096.0f)) + 0.5f) - 1.0f);

    artemis_stream_setbuffer(&txstream, (uint8_t *)module.txbuffer, ARTEMIS_PCA9685_I2CBUFFER_SIZE);
    artemis_stream_put(&txstream, ARTEMIS_PCA9685_REG_MODE1);
    artemis_i2c_send(&module.i2c, &txstream);

    artemis_stream_setbuffer(&rxstream, (uint8_t *)module.rxbuffer, ARTEMIS_PCA9685_I2CBUFFER_SIZE);
    artemis_i2c_receive(&module.i2c, &rxstream, 1);
    artemis_stream_get(&rxstream, &oldmode);

    // prescale can only be set when in sleep mode
    newmode = (oldmode & ~ARTEMIS_PCA9685_MODE1_RESTART) | ARTEMIS_PCA9685_MODE1_SLEEP;

    artemis_stream_reset(&txstream);
    artemis_stream_put(&txstream, ARTEMIS_PCA9685_REG_MODE1);
    artemis_stream_put(&txstream, newmode);
    artemis_i2c_send(&module.i2c, &txstream);

    artemis_stream_reset(&txstream);
    artemis_stream_put(&txstream, ARTEMIS_PCA9685_REG_PRESCALE);
    artemis_stream_put(&txstream, prescale);
    artemis_i2c_send(&module.i2c, &txstream);

    artemis_stream_reset(&txstream);
    artemis_stream_put(&txstream, ARTEMIS_PCA9685_REG_MODE1);
    artemis_stream_put(&txstream, oldmode);
    artemis_i2c_send(&module.i2c, &txstream);

    artemis_time_delayus(500);
}
