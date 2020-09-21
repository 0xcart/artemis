///
/// @file artemis_icm20649.c
///

#include "artemis_debug.h"
#include "artemis_icm20649.h"
#include "artemis_spi.h"
#include "artemis_time.h"

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
#define ARTEMIS_ICM20649_REG_ACCEL_SMPLRT_DIV   (0x10) // accel sample rate divider; 1125Hz / (1 + SAMPLE_RATE_DIVIDER[11:0])
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
#define ARTEMIS_ICM20649_BANK2_GYRO_FS_SEL_500  (0x00) // gyro full scale select: ±500 dps
#define ARTEMIS_ICM20649_BANK2_GYRO_FS_SEL_1000 (0x02) // gyro full scale select: ±1000 dps
#define ARTEMIS_ICM20649_BANK2_GYRO_FS_SEL_2000 (0x04) // gyro full scale select: ±2000 dps
#define ARTEMIS_ICM20649_BANK2_GYRO_FS_SEL_4000 (0x06) // gyro full scale select: ±4000 dps
#define ARTEMIS_ICM20649_BANK2_GYRO_FCHOICE     (0x01) // enable gyro DLPF

// ICM20649 attributes
#define ARTEMIS_ICM20649_IDENTITY               (0xE1) // ICM20649 identity
#define ARTEMIS_ICM20649_SCLK_FREQ              (7000000) // SPI SCLK frequency
#define ARTEMIS_ICM20649_ACCEL_INTERNAL_SMPLRT  (1125) // accel internal sample rate
#define ARTEMIS_ICM20649_GYRO_INTERNAL_SMPLRT   (1100) // gyro internal sample rate

// application specific
#define ARTEMIS_ICM20649_SPIBUFFER_LENGTH       (8) // transmit and receive buffer length

typedef uint8_t buffer_t[ARTEMIS_ICM20649_SPIBUFFER_LENGTH];

typedef struct s_module_t
{
    artemis_spi_t spi;
    buffer_t txbuffer;
    buffer_t rxbuffer;
} module_t;

static module_t module;

static void module_icm20649_bank(uint8_t bank);
static void module_icm20649_reset(void);
static void module_icm20649_identity(void);
static void module_icm20649_configure(void);

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

    am_hal_gpio_pinconfig(AM_BSP_GPIO_IOM0_SCK, g_AM_BSP_GPIO_IOM0_SCK);
    am_hal_gpio_pinconfig(AM_BSP_GPIO_IOM0_MISO, g_AM_BSP_GPIO_IOM0_MISO);
    am_hal_gpio_pinconfig(AM_BSP_GPIO_IOM0_MOSI, g_AM_BSP_GPIO_IOM0_MOSI);
    am_hal_gpio_pinconfig(AM_BSP_GPIO_IOM0_CS, g_AM_BSP_GPIO_IOM0_CS);

    module_icm20649_reset();
    module_icm20649_identity();
    module_icm20649_configure();
}

///
///
///
static void module_icm20649_bank(uint8_t bank)
{
    artemis_stream_t txstream = {0};

    artemis_stream_setbuffer(&txstream, module.txbuffer, ARTEMIS_ICM20649_SPIBUFFER_LENGTH);
    artemis_stream_put(&txstream, ARTEMIS_ICM20649_REG_BANK_SELECT);
    artemis_stream_put(&txstream, bank);
    artemis_spi_send(&module.spi, &txstream);
}

