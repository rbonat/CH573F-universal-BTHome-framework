#ifndef BATTERY_H
#define BATTERY_H

#include <stdbool.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

uint16_t BatterySensor_GetRaw(void);
uint16_t BatterySensor_GetMilliVolts(uint16_t raw_adc);
uint8_t BatterySensor_PercentFromMilliVolts(uint16_t mv);
bool BatterySensor_IsLow(uint16_t vbat_mv);

#ifdef __cplusplus
}
#endif

#endif
