#include "CONFIG.h"
#include "BTHome_temp_hygro.h"
#include "battery.h"
#include "bthome_v2.h"
#include "aht2x.h"

static uint8_t BTHome_temp_hygro_TaskID;
static uint8_t BTHome_PacketID;

void BTHome_temp_hygro_Init(void)
{
    BTHome_temp_hygro_TaskID = TMOS_ProcessEventRegister(BTHome_temp_hygro_ProcessEvent);

    uint8_t adv_enable = TRUE;
    uint8_t adv_type = GAP_ADTYPE_ADV_NONCONN_IND;
    GAPRole_SetParameter(GAPROLE_ADVERT_ENABLED, sizeof(uint8_t), &adv_enable);
    GAPRole_SetParameter(GAPROLE_ADV_EVENT_TYPE, sizeof(uint8_t), &adv_type);

    uint16_t advInt = ADV_INTERVAL;
    GAP_SetParamValue(TGAP_DISC_ADV_INT_MIN, advInt);
    GAP_SetParamValue(TGAP_DISC_ADV_INT_MAX, advInt);

    tmos_set_event(BTHome_temp_hygro_TaskID, SBP_START_DEVICE_EVT);
}



uint16_t BTHome_temp_hygro_ProcessEvent(uint8_t task_id, uint16_t events)
{
    if(events & SBP_START_DEVICE_EVT)
    {
        GAPRole_PeripheralStartDevice(BTHome_temp_hygro_TaskID, NULL, NULL);
        aht2x_init();

        /* pierwszy pomiar po 2 s */
        tmos_start_task(BTHome_temp_hygro_TaskID, TEMP_HYGRO_EVT, 16);

        return (events ^ SBP_START_DEVICE_EVT);
    }

    if(events & TEMP_HYGRO_EVT)
    {
        uint16_t vbat_raw = BatterySensor_GetRaw();
        uint16_t vbat;
        uint8_t vbat_pct;
        bool battery_low;
        aht2x_data_t aht;
        bool aht_ok;
        bthome_v2_builder_t bthome;
        uint8_t service_data[24];
        uint8_t adv[31];
        uint8_t len;

        aht_ok = false;

#if CALIBRATION_MODE
        vbat = vbat_raw;
#else
        {
            vbat = BatterySensor_GetMilliVolts(vbat_raw);
            aht_ok = aht2x_read(&aht);
        }
#endif
        vbat_pct = BatterySensor_PercentFromMilliVolts(vbat);
        battery_low = BatterySensor_IsLow(vbat);

        bthome_v2_init(&bthome, service_data, sizeof(service_data), false, false);
        bthome_v2_add_packet_id(&bthome, BTHome_PacketID++);
        bthome_v2_add_object_uint(&bthome, BTHOME_ID_BATTERY_PERCENT, vbat_pct);
        if(aht_ok)
        {
            bthome_v2_add_object_sint(
                &bthome,
                BTHOME_ID_TEMPERATURE_0_01,
                aht.temperature_x100);

            bthome_v2_add_object_uint(
                &bthome,
                BTHOME_ID_HUMIDITY_0_01,
                aht.humidity_x100);
        }
        bthome_v2_add_object_uint(&bthome, BTHOME_ID_VOLTAGE_0_001, vbat);
        bthome_v2_add_boolean(&bthome, BTHOME_ID_BATTERY_LOW_BOOLEAN, battery_low);


        len = bthome_v2_build_legacy_advertisement(adv,
                                                   sizeof(adv),
                                                   bthome_v2_data(&bthome),
                                                   bthome_v2_size(&bthome),
                                                   BTHOME_NAME,
                                                   true);

        GAPRole_SetParameter(GAPROLE_ADVERT_DATA, len, adv);
        tmos_start_task(BTHome_temp_hygro_TaskID, TEMP_HYGRO_EVT, TEMP_HYGRO_PERIOD_MS);

        return (events ^ TEMP_HYGRO_EVT);
    }

    if(events & SYS_EVENT_MSG)
    {
        uint8_t *pMsg;
        if((pMsg = tmos_msg_receive(BTHome_temp_hygro_TaskID)) != NULL)
        {
            tmos_msg_deallocate(pMsg);
        }
        return (events ^ SYS_EVENT_MSG);
    }

    return 0;
}
