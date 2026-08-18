#ifndef BTHOME_DOOR_WINDOW_H
#define BTHOME_DOOR_WINDOW_H

#include <stdint.h>

#define SBP_START_DEVICE_EVT 0x0001

void BTHome_DoorWindow_Init(void);
uint16_t BTHome_DoorWindow_ProcessEvent(uint8_t task_id, uint16_t events);
void BTHome_DoorWindow_Poll(void);
void BTHome_DoorWindow_SleepIfIdle(void);

#endif
