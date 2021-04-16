///
/// @file artemis_icm20649.c
///

#include "artemis_debug.h"
#include "artemis_icm20649.h"
#include "artemis_spi.h"
#include "artemis_time.h"
#include <stddef.h>

// ICM20649 bank select register
#define ARTEMIS_ICM20649_REG_BANK_SELECT        (0x7F) // bank select

// ICM20649 bank 0 registers
#define ARTEMIS_ICM20649_REG_WHO_AM_I           (0x00) // who am I
#define ARTEMIS_ICM20649_REG_USER_CTRL          (0x03) // user control
#define ARTEMIS_ICM20649_REG_PWR_MGMT_1         (0x06) // power management 1
#define ARTEMIS_ICM20649_REG_PWR_MGMT_2         (0x07) // power management 2
#define ARTEMIS_ICM20649_REG_ACCEL_XOUT_H       (0x2D) // high byte of accel x-axis data
#define ARTEMIS_ICM20649_REG_GYRO_XOUT_H        (0x33) // high byte of gyro x-axis data

// ICM20649 bank 2 registers
#define ARTEMIS_ICM20649_REG_ACCEL_SMPLRT_DIV1  (0x10) // accel sample rate divider msb[11:8]; 1125Hz / (1 + SAMPLE_RATE_DIVIDER[11:0])
#define ARTEMIS_ICM20649_REG_ACCEL_SMPLRT_DIV2  (0x11) // accel sample rate divider lsb[ 7:0]; 1125Hz / (1 + SAMPLE_RATE_DIVIDER[11:0])
#define ARTEMIS_ICM20649_REG_ACCEL_CONFIG       (0x14) // accel configuration
#define ARTEMIS_ICM20649_REG_GYRO_SMPLRT_DIV    (0x00) // gyro sample rate divider; 1100Hz / (1 + SAMPLE_RATE_DIVIDER[7:0])
#define ARTEMIS_ICM20649_REG_GYRO_CONFIG        (0x01) // gyro configuration

// ICM20649 bank select bit masks
#define ARTEMIS_ICM20649_BANK_SELECT_0          (0x00) // bank select 0
#define ARTEMIS_ICM20649_BANK_SELECT_2          (0x20) // bank select 2

// ICM20649 bank 0 bit masks
#define ARTEMIS_ICM20649_BANK0_DMP_EN           (0x80) // enable Digital Motion Processor (DMP)
#define ARTEMIS_ICM20649_BANK0_I2C_IF_DIS       (0x10) // disable I2C interface
#define ARTEMIS_ICM20649_BANK0_DEVICE_RESET     (0x80) // reset the device
#define ARTEMIS_ICM20649_BANK0_TEMP_DIS         (0x08) // disable temperature sensor
#define ARTEMIS_ICM20649_BANK0_CLKSEL           (0x01) // clock select

// ICM20649 bank 2 bit masks
#define ARTEMIS_ICM20649_BANK2_ACCEL_FS_SEL_4   (0x00) // accel full scale select: ±4g
#define ARTEMIS_ICM20649_BANK2_ACCEL_FS_SEL_8   (0x02) // accel full scale select: ±8g
#define ARTEMIS_ICM20649_BANK2_ACCEL_FS_SEL_16  (0x04) // accel full scale select: ±16g
#define ARTEMIS_ICM20649_BANK2_ACCEL_FS_SEL_30  (0x06) // accel full scale select: ±30g
#define ARTEMIS_ICM20649_BANK2_ACCEL_FCHOICE    (0x01) // enable accel DLPF
#define ARTEMIS_ICM20649_BANK2_GYRO_FS_SEL_500  (0x00) // gyro full scale select: ±500dps
#define ARTEMIS_ICM20649_BANK2_GYRO_FS_SEL_1000 (0x02) // gyro full scale select: ±1000dps
#define ARTEMIS_ICM20649_BANK2_GYRO_FS_SEL_2000 (0x04) // gyro full scale select: ±2000dps
#define ARTEMIS_ICM20649_BANK2_GYRO_FS_SEL_4000 (0x06) // gyro full scale select: ±4000dps
#define ARTEMIS_ICM20649_BANK2_GYRO_FCHOICE     (0x01) // enable gyro DLPF

// ICM20649 attributes
#define ARTEMIS_ICM20649_IDENTITY               (0xE1) // ICM20649 identity
#define ARTEMIS_ICM20649_SCLK_FREQ              (7000000) // SPI SCLK frequency
#define ARTEMIS_ICM20649_ACCEL_INTERNAL_SMPLRT  (1125) // accel internal sample rate
#define ARTEMIS_ICM20649_ACCEL_SENSITIVITY      (2048.0f) // accel sensitivity scale factor for ACCEL_FS_SEL=2 (±16g)
#define ARTEMIS_ICM20649_ACCEL_Z_1G             (1.0f) // z-axis 1G acceleration
#define ARTEMIS_ICM20649_GYRO_INTERNAL_SMPLRT   (1100) // gyro internal sample rate
#define ARTEMIS_ICM20649_GYRO_SENSITIVITY       (16.384f) // gyro sensitivity scale factor for GYRO_FS_SEL=2 (±2000dps)

