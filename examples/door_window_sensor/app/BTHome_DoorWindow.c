#include "CONFIG.h"
#include "HAL.h"
#include "BTHome_DoorWindow.h"
#include "battery.h"
#include "bthome_v2.h"

static uint8_t BTHome_DoorWindow_TaskID;
static uint8_t BTHome_PacketID;
static bool DoorWindow_Open;
static bool DoorWindow_Advertising;
static bool DoorWindow_SleepAllowed;
static bool DoorWindow_DebouncePending;
static volatile bool DoorWindow_GpioWakePending;
static uint32_t DoorWindow_NextStatusWake;

static bool DoorWindow_ReadOpen(void)
{
    return GPIOA_ReadPortPin(SENSOR_PIN) ? SENSOR_OPEN_LEVEL : !SENSOR_OPEN_LEVEL;
}

/* The CH573 GPIO controller selects one edge at a time. Rearm the opposite
 * edge after every interrupt, so both opening and closing wake the device. */
static void DoorWindow_ArmSensorInterrupt(void)
{
    GPIOA_ITModeCfg(SENSOR_PIN,
                    GPIOA_ReadPortPin(SENSOR_PIN) ? GPIO_ITMode_FallEdge : GPIO_ITMode_RiseEdge);
}

__attribute__((interrupt("WCH-Interrupt-fast")))
__attribute__((section(".highcode")))
void GPIOA_IRQHandler(void)
{
    if(GPIOA_ReadITFlagBit(SENSOR_PIN))
    {
        GPIOA_ClearITFlagBit(SENSOR_PIN);
        DoorWindow_ArmSensorInterrupt();
        DoorWindow_GpioWakePending = true;
    }
}

static void DoorWindow_SetAdvertising(bool enabled)
{
    uint8_t value = enabled ? TRUE : FALSE;

    GAPRole_SetParameter(GAPROLE_ADVERT_ENABLED, sizeof(value), &value);
    DoorWindow_Advertising = enabled;
}

static void DoorWindow_SetAdvertisementInterval(uint16_t interval)
{
    GAP_SetParamValue(TGAP_DISC_ADV_INT_MIN, interval);
    GAP_SetParamValue(TGAP_DISC_ADV_INT_MAX, interval);
}

static void DoorWindow_StartAdvertisement(void)
{
    uint16_t vbat_raw = BatterySensor_GetRaw();
    uint16_t vbat = BatterySensor_GetMilliVolts(vbat_raw);
    uint8_t vbat_pct = BatterySensor_PercentFromMilliVolts(vbat);
    bool battery_low = BatterySensor_IsLow(vbat);
    bthome_v2_builder_t bthome;
    uint8_t service_data[20];
    uint8_t adv[31];
    uint8_t len;

    bthome_v2_init(&bthome, service_data, sizeof(service_data), false, true);
    bthome_v2_add_packet_id(&bthome, BTHome_PacketID++);
    bthome_v2_add_object_uint(&bthome, BTHOME_ID_BATTERY_PERCENT, vbat_pct);
    bthome_v2_add_object_uint(&bthome, BTHOME_ID_VOLTAGE_0_001, vbat);
    bthome_v2_add_boolean(&bthome, BTHOME_ID_OPENING_BOOLEAN, DoorWindow_Open);
    bthome_v2_add_boolean(&bthome, BTHOME_ID_BATTERY_LOW_BOOLEAN, battery_low);

    len = bthome_v2_build_legacy_advertisement(adv, sizeof(adv),
                                                bthome_v2_data(&bthome), bthome_v2_size(&bthome),
                                                BTHOME_NAME, true);
    GAPRole_SetParameter(GAPROLE_ADVERT_DATA, len, adv);

    tmos_stop_task(BTHome_DoorWindow_TaskID, DOOR_ADV_SLOW_EVT);
    tmos_stop_task(BTHome_DoorWindow_TaskID, DOOR_ADV_STOP_EVT);
    DoorWindow_SetAdvertisementInterval(ADV_FAST_INTERVAL);
    DoorWindow_SetAdvertising(true);
    DoorWindow_SleepAllowed = false;
    tmos_start_task(BTHome_DoorWindow_TaskID, DOOR_ADV_SLOW_EVT, ADV_FAST_PHASE_TMOS);
}

