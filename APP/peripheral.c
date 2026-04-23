#include "CONFIG.h"
#include "peripheral.h"
#include "bthome_v2.h"

/* ===== konfiguracja ===== */

#define SOIL_PERIOD_MS       (30*1600)    //pomiar co 30 sek (units of 625us)
#define ADV_INTERVAL         (10*1600)    //adv co 10 sek (units of 625us)       
#define BTHOME_NAME          "BT_SOIL_1"
#define SOIL_MEASURE_PIN   GPIO_Pin_9
#define SOIL_REVERSE_PIN   GPIO_Pin_14

#define SOIL_EVT        0x4000

// --------SOIL 2
#define SOIL_RAW_DRY    3500
#define SOIL_RAW_WET    42000

// -----SOIL1
// #define SOIL_RAW_DRY    3500
// #define SOIL_RAW_WET    17000

#define SOIL_TIMEOUT_TICKS 30000
static uint8_t Peripheral_TaskID;
static uint8_t BTHome_PacketID;


/* ===== RC soil pomiar ===== */

static uint16_t SoilRC(void)
{
    uint32_t aver = 0;

    GPIOPinRemap(DISABLE, RB_PIN_TMR0);
    TMR0_ITCfg(DISABLE, TMR0_3_IT_CYC_END | TMR0_3_IT_DATA_ACT | TMR0_3_IT_FIFO_HF | TMR0_3_IT_FIFO_OV);

    for (int i = 0; i < 8; i++) {
        GPIOB_ModeCfg(SOIL_REVERSE_PIN, GPIO_ModeOut_PP_5mA);
        GPIOB_ResetBits(SOIL_REVERSE_PIN);
        GPIOA_ModeCfg(SOIL_MEASURE_PIN, GPIO_ModeOut_PP_5mA);
        GPIOA_SetBits(SOIL_MEASURE_PIN);
        mDelayuS(20);

        GPIOA_ModeCfg(SOIL_MEASURE_PIN, GPIO_ModeIN_Floating);
        TMR0_TimerInit(SOIL_TIMEOUT_TICKS + 2);
        TMR0_ClearITFlag(TMR0_3_IT_CYC_END | TMR0_3_IT_DATA_ACT | TMR0_3_IT_FIFO_HF | TMR0_3_IT_FIFO_OV);

        while(GPIOA_ReadPortPin(SOIL_MEASURE_PIN) && !TMR0_GetITFlag(TMR0_3_IT_CYC_END))
        {
        }

        uint32_t t = TMR0_GetCurrentTimer();
        TMR0_Disable();

        if(t > SOIL_TIMEOUT_TICKS)
        {
            t = SOIL_TIMEOUT_TICKS;
        }

        aver = aver + t;

        GPIOA_ModeCfg(SOIL_MEASURE_PIN, GPIO_ModeOut_PP_5mA);
        GPIOA_ResetBits(SOIL_MEASURE_PIN);
        GPIOB_ModeCfg(SOIL_REVERSE_PIN, GPIO_ModeOut_PP_5mA);
        GPIOB_SetBits(SOIL_REVERSE_PIN);
        mDelayuS(20);

        GPIOB_ModeCfg(SOIL_REVERSE_PIN, GPIO_ModeIN_Floating);
        TMR0_TimerInit(SOIL_TIMEOUT_TICKS + 2);
        TMR0_ClearITFlag(TMR0_3_IT_CYC_END | TMR0_3_IT_DATA_ACT | TMR0_3_IT_FIFO_HF | TMR0_3_IT_FIFO_OV);

        while(GPIOB_ReadPortPin(SOIL_REVERSE_PIN) && !TMR0_GetITFlag(TMR0_3_IT_CYC_END))
        {
        }

        t = TMR0_GetCurrentTimer();
        TMR0_Disable();

        if(t > SOIL_TIMEOUT_TICKS)
        {
            t = SOIL_TIMEOUT_TICKS;
        }

        aver = aver + t;
    }

    GPIOA_ModeCfg(SOIL_MEASURE_PIN, GPIO_ModeIN_PD);
    GPIOB_ModeCfg(SOIL_REVERSE_PIN, GPIO_ModeIN_PD);

    return (uint16_t)(aver);
}

static uint16_t SoilMovingAverage(uint16_t raw)
{
    static uint16_t samples[5];
    static uint8_t index = 0;
    static uint8_t count = 0;
    static uint32_t sum = 0;

    sum = sum - samples[index] + raw;
    samples[index] = raw;
    index = (index + 1) % 5;

    if(count < 5)
    {
        count++;
    }

    return (uint16_t)(sum / count);
}

static uint16_t SoilPercentX100(uint16_t raw)
{
    if(raw <= SOIL_RAW_DRY)
    {
        return 0;
    }

    if(raw >= SOIL_RAW_WET)
    {
        return 10000;
    }

    return (uint16_t)(((uint32_t)(raw - SOIL_RAW_DRY) * 10000U) / (SOIL_RAW_WET - SOIL_RAW_DRY));
}

