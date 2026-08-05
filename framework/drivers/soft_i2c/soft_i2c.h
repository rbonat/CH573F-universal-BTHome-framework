#ifndef SOFT_I2C_H
#define SOFT_I2C_H

#include <stdbool.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#ifndef SOFT_I2C_DEFAULT_SPEED_HZ
#define SOFT_I2C_DEFAULT_SPEED_HZ 100000UL
#endif

#ifndef SOFT_I2C_CLOCK_STRETCH_TIMEOUT_US
#define SOFT_I2C_CLOCK_STRETCH_TIMEOUT_US 1000U
#endif

typedef enum
{
    SOFT_I2C_PORT_A = 0,
    SOFT_I2C_PORT_B = 1,
} SoftI2cPort_t;

typedef struct
{
    SoftI2cPort_t port;
    uint32_t pin;
} SoftI2cPin_t;

typedef struct
{
    SoftI2cPin_t sda;
    SoftI2cPin_t scl;
    uint32_t speed_hz;
} SoftI2cConfig_t;

typedef struct
{
    SoftI2cConfig_t cfg;
    uint16_t half_period_us;
    bool initialized;
} SoftI2cBus_t;

typedef enum
{
    SOFT_I2C_OK = 0,
    SOFT_I2C_ERR_NACK,
    SOFT_I2C_ERR_TIMEOUT,
    SOFT_I2C_ERR_BUS,
    SOFT_I2C_ERR_PARAM,
} SoftI2cStatus_t;

void SoftI2c_Init(SoftI2cBus_t *bus, const SoftI2cConfig_t *cfg);
void SoftI2c_Deinit(SoftI2cBus_t *bus);

SoftI2cStatus_t SoftI2c_Write(SoftI2cBus_t *bus, uint8_t addr_7bit, const uint8_t *data, uint16_t len);
SoftI2cStatus_t SoftI2c_Read(SoftI2cBus_t *bus, uint8_t addr_7bit, uint8_t *data, uint16_t len);
SoftI2cStatus_t SoftI2c_WriteRead(SoftI2cBus_t *bus,
                                  uint8_t addr_7bit,
                                  const uint8_t *write_data,
                                  uint16_t write_len,
                                  uint8_t *read_data,
                                  uint16_t read_len);

#ifdef __cplusplus
}
#endif

#endif