///
///
///
static void module_icm20649_reset(void)
{
    artemis_stream_t txstream = {0};

    module_icm20649_bank(ARTEMIS_ICM20649_BANK_SELECT_0);

    artemis_stream_setbuffer(&txstream, module.txbuffer, ARTEMIS_ICM20649_SPIBUFFER_LENGTH);
    artemis_stream_put(&txstream, ARTEMIS_ICM20649_REG_PWR_MGMT_1);
    artemis_stream_put(&txstream, ARTEMIS_ICM20649_BANK0_DEVICE_RESET);
    artemis_spi_send(&module.spi, &txstream);

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

    module_icm20649_bank(ARTEMIS_ICM20649_BANK_SELECT_0);

    artemis_stream_setbuffer(&txstream, module.txbuffer, ARTEMIS_ICM20649_SPIBUFFER_LENGTH);
    artemis_stream_setbuffer(&rxstream, module.rxbuffer, ARTEMIS_ICM20649_SPIBUFFER_LENGTH);
    artemis_stream_put(&txstream, ARTEMIS_SPI_READBIT_MSB | ARTEMIS_ICM20649_REG_WHO_AM_I);
    artemis_spi_transfer(&module.spi, &txstream, &rxstream);
    artemis_stream_get(&rxstream, &identity);

    ARTEMIS_DEBUG_PRINTF("ICM20649 IDENTITY:\t0x%02X\n", identity);
}

///
///
///
static void module_icm20649_configure(void)
{
    artemis_stream_t txstream = {0};

    module_icm20649_bank(ARTEMIS_ICM20649_BANK_SELECT_0);

    // disable I2C interface
    artemis_stream_setbuffer(&txstream, module.txbuffer, ARTEMIS_ICM20649_SPIBUFFER_LENGTH);
    artemis_stream_put(&txstream, ARTEMIS_ICM20649_REG_USER_CTRL);
    artemis_stream_put(&txstream, ARTEMIS_ICM20649_BANK0_I2C_IF_DIS);
    artemis_spi_send(&module.spi, &txstream);

    // disable temperature sensor and auto select best available clock source
    artemis_stream_reset(&txstream);
    artemis_stream_put(&txstream, ARTEMIS_ICM20649_REG_PWR_MGMT_1);
    artemis_stream_put(&txstream, ARTEMIS_ICM20649_BANK0_TEMP_DIS | ARTEMIS_ICM20649_BANK0_CLKSEL);
    artemis_spi_send(&module.spi, &txstream);

    // ensure all accel and gyro axes are enabled
    artemis_stream_reset(&txstream);
    artemis_stream_put(&txstream, ARTEMIS_ICM20649_REG_PWR_MGMT_2);
    artemis_stream_put(&txstream, 0);
    artemis_spi_send(&module.spi, &txstream);

    module_icm20649_bank(ARTEMIS_ICM20649_BANK_SELECT_2);

    // accel sample rate divider
    artemis_stream_reset(&txstream);
    artemis_stream_put(&txstream, ARTEMIS_ICM20649_REG_ACCEL_SMPLRT_DIV);
    artemis_stream_put(&txstream, ARTEMIS_ICM20649_ACCEL_INTERNAL_SMPLRT / 5); // 1125 / (1 + 4) = 225Hz
    artemis_spi_send(&module.spi, &txstream);

    // accel configuration
    artemis_stream_reset(&txstream);
    artemis_stream_put(&txstream, ARTEMIS_ICM20649_REG_ACCEL_CONFIG);
    artemis_stream_put(&txstream, ARTEMIS_ICM20649_BANK2_ACCEL_FS_SEL_16 | ARTEMIS_ICM20649_BANK2_ACCEL_FCHOICE);
    artemis_spi_send(&module.spi, &txstream);

    // gyro sample rate divider
    artemis_stream_reset(&txstream);
    artemis_stream_put(&txstream, ARTEMIS_ICM20649_REG_GYRO_SMPLRT_DIV);
    artemis_stream_put(&txstream, ARTEMIS_ICM20649_GYRO_INTERNAL_SMPLRT / 5); // 1100 / (1 + 4) = 220Hz
    artemis_spi_send(&module.spi, &txstream);

    // gyro configuration
    artemis_stream_reset(&txstream);
    artemis_stream_put(&txstream, ARTEMIS_ICM20649_REG_GYRO_CONFIG);
    artemis_stream_put(&txstream, ARTEMIS_ICM20649_BANK2_GYRO_FS_SEL_2000 | ARTEMIS_ICM20649_BANK2_GYRO_FCHOICE);
    artemis_spi_send(&module.spi, &txstream);

    module_icm20649_bank(ARTEMIS_ICM20649_BANK_SELECT_0);
}