// application specific
#define ARTEMIS_ICM20649_BUFFER_LENGTH          (8)    // transmit and receive buffer length

typedef uint8_t module_buffer_t[ARTEMIS_ICM20649_BUFFER_LENGTH];
typedef int16_t module_data_t[ARTEMIS_IMU_AXIS_COUNT];

typedef struct s_module_t
{
    artemis_spi_t spi;
    module_buffer_t txbuffer;
    module_buffer_t rxbuffer;
} module_t;

static module_t module;

static void module_icm20649_reset(void);
static void module_icm20649_identity(void);
static void module_icm20649_configure(void);
static void module_icm20649_transfer(uint8_t reg, uint8_t data);
static void module_icm20649_readsensor(uint8_t reg, module_data_t *data);

///
///
///
void artemis_icm20649_initialize(void)
{
    artemis_spi_t *spi = &module.spi;

    spi->chipselect = AM_BSP_IOM0_CS_CHNL;

    spi->iom.module = ARTEMIS_IOM_MODULE_SPI0;
    spi->iom.config.eInterfaceMode = AM_HAL_IOM_SPI_MODE;
    spi->iom.config.ui32ClockFreq = ARTEMIS_ICM20649_SCLK_FREQ;
    spi->iom.config.eSpiMode = AM_HAL_IOM_SPI_MODE_0;
    artemis_iom_initialize(&spi->iom);

    ARTEMIS_DEBUG_HALSTATUS(am_hal_gpio_pinconfig(AM_BSP_GPIO_IOM0_SCK, g_AM_BSP_GPIO_IOM0_SCK));
    ARTEMIS_DEBUG_HALSTATUS(am_hal_gpio_pinconfig(AM_BSP_GPIO_IOM0_MISO, g_AM_BSP_GPIO_IOM0_MISO));
    ARTEMIS_DEBUG_HALSTATUS(am_hal_gpio_pinconfig(AM_BSP_GPIO_IOM0_MOSI, g_AM_BSP_GPIO_IOM0_MOSI));
    ARTEMIS_DEBUG_HALSTATUS(am_hal_gpio_pinconfig(AM_BSP_GPIO_IOM0_CS, g_AM_BSP_GPIO_IOM0_CS));

    module_icm20649_reset();
    module_icm20649_identity();
    module_icm20649_configure();
}

///
///
///
void artemis_icm20649_readaccel(artemis_icm20649_data_t *accel)
{
    module_data_t data;

    module_icm20649_readsensor(ARTEMIS_ICM20649_REG_ACCEL_XOUT_H, &data);

    for (size_t i = 0; i < ARTEMIS_IMU_AXIS_COUNT; i++) {
        (*accel)[i] = data[i] / ARTEMIS_ICM20649_ACCEL_SENSITIVITY;
    }

    (*accel)[ARTEMIS_IMU_AXIS_Z] -= ARTEMIS_ICM20649_ACCEL_Z_1G;
}

///
///
///
void artemis_icm20649_readgyro(artemis_icm20649_data_t *gyro)
{
    module_data_t data;

    module_icm20649_readsensor(ARTEMIS_ICM20649_REG_GYRO_XOUT_H, &data);

    for (size_t i = 0; i < ARTEMIS_IMU_AXIS_COUNT; i++) {
        (*gyro)[i] = data[i] / ARTEMIS_ICM20649_GYRO_SENSITIVITY;
    }
}

///
///
///
static void module_icm20649_reset(void)
{
    module_icm20649_transfer(ARTEMIS_ICM20649_REG_BANK_SELECT, ARTEMIS_ICM20649_BANK_SELECT_0);
    module_icm20649_transfer(ARTEMIS_ICM20649_REG_PWR_MGMT_1, ARTEMIS_ICM20649_BANK0_DEVICE_RESET);

    artemis_time_delayms(100);
}

///
///
///
static void module_icm20649_identity(void)
{
    uint8_t identity = 0;
    artemis_stream_t txstream = {0};
    artemis_stream_t rxstream = {0};

    module_icm20649_transfer(ARTEMIS_ICM20649_REG_BANK_SELECT, ARTEMIS_ICM20649_BANK_SELECT_0);

    artemis_stream_setbuffer(&txstream, module.txbuffer, ARTEMIS_ICM20649_BUFFER_LENGTH);
    artemis_stream_setbuffer(&rxstream, module.rxbuffer, ARTEMIS_ICM20649_BUFFER_LENGTH);
    artemis_stream_put(&txstream, ARTEMIS_ICM20649_REG_WHO_AM_I | ARTEMIS_SPI_READBIT_MSB);
    artemis_spi_transfer(&module.spi, false, &txstream, &rxstream);

    artemis_stream_reset(&rxstream);
    artemis_spi_receive(&module.spi, true, &rxstream, 1);
    artemis_stream_get(&rxstream, &identity);

    ARTEMIS_DEBUG_ASSERT(identity == ARTEMIS_ICM20649_IDENTITY);
}

