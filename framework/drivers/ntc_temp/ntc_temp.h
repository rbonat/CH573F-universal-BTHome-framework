#ifndef NTC_TEMP_H
#define NTC_TEMP_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

uint16_t NtcTempSensor_GetRaw(void);
int16_t NtcTempSensor_GetTemperatureX10(uint16_t raw);

#ifdef __cplusplus
}
#endif

#endif
