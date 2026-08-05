/********************************** (C) COPYRIGHT *******************************
 * File Name          : BTHome_temp_hygro.h
 * Author             : WCH
 * Version            : V1.0
 * Date               : 2018/12/11
 * Description        :
 * Copyright (c) 2021 Nanjing Qinheng Microelectronics Co., Ltd.
 * SPDX-License-Identifier: Apache-2.0
 *******************************************************************************/

#ifndef BTHOME_TEMP_HYGRO_H
#define BTHOME_TEMP_HYGRO_H

#ifdef __cplusplus
extern "C" {
#endif

/*********************************************************************
 * INCLUDES
 */

/*********************************************************************
 * CONSTANTS
 */

// BTHome temp_hygro Task Events
#define SBP_START_DEVICE_EVT    0x0001
#define SBP_PERIODIC_EVT        0x0002
#define SBP_READ_RSSI_EVT       0x0004
#define SBP_PARAM_UPDATE_EVT    0x0008

/*********************************************************************
 * MACROS
 */
typedef struct
{
    uint16_t connHandle; // Connection handle of current connection
    uint16_t connInterval;
    uint16_t connSlaveLatency;
    uint16_t connTimeout;
} BTHome_temp_hygroConnItem_t;

/*********************************************************************
 * FUNCTIONS
 */

/*
 * Task Initialization for the BLE Application
 */
extern void BTHome_temp_hygro_Init(void);

/*
 * Task Event Processor for the BLE Application
 */
extern uint16_t BTHome_temp_hygro_ProcessEvent(uint8_t task_id, uint16_t events);

/*********************************************************************
*********************************************************************/

#ifdef __cplusplus
}
#endif

#endif
