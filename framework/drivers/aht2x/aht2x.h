#ifndef AHT2X_H
#define AHT2X_H

#include <stdbool.h>
#include <stdint.h>

#include "CONFIG.h"
#include "soft_i2c.h"

#ifdef __cplusplus
extern "C" {
#endif

#ifndef AHT2X_I2C_ADDR
#define AHT2X_I2C_ADDR 0x38U
#endif

#ifndef AHT2X_I2C_SDA_PORT
#define AHT2X_I2C_SDA_PORT SOFT_I2C_PORT_A
#endif

#ifndef AHT2X_I2C_SDA_PIN
#define AHT2X_I2C_SDA_PIN GPIO_Pin_12
#endif

#ifndef AHT2X_I2C_SCL_PORT
#define AHT2X_I2C_SCL_PORT SOFT_I2C_PORT_A
#endif

#ifndef AHT2X_I2C_SCL_PIN
#define AHT2X_I2C_SCL_PIN GPIO_Pin_13
#endif

#ifndef AHT2X_I2C_SPEED_HZ
#define AHT2X_I2C_SPEED_HZ SOFT_I2C_DEFAULT_SPEED_HZ
#endif

#ifndef AHT2X_POWER_ON_DELAY_MS
#define AHT2X_POWER_ON_DELAY_MS 100U
#endif

#ifndef AHT2X_INIT_DELAY_MS
#define AHT2X_INIT_DELAY_MS 10U
#endif

#ifndef AHT2X_MEASURE_DELAY_MS
#define AHT2X_MEASURE_DELAY_MS 80U
#endif

#define AHT2X_STATUS_BUSY       0x80U
#define AHT2X_STATUS_CALIBRATED 0x08U

typedef struct
{
    int16_t temperature_x100;
    uint16_t humidity_x100;
} aht2x_data_t;

bool aht2x_init(void);
bool aht2x_init_with_config(const SoftI2cConfig_t *i2c_config);
bool aht2x_read(aht2x_data_t *data);
bool aht2x_read_status(uint8_t *status);

#ifdef __cplusplus
}
#endif

#endif
