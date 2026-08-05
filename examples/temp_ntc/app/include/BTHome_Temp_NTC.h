#ifndef BTHOME_TEMP_NTC_H
#define BTHOME_TEMP_NTC_H

#include <stdint.h>

#define SBP_START_DEVICE_EVT 0x0001

void BTHome_Temp_NTC_Init(void);
uint16_t BTHome_Temp_NTC_ProcessEvent(uint8_t task_id, uint16_t events);

#endif
