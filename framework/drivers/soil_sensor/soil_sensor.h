#ifndef SOIL_SENSOR_H
#define SOIL_SENSOR_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

uint16_t SoilSensor_GetSoilRaw(void);
uint16_t SoilSensor_FilterSoilRaw(uint16_t raw);
uint16_t SoilSensor_SoilPercentX100(uint16_t raw);
uint16_t SoilSensor_ApplyTemperatureCorrectionX100(uint16_t soil_x100, int16_t temp_x10);

#ifdef __cplusplus
}
#endif

#endif
