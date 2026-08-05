#include "CONFIG.h"
#include "BTHome_Soil.h"
#include "battery.h"
#include "bthome_v2.h"
#include "ntc_temp.h"
#include "soil_sensor.h"

static uint8_t BTHome_Soil_TaskID;
static uint8_t BTHome_PacketID;

void BTHome_Soil_Init(void)
{
    BTHome_Soil_TaskID = TMOS_ProcessEventRegister(BTHome_Soil_ProcessEvent);

    uint8_t adv_enable = TRUE;
    uint8_t adv_type = GAP_ADTYPE_ADV_NONCONN_IND;
    GAPRole_SetParameter(GAPROLE_ADVERT_ENABLED, sizeof(uint8_t), &adv_enable);
    GAPRole_SetParameter(GAPROLE_ADV_EVENT_TYPE, sizeof(uint8_t), &adv_type);

    uint16_t advInt = ADV_INTERVAL;
    GAP_SetParamValue(TGAP_DISC_ADV_INT_MIN, advInt);
    GAP_SetParamValue(TGAP_DISC_ADV_INT_MAX, advInt);

    tmos_set_event(BTHome_Soil_TaskID, SBP_START_DEVICE_EVT);
}

uint16_t BTHome_Soil_ProcessEvent(uint8_t task_id, uint16_t events)
{
    if(events & SBP_START_DEVICE_EVT)
    {
        GAPRole_PeripheralStartDevice(BTHome_Soil_TaskID, NULL, NULL);

        /* pierwszy pomiar po 2 s */
        tmos_start_task(BTHome_Soil_TaskID, SOIL_EVT, 16);

        return (events ^ SBP_START_DEVICE_EVT);
    }

    if(events & SOIL_EVT)
    {
        uint16_t vbat_raw = BatterySensor_GetRaw();
        uint16_t vbat;
        uint8_t vbat_pct;
        uint16_t ntc_raw = NtcTempSensor_GetRaw();
        int16_t ntc_temp_x10;
        uint16_t soil_raw = SoilSensor_GetSoilRaw();
        uint16_t moisture;
        bool battery_low;
        bthome_v2_builder_t bthome;
        uint8_t service_data[24];
        uint8_t adv[31];
        uint8_t len;

#if CALIBRATION_MODE
        vbat = vbat_raw;
        ntc_temp_x10 = (int16_t)ntc_raw;
        moisture = soil_raw;
#else
        {
            uint16_t soil_filtered_raw = SoilSensor_FilterSoilRaw(soil_raw);

            vbat = BatterySensor_GetMilliVolts(vbat_raw);
            ntc_temp_x10 = NtcTempSensor_GetTemperatureX10(ntc_raw);
            moisture = SoilSensor_ApplyTemperatureCorrectionX100(SoilSensor_SoilPercentX100(soil_filtered_raw), ntc_temp_x10);
        }
#endif

        vbat_pct = BatterySensor_PercentFromMilliVolts(vbat);
        battery_low = BatterySensor_IsLow(vbat);

        bthome_v2_init(&bthome, service_data, sizeof(service_data), false, false);
        bthome_v2_add_packet_id(&bthome, BTHome_PacketID++);
        bthome_v2_add_object_uint(&bthome, BTHOME_ID_BATTERY_PERCENT, vbat_pct);
        bthome_v2_add_object_uint(&bthome, BTHOME_ID_VOLTAGE_0_001, vbat);
        bthome_v2_add_object_uint(&bthome, BTHOME_ID_MOISTURE_0_01, moisture);
        bthome_v2_add_boolean(&bthome, BTHOME_ID_BATTERY_LOW_BOOLEAN, battery_low);
        bthome_v2_add_object_sint(&bthome, BTHOME_ID_TEMPERATURE_0_1, ntc_temp_x10);

        len = bthome_v2_build_legacy_advertisement(adv,
                                                   sizeof(adv),
                                                   bthome_v2_data(&bthome),
                                                   bthome_v2_size(&bthome),
                                                   NULL,
                                                   true);

        GAPRole_SetParameter(GAPROLE_ADVERT_DATA, len, adv);
        tmos_start_task(BTHome_Soil_TaskID, SOIL_EVT, SOIL_PERIOD_MS);

        return (events ^ SOIL_EVT);
    }

    if(events & SYS_EVENT_MSG)
    {
        uint8_t *pMsg;
        if((pMsg = tmos_msg_receive(BTHome_Soil_TaskID)) != NULL)
        {
            tmos_msg_deallocate(pMsg);
        }
        return (events ^ SYS_EVENT_MSG);
    }

    return 0;
}
