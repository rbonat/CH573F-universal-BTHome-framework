#include "CONFIG.h"
#include "battery.h"

#ifndef BATTERY_VOLTAGE_DECIMAL_PLACES
#define BATTERY_VOLTAGE_DECIMAL_PLACES 2U
#endif

#if (BATTERY_VOLTAGE_DECIMAL_PLACES > 3U)
#error "BATTERY_VOLTAGE_DECIMAL_PLACES must be between 0 and 3"
#endif

static void BatterySensor_AdcPowerDown(void)
{
    R8_ADC_CFG = 0;
    R8_ADC_CONVERT = 0;
    R8_TKEY_CFG &= ~RB_TKEY_PWR_ON;
    R8_TEM_SENSOR = 0;
}

static uint16_t BatterySensor_RoundMilliVolts(uint16_t millivolts)
{
#if BATTERY_VOLTAGE_DECIMAL_PLACES == 0U
    return (uint16_t)((((uint32_t)millivolts + 500U) / 1000U) * 1000U);
#elif BATTERY_VOLTAGE_DECIMAL_PLACES == 1U
    return (uint16_t)((((uint32_t)millivolts + 50U) / 100U) * 100U);
#elif BATTERY_VOLTAGE_DECIMAL_PLACES == 2U
    return (uint16_t)((((uint32_t)millivolts + 5U) / 10U) * 10U);
#else
    return millivolts;
#endif
}

uint16_t BatterySensor_GetRaw(void)
{
    uint16_t adc;

    GPIOA_ModeCfg(STATUS_LED_PIN, GPIO_ModeOut_PP_5mA);
    GPIOA_ResetBits(STATUS_LED_PIN);

    GPIOAGPPCfg(ENABLE, VBAT_MEASURE_ADC_IE);
    GPIOA_ModeCfg(VBAT_MEASURE_PIN, GPIO_ModeIN_Floating);
    GPIOA_ModeCfg(VBAT_GND_PIN, GPIO_ModeOut_PP_5mA);
    GPIOA_ResetBits(VBAT_GND_PIN);

    ADC_ExtSingleChSampInit(SampleFreq_3_2, ADC_PGA_0);
    ADC_ChannelCfg(VBAT_MEASURE_ADC_CH);

    mDelaymS(VBAT_SETTLE_MS);

    ADC_ExcutSingleConver();
    adc = ADC_ExcutSingleConver();

    GPIOA_SetBits(STATUS_LED_PIN);
    GPIOAGPPCfg(DISABLE, VBAT_MEASURE_ADC_IE);
    BatterySensor_AdcPowerDown();

    GPIOA_ModeCfg(VBAT_GND_PIN, GPIO_ModeIN_Floating);
    GPIOA_ModeCfg(VBAT_MEASURE_PIN, GPIO_ModeIN_Floating);
    GPIOA_ModeCfg(STATUS_LED_PIN, GPIO_ModeIN_PU);
    return adc;
}

uint16_t BatterySensor_GetMilliVolts(uint16_t raw_adc)
{
    uint32_t raw_mv;
    uint16_t millivolts;

    if(raw_adc <= 1536U)
    {
        return 0;
    }

    raw_mv = ((((uint32_t)(raw_adc - 1536U) * ADC_VREF_MV) + 256U) / 512U) * VBAT_DIVIDER_RATIO;

    if(VBAT_CAL_POINT1_RAW == VBAT_CAL_POINT2_RAW)
    {
        millivolts = (uint16_t)raw_mv;
    }
    else
    {
        millivolts = (uint16_t)(VBAT_CAL_POINT1_REAL_MV +
                                (((int32_t)raw_adc - (int32_t)VBAT_CAL_POINT1_RAW) *
                                 ((int32_t)VBAT_CAL_POINT2_REAL_MV - (int32_t)VBAT_CAL_POINT1_REAL_MV)) /
                                ((int32_t)VBAT_CAL_POINT2_RAW - (int32_t)VBAT_CAL_POINT1_RAW));
    }

    return BatterySensor_RoundMilliVolts(millivolts);
}

uint8_t BatterySensor_PercentFromMilliVolts(uint16_t mv)
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

bool BatterySensor_IsLow(uint16_t vbat_mv)
{
    static bool battery_low = false;

    if(!battery_low && vbat_mv <= 3400U)
    {
        battery_low = true;
    }
    else if(battery_low && vbat_mv >= 3550U)
    {
        battery_low = false;
    }

    return battery_low;
}
