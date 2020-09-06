///
/// @file artemis_servo.c
///
/// Adafruit 16-Channel 12-bit PWM/Servo Driver
/// https://www.adafruit.com/product/815
///

#include "artemis_i2c.h"
#include "artemis_servo.h"

#define ARTEMIS_SERVO_PCA9685_ADDRESS      (0x40)
#define ARTEMIS_SERVO_PCA9685_FREQUENCY    (25000000)
#define ARTEMIS_SERVO_PCA9685_MODE1        (0x00) // mode register 1
#define ARTEMIS_SERVO_PCA9685_MODE2        (0x01) // mode register 2
#define ARTEMIS_SERVO_PCA9685_SUBADR1      (0x02) // i2c subaddress 1
#define ARTEMIS_SERVO_PCA9685_SUBADR2      (0x03) // i2c subaddress 2
#define ARTEMIS_SERVO_PCA9685_SUBADR3      (0x04) // i2c subaddress 3
#define ARTEMIS_SERVO_PCA9685_ALLCALLADR   (0x05) // LED all call i2c address
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
}

///
///
///
void artemis_servo_update(uint64_t elapsed_us)
{
}