///
///
///
static void module_icm20649_configure(void)
{
    // select bank 0
    module_icm20649_transfer(ARTEMIS_ICM20649_REG_BANK_SELECT, ARTEMIS_ICM20649_BANK_SELECT_0);

    // enable DMP and disable I2C interface
    module_icm20649_transfer(ARTEMIS_ICM20649_REG_USER_CTRL, ARTEMIS_ICM20649_BANK0_DMP_EN | ARTEMIS_ICM20649_BANK0_I2C_IF_DIS);

    // auto select best available clock source
    module_icm20649_transfer(ARTEMIS_ICM20649_REG_PWR_MGMT_1, ARTEMIS_ICM20649_BANK0_CLKSEL);

    // ensure all accel and gyro axes are enabled
    module_icm20649_transfer(ARTEMIS_ICM20649_REG_PWR_MGMT_2, 0);

    // select bank 2
    module_icm20649_transfer(ARTEMIS_ICM20649_REG_BANK_SELECT, ARTEMIS_ICM20649_BANK_SELECT_2);

    // accel sample rate divider; 225Hz = 1125 / (1 + 4)
    module_icm20649_transfer(ARTEMIS_ICM20649_REG_ACCEL_SMPLRT_DIV2, ARTEMIS_ICM20649_ACCEL_INTERNAL_SMPLRT / 5);

    // accel configuration
    module_icm20649_transfer(ARTEMIS_ICM20649_REG_ACCEL_CONFIG, ARTEMIS_ICM20649_BANK2_ACCEL_FS_SEL_16 | ARTEMIS_ICM20649_BANK2_ACCEL_FCHOICE);

    // gyro sample rate divider; 220Hz = 1100 / (1 + 4)
    module_icm20649_transfer(ARTEMIS_ICM20649_REG_GYRO_SMPLRT_DIV, ARTEMIS_ICM20649_GYRO_INTERNAL_SMPLRT / 5);

    // gyro configuration
    module_icm20649_transfer(ARTEMIS_ICM20649_REG_GYRO_CONFIG, ARTEMIS_ICM20649_BANK2_GYRO_FS_SEL_2000 | ARTEMIS_ICM20649_BANK2_GYRO_FCHOICE);

    // select bank 0
    module_icm20649_transfer(ARTEMIS_ICM20649_REG_BANK_SELECT, ARTEMIS_ICM20649_BANK_SELECT_0);
}

//
//
//
static void module_icm20649_transfer(uint8_t reg, uint8_t data)
{
    artemis_stream_t txstream = {0};
    artemis_stream_t rxstream = {0};

    artemis_stream_setbuffer(&txstream, module.txbuffer, ARTEMIS_ICM20649_BUFFER_LENGTH);
    artemis_stream_setbuffer(&rxstream, module.rxbuffer, ARTEMIS_ICM20649_BUFFER_LENGTH);
    artemis_stream_put(&txstream, reg);
    artemis_spi_transfer(&module.spi, false, &txstream, &rxstream);

    artemis_stream_reset(&txstream);
    artemis_stream_reset(&rxstream);
    artemis_stream_put(&txstream, data);
    artemis_spi_transfer(&module.spi, true, &txstream, &rxstream);
}

///
///
///
static void module_icm20649_readsensor(uint8_t reg, module_data_t *data)
{
    uint8_t hi = 0;
    uint8_t lo = 0;
    artemis_stream_t txstream = {0};
    artemis_stream_t rxstream = {0};

    artemis_stream_setbuffer(&txstream, module.txbuffer, ARTEMIS_ICM20649_BUFFER_LENGTH);
    artemis_stream_setbuffer(&rxstream, module.rxbuffer, ARTEMIS_ICM20649_BUFFER_LENGTH);
    artemis_stream_put(&txstream, reg | ARTEMIS_SPI_READBIT_MSB);
    artemis_spi_transfer(&module.spi, false, &txstream, &rxstream);

    artemis_stream_reset(&rxstream);
    artemis_spi_receive(&module.spi, true, &rxstream, sizeof(module_data_t));

    for (size_t i = 0; i < ARTEMIS_IMU_AXIS_COUNT; i++) {
        artemis_stream_get(&rxstream, &hi);
        artemis_stream_get(&rxstream, &lo);
        (*data)[i] = (int16_t)((hi << 8) | lo);
    }
}
