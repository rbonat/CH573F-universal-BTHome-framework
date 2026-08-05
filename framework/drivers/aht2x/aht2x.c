#include "CONFIG.h"
#include "aht2x.h"
#include "measurement_precision.h"

#define AHT2X_CMD_STATUS      0x71U
#define AHT2X_CMD_INIT        0xBEU
#define AHT2X_CMD_MEASURE     0xACU
#define AHT2X_INIT_ARG0       0x08U
#define AHT2X_INIT_ARG1       0x00U
#define AHT2X_MEASURE_ARG0    0x33U
#define AHT2X_MEASURE_ARG1    0x00U
#define AHT2X_RAW_ROUNDING    32768UL

static SoftI2cBus_t aht2x_i2c;

static const SoftI2cConfig_t aht2x_default_i2c_config = {
    { AHT2X_I2C_SDA_PORT, AHT2X_I2C_SDA_PIN },
    { AHT2X_I2C_SCL_PORT, AHT2X_I2C_SCL_PIN },
    AHT2X_I2C_SPEED_HZ,
};

static uint16_t aht2x_round_unsigned_x100(uint16_t value, uint8_t decimal_places)
{
    if(decimal_places == 0U)
    {
        return (uint16_t)((((uint32_t)value + 50U) / 100U) * 100U);
    }

    if(decimal_places == 1U)
    {
        return (uint16_t)((((uint32_t)value + 5U) / 10U) * 10U);
    }

    return value;
}

static int16_t aht2x_round_signed_x100(int16_t value, uint8_t decimal_places)
{
    int16_t divisor;
    int16_t half_divisor;

    if(decimal_places >= 2U)
    {
        return value;
    }

    divisor = decimal_places == 0U ? 100 : 10;
    half_divisor = decimal_places == 0U ? 50 : 5;

    if(value >= 0)
    {
        return (int16_t)(((value + half_divisor) / divisor) * divisor);
    }

    return (int16_t)(((value - half_divisor) / divisor) * divisor);
}

static bool aht2x_write_command(uint8_t cmd, uint8_t arg0, uint8_t arg1)
{
    uint8_t buf[3];

    buf[0] = cmd;
    buf[1] = arg0;
    buf[2] = arg1;

    return SoftI2c_Write(&aht2x_i2c, AHT2X_I2C_ADDR, buf, sizeof(buf)) == SOFT_I2C_OK;
}

static void aht2x_convert_raw(const uint8_t *buf, aht2x_data_t *data)
{
    uint32_t humidity_raw;
    uint32_t temperature_raw;
    uint32_t humidity_x100;
    int32_t temperature_x100;

    humidity_raw = ((uint32_t)buf[1] << 12) |
                   ((uint32_t)buf[2] << 4) |
                   ((uint32_t)buf[3] >> 4);

    temperature_raw = (((uint32_t)buf[3] & 0x0FU) << 16) |
                      ((uint32_t)buf[4] << 8) |
                      (uint32_t)buf[5];

    humidity_x100 = ((humidity_raw * 625UL) + AHT2X_RAW_ROUNDING) >> 16;
    temperature_x100 = (int32_t)(((temperature_raw * 1250UL) + AHT2X_RAW_ROUNDING) >> 16) - 5000L;

    if(humidity_x100 > 10000U)
    {
        humidity_x100 = 10000U;
    }

    data->humidity_x100 = aht2x_round_unsigned_x100((uint16_t)humidity_x100,
                                                     AHT_HUMIDITY_DECIMAL_PLACES);
    data->temperature_x100 = aht2x_round_signed_x100((int16_t)temperature_x100,
                                                      AHT_TEMPERATURE_DECIMAL_PLACES);
}

bool aht2x_read_status(uint8_t *status)
{
    uint8_t cmd = AHT2X_CMD_STATUS;

    if(status == 0)
    {
        return false;
    }

    return SoftI2c_WriteRead(&aht2x_i2c, AHT2X_I2C_ADDR, &cmd, 1U, status, 1U) == SOFT_I2C_OK;
}

bool aht2x_init_with_config(const SoftI2cConfig_t *i2c_config)
{
    uint8_t status;

    if(i2c_config == 0)
    {
        return false;
    }

    SoftI2c_Init(&aht2x_i2c, i2c_config);

    mDelaymS(AHT2X_POWER_ON_DELAY_MS);

    if(!aht2x_read_status(&status))
    {
        return false;
    }

    if((status & AHT2X_STATUS_CALIBRATED) == 0U)
    {
        if(!aht2x_write_command(AHT2X_CMD_INIT, AHT2X_INIT_ARG0, AHT2X_INIT_ARG1))
        {
            return false;
        }
        mDelaymS(AHT2X_INIT_DELAY_MS);
    }

    return true;
}

bool aht2x_init(void)
{
    return aht2x_init_with_config(&aht2x_default_i2c_config);
}

bool aht2x_read(aht2x_data_t *data)
{
    uint8_t buf[7];

    if(data == 0)
    {
        return false;
    }

    if(!aht2x_write_command(AHT2X_CMD_MEASURE, AHT2X_MEASURE_ARG0, AHT2X_MEASURE_ARG1))
    {
        return false;
    }

    mDelaymS(AHT2X_MEASURE_DELAY_MS);

    if(SoftI2c_Read(&aht2x_i2c, AHT2X_I2C_ADDR, buf, sizeof(buf)) != SOFT_I2C_OK)
    {
        return false;
    }

    if((buf[0] & AHT2X_STATUS_BUSY) != 0U)
    {
        return false;
    }

    aht2x_convert_raw(buf, data);
    return true;
}