void BTHome_DoorWindow_Init(void)
{
    uint8_t adv_type = GAP_ADTYPE_ADV_NONCONN_IND;

    BTHome_DoorWindow_TaskID = TMOS_ProcessEventRegister(BTHome_DoorWindow_ProcessEvent);
    GAPRole_SetParameter(GAPROLE_ADV_EVENT_TYPE, sizeof(adv_type), &adv_type);
    DoorWindow_SetAdvertising(false);

    GPIOA_ModeCfg(SENSOR_PIN, GPIO_ModeIN_Floating);
    DoorWindow_Open = DoorWindow_ReadOpen();
    DoorWindow_ArmSensorInterrupt();
    PWR_PeriphWakeUpCfg(ENABLE, RB_SLP_RTC_WAKE | RB_SLP_GPIO_WAKE, Edge_LongDelay);
    PFIC_EnableIRQ(GPIO_A_IRQn);

    tmos_set_event(BTHome_DoorWindow_TaskID, SBP_START_DEVICE_EVT);
}

uint16_t BTHome_DoorWindow_ProcessEvent(uint8_t task_id, uint16_t events)
{
    if(events & SBP_START_DEVICE_EVT)
    {
        GAPRole_PeripheralStartDevice(BTHome_DoorWindow_TaskID, NULL, NULL);
        /* Power-up advertises one status burst, then only changes/heartbeats. */
        DoorWindow_StartAdvertisement();
        return events ^ SBP_START_DEVICE_EVT;
    }

    if(events & DOOR_DEBOUNCE_EVT)
    {
        bool open = DoorWindow_ReadOpen();

        DoorWindow_DebouncePending = false;
        if(open != DoorWindow_Open)
        {
            DoorWindow_Open = open;
            DoorWindow_StartAdvertisement();
        }
        else if(!DoorWindow_Advertising)
        {
            DoorWindow_SleepAllowed = true;
        }
        return events ^ DOOR_DEBOUNCE_EVT;
    }

    if(events & DOOR_ADV_SLOW_EVT)
    {
        DoorWindow_SetAdvertisementInterval(ADV_SLOW_INTERVAL);
        tmos_start_task(BTHome_DoorWindow_TaskID, DOOR_ADV_STOP_EVT, ADV_SLOW_PHASE_TMOS);
        return events ^ DOOR_ADV_SLOW_EVT;
    }

    if(events & DOOR_ADV_STOP_EVT)
    {
        DoorWindow_SetAdvertising(false);
        DoorWindow_NextStatusWake = RTC_GetCycle32k() + DOOR_STATUS_PERIOD_RTC_TICKS;
        DoorWindow_SleepAllowed = true;
        return events ^ DOOR_ADV_STOP_EVT;
    }

    if(events & DOOR_STATUS_EVT)
    {
        DoorWindow_Open = DoorWindow_ReadOpen();
        DoorWindow_StartAdvertisement();
        return events ^ DOOR_STATUS_EVT;
    }

    if(events & SYS_EVENT_MSG)
    {
        uint8_t *message = tmos_msg_receive(BTHome_DoorWindow_TaskID);
        if(message != NULL)
        {
            tmos_msg_deallocate(message);
        }
        return events ^ SYS_EVENT_MSG;
    }

    return 0;
}

void BTHome_DoorWindow_Poll(void)
{
    if(DoorWindow_GpioWakePending)
    {
        DoorWindow_GpioWakePending = false;
        DoorWindow_SleepAllowed = false;
        DoorWindow_DebouncePending = true;
        tmos_start_task(BTHome_DoorWindow_TaskID, DOOR_DEBOUNCE_EVT, DOOR_DEBOUNCE_TMOS);
    }
}

void BTHome_DoorWindow_SleepIfIdle(void)
{
    if(DoorWindow_SleepAllowed && !DoorWindow_Advertising && !DoorWindow_DebouncePending)
    {
        DoorWindow_SleepAllowed = false;
        CH57X_LowPower(DoorWindow_NextStatusWake);

        /* With advertising disabled, GPIO and RTC are the only enabled wake
         * sources. GPIO ISR sets the flag; otherwise this was the 5 min
         * heartbeat deadline. */
        if(!DoorWindow_GpioWakePending)
        {
            tmos_set_event(BTHome_DoorWindow_TaskID, DOOR_STATUS_EVT);
        }
    }
}