/* ===== Vbat pomiar ===== */
uint16_t ReadBattery_mV(void)
{
    uint16_t adc;

    /* ? LED ON (PA8, aktywny LOW) */
    GPIOA_ModeCfg(GPIO_Pin_8, GPIO_ModeOut_PP_5mA);
    GPIOA_ResetBits(GPIO_Pin_8);

    ADC_ExtSingleChSampInit(SampleFreq_3_2, ADC_PGA_0);

    GPIOA_ModeCfg(GPIO_Pin_5, GPIO_ModeOut_PP_5mA);
    GPIOA_ResetBits(GPIO_Pin_5);

    mDelaymS(3);

    GPIOA_ModeCfg(GPIO_Pin_4, GPIO_ModeIN_Floating);

    ADC_ChannelCfg(CH_EXTIN_0);

    mDelayuS(20);

    /* pierwszy pomiar laduje kondensator */
    ADC_ExcutSingleConver();

    /* drugi wlasciwy */
    adc = ADC_ExcutSingleConver();


    /* ? LED OFF */
    GPIOA_SetBits(GPIO_Pin_8);

    GPIOA_ModeCfg(GPIO_Pin_5, GPIO_ModeIN_PU);
    //GPIOA_ModeCfg(GPIO_Pin_4, GPIO_ModeIN_PU);
    
    // kalibracja
    return ((uint32_t)adc * 1460 + 1201000) / 1000;
    //return (adc);
}

static uint8_t BatteryPercentFromMilliVolts(uint16_t mv)
{
    static const uint16_t mv_points[] = {4200, 4100, 3900, 3750, 3700, 3550, 3450, 3000};
    static const uint8_t pct_points[] = {100, 90, 75, 50, 25, 10, 5, 0};

    if(mv >= mv_points[0])
    {
        return pct_points[0];
    }

    if(mv <= mv_points[7])
    {
        return pct_points[7];
    }

    for(uint8_t i = 0; i < 7; i++)
    {
        if(mv <= mv_points[i] && mv > mv_points[i + 1])
        {
            uint16_t mv_hi = mv_points[i];
            uint16_t mv_lo = mv_points[i + 1];
            uint8_t pct_hi = pct_points[i];
            uint8_t pct_lo = pct_points[i + 1];

            return (uint8_t)(pct_lo + (((uint32_t)(mv - mv_lo) * (pct_hi - pct_lo)) / (mv_hi - mv_lo)));
        }
    }

    return 0;
}

/* ===== init ===== */

void Peripheral_Init()
{
    Peripheral_TaskID = TMOS_ProcessEventRegister(Peripheral_ProcessEvent);

    uint8_t adv_enable = TRUE;
    GAPRole_SetParameter(GAPROLE_ADVERT_ENABLED, sizeof(uint8_t), &adv_enable);

    uint16_t advInt = ADV_INTERVAL;
    GAP_SetParamValue(TGAP_DISC_ADV_INT_MIN, advInt);
    GAP_SetParamValue(TGAP_DISC_ADV_INT_MAX, advInt);

    tmos_set_event(Peripheral_TaskID, SBP_START_DEVICE_EVT);
}

/* ===== event loop ===== */

uint16_t Peripheral_ProcessEvent(uint8_t task_id, uint16_t events)
{
    if(events & SBP_START_DEVICE_EVT)
    {
        GAPRole_PeripheralStartDevice(Peripheral_TaskID, NULL, NULL);

        /* pierwszy pomiar po 2 s */
        tmos_start_task(Peripheral_TaskID, SOIL_EVT, 16);

        return (events ^ SBP_START_DEVICE_EVT);
    }

    if(events & SOIL_EVT)
    {
        uint16_t vbat = ReadBattery_mV();
        uint8_t vbat_pct = BatteryPercentFromMilliVolts(vbat);
        uint16_t moisture = SoilRC();
        uint8_t moisture_percent;
        bthome_v2_builder_t bthome;
        uint8_t service_data[20];
        uint8_t adv[31];
        uint8_t len;

        //moisture = SoilPercentX100(moisture);
        //moisture_percent = (uint8_t)((moisture + 50U) / 100U);

        bthome_v2_init(&bthome, service_data, sizeof(service_data), false, false);
        bthome_v2_add_packet_id(&bthome, BTHome_PacketID++);
        bthome_v2_add_object_uint(&bthome, BTHOME_ID_BATTERY_PERCENT, vbat_pct);
        bthome_v2_add_object_uint(&bthome, BTHOME_ID_VOLTAGE_0_001, vbat);
        bthome_v2_add_object_uint(&bthome, BTHOME_ID_MOISTURE_0_01, moisture);

        len = bthome_v2_build_legacy_advertisement(adv,
                                                   sizeof(adv),
                                                   bthome_v2_data(&bthome),
                                                   bthome_v2_size(&bthome),
                                                   BTHOME_NAME,
                                                   true);

        GAPRole_SetParameter(GAPROLE_ADVERT_DATA, len, adv);

    /* zaplanuj wake za x s */
    tmos_start_task(Peripheral_TaskID, SOIL_EVT, SOIL_PERIOD_MS);


    return (events ^ SOIL_EVT);
}

    if(events & SYS_EVENT_MSG)
    {
        uint8_t *pMsg;
        if((pMsg = tmos_msg_receive(Peripheral_TaskID)) != NULL)
        {
            tmos_msg_deallocate(pMsg);
        }
        return (events ^ SYS_EVENT_MSG);
    }

    return 0;
}
