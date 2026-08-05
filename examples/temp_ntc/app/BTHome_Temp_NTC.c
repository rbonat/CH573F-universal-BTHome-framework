#include "CONFIG.h"
#include "BTHome_Temp_NTC.h"
#include "battery.h"
#include "bthome_v2.h"
#include "ntc_temp.h"

static uint8_t BTHome_Temp_NTC_TaskID;
static uint8_t BTHome_PacketID;

void BTHome_Temp_NTC_Init(void)
{
    uint8_t adv_enable = TRUE;
    uint8_t adv_type = GAP_ADTYPE_ADV_NONCONN_IND;
    uint16_t adv_interval = ADV_INTERVAL;

    BTHome_Temp_NTC_TaskID = TMOS_ProcessEventRegister(BTHome_Temp_NTC_ProcessEvent);
    GAPRole_SetParameter(GAPROLE_ADVERT_ENABLED, sizeof(adv_enable), &adv_enable);
    GAPRole_SetParameter(GAPROLE_ADV_EVENT_TYPE, sizeof(adv_type), &adv_type);
    GAP_SetParamValue(TGAP_DISC_ADV_INT_MIN, adv_interval);
    GAP_SetParamValue(TGAP_DISC_ADV_INT_MAX, adv_interval);
    tmos_set_event(BTHome_Temp_NTC_TaskID, SBP_START_DEVICE_EVT);
}

uint16_t BTHome_Temp_NTC_ProcessEvent(uint8_t task_id, uint16_t events)
{
    if(events & SBP_START_DEVICE_EVT)
    {
        GAPRole_PeripheralStartDevice(BTHome_Temp_NTC_TaskID, NULL, NULL);
        tmos_start_task(BTHome_Temp_NTC_TaskID, TEMP_NTC_EVT, 16);
        return events ^ SBP_START_DEVICE_EVT;
    }

    if(events & TEMP_NTC_EVT)
    {
        uint16_t vbat_raw = BatterySensor_GetRaw();
        uint16_t ntc_raw = NtcTempSensor_GetRaw();
        uint16_t vbat;
        int16_t temperature_x10;
        uint8_t vbat_pct;
        bool battery_low;
        bthome_v2_builder_t bthome;
        uint8_t service_data[24];
        uint8_t adv[31];
        uint8_t len;

#if CALIBRATION_MODE
        vbat = vbat_raw;
        temperature_x10 = (int16_t)ntc_raw;
#else
        vbat = BatterySensor_GetMilliVolts(vbat_raw);
        temperature_x10 = NtcTempSensor_GetTemperatureX10(ntc_raw);
#endif
        vbat_pct = BatterySensor_PercentFromMilliVolts(vbat);
        battery_low = BatterySensor_IsLow(vbat);

        bthome_v2_init(&bthome, service_data, sizeof(service_data), false, false);
        bthome_v2_add_packet_id(&bthome, BTHome_PacketID++);
        bthome_v2_add_object_uint(&bthome, BTHOME_ID_BATTERY_PERCENT, vbat_pct);
        bthome_v2_add_object_uint(&bthome, BTHOME_ID_VOLTAGE_0_001, vbat);
        bthome_v2_add_boolean(&bthome, BTHOME_ID_BATTERY_LOW_BOOLEAN, battery_low);
        bthome_v2_add_object_sint(&bthome, BTHOME_ID_TEMPERATURE_0_1, temperature_x10);

        len = bthome_v2_build_legacy_advertisement(adv, sizeof(adv),
                                                    bthome_v2_data(&bthome), bthome_v2_size(&bthome),
                                                    BTHOME_NAME, true);
        GAPRole_SetParameter(GAPROLE_ADVERT_DATA, len, adv);
        tmos_start_task(BTHome_Temp_NTC_TaskID, TEMP_NTC_EVT, TEMP_NTC_PERIOD_MS);
        return events ^ TEMP_NTC_EVT;
    }

    if(events & SYS_EVENT_MSG)
    {
        uint8_t *message = tmos_msg_receive(BTHome_Temp_NTC_TaskID);
        if(message != NULL)
        {
            tmos_msg_deallocate(message);
        }
        return events ^ SYS_EVENT_MSG;
    }

    return 0;
}
