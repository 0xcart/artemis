///
/// @file artemis_servo.c
///
/// Adafruit 16-Channel 12-bit PWM/Servo Driver
/// https://www.adafruit.com/product/815
///

#include "artemis_i2c.h"
#include "artemis_math.h"
#include "artemis_servo.h"

#define ARTEMIS_SERVO_PCA9685_ADDRESS      (0x40)
#define ARTEMIS_SERVO_PCA9685_FREQUENCY    (25000000)
#define ARTEMIS_SERVO_PCA9685_MINSTEP      (0)    // 12-bit resolution (0 to 4095)
#define ARTEMIS_SERVO_PCA9685_MAXSTEP      (4095) // 12-bit resolution (0 to 4095)
#define ARTEMIS_SERVO_PCA9685_MODE1        (0x00) // mode register 1
#define ARTEMIS_SERVO_PCA9685_MODE2        (0x01) // mode register 2
#define ARTEMIS_SERVO_PCA9685_SUBADR1      (0x02) // i2c subaddress 1
#define ARTEMIS_SERVO_PCA9685_SUBADR2      (0x03) // i2c subaddress 2
#define ARTEMIS_SERVO_PCA9685_SUBADR3      (0x04) // i2c subaddress 3
#define ARTEMIS_SERVO_PCA9685_ALLCALLADR   (0x05) // LED all call i2c address
#define ARTEMIS_SERVO_PCA9685_LED0_ON_L    (0x06) // LED0 on tick, low byte
#define ARTEMIS_SERVO_PCA9685_LED0_ON_H    (0x07) // LED0 on tick, high byte
#define ARTEMIS_SERVO_PCA9685_LED0_OFF_L   (0x08) // LED0 off tick, low byte
#define ARTEMIS_SERVO_PCA9685_LED0_OFF_H   (0x09) // LED0 off tick, high byte
#define ARTEMIS_SERVO_MODE1_RESTART        (0x80) // restart enabled
#define ARTEMIS_SERVO_PCA9685_ALLLED_ON_L  (0xFA) // load all the LEDn_ON registers, low
#define ARTEMIS_SERVO_PCA9685_ALLLED_ON_H  (0xFB) // load all the LEDn_ON registers, high
#define ARTEMIS_SERVO_PCA9685_ALLLED_OFF_L (0xFC) // load all the LEDn_OFF registers, low
#define ARTEMIS_SERVO_PCA9685_ALLLED_OFF_H (0xFD) // load all the LEDn_OFF registers, high
#define ARTEMIS_SERVO_PCA9685_PRESCALE     (0xFE) // prescaler for PWM output frequency
#define ARTEMIS_SERVO_PCA9685_TESTMODE     (0xFF) // defines the test mode to be entered
#define ARTEMIS_SERVO_I2CBUFFER_SIZE       (32)   // bytes; must be a multiple of sizeof(uint32_t)

// while i2c operates on bytes the hal operates on 32-bit words
typedef uint32_t buffer_t[ARTEMIS_SERVO_I2CBUFFER_SIZE / sizeof(uint32_t)];

typedef struct s_module_t
{
    artemis_i2c_t i2c;
    buffer_t txbuffer;
    buffer_t rxbuffer;
} module_t;

static module_t module;

static void module_servo_reset(void);

///
///
///
void artemis_servo_initialize(void)
{
    artemis_i2c_t *i2c = &module.i2c;
    i2c->iom.module = ARTEMIS_IOM_MODULE_I2C0; // QWIIC
    i2c->iom.config.eInterfaceMode = AM_HAL_IOM_I2C_MODE;
    i2c->iom.config.ui32ClockFreq = AM_HAL_IOM_400KHZ;
    i2c->address = ARTEMIS_SERVO_PCA9685_ADDRESS;
    i2c->stop = true;
    artemis_stream_setbuffer(&i2c->iom.txstream, (uint8_t *)module.txbuffer, ARTEMIS_SERVO_I2CBUFFER_SIZE);
    artemis_stream_setbuffer(&i2c->iom.rxstream, (uint8_t *)module.rxbuffer, ARTEMIS_SERVO_I2CBUFFER_SIZE);
    artemis_iom_initialize(&i2c->iom);
    am_hal_gpio_pinconfig(AM_BSP_GPIO_IOM4_SCL, g_AM_BSP_GPIO_IOM4_SCL);
    am_hal_gpio_pinconfig(AM_BSP_GPIO_IOM4_SDA, g_AM_BSP_GPIO_IOM4_SDA);

    module_servo_reset();
}

///
///
///
void artemis_servo_update(uint64_t elapsed_us)
{
}

///
///
///
void artemis_servo_pwm(uint8_t pin, uint16_t value)
{
    uint16_t on;
    uint16_t off;
    artemis_stream_t *txstream = &module.i2c.iom.txstream;

    value = ARTEMIS_MATH_MIN(value, ARTEMIS_SERVO_PCA9685_MAXSTEP);

    if (ARTEMIS_SERVO_PCA9685_MAXSTEP == value) {
        on = ARTEMIS_SERVO_PCA9685_MAXSTEP;
        off = 0;
    }
    else if (ARTEMIS_SERVO_PCA9685_MINSTEP == value) {
        on = 0;
        off = ARTEMIS_SERVO_PCA9685_MAXSTEP;
    }
    else {
        on = 0;
        off = value;
    }

    artemis_stream_reset(txstream);
    artemis_stream_put(txstream, ARTEMIS_SERVO_PCA9685_LED0_ON_L + (pin * 4));
    artemis_stream_put(txstream, on);
    artemis_stream_put(txstream, on >> 8);
    artemis_stream_put(txstream, off);
    artemis_stream_put(txstream, off >> 8);
    artemis_i2c_send(&module.i2c);
}

///
///
///
static void module_servo_reset(void)
{
    artemis_stream_t *txstream = &module.i2c.iom.txstream;

    artemis_stream_reset(txstream);
    artemis_stream_put(txstream, ARTEMIS_SERVO_PCA9685_MODE1);
    artemis_stream_put(txstream, ARTEMIS_SERVO_MODE1_RESTART);
    artemis_i2c_send(&module.i2c);
}
