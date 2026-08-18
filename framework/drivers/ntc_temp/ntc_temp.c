#include "CONFIG.h"
#include "ntc_temp.h"

#ifndef NTC_TEMPERATURE_DECIMAL_PLACES
#define NTC_TEMPERATURE_DECIMAL_PLACES 1U
#endif

#if (NTC_TEMPERATURE_DECIMAL_PLACES > 1U)
#error "NTC_TEMPERATURE_DECIMAL_PLACES must be between 0 and 1"
#endif

typedef struct
{
    uint16_t *samples;
    uint8_t sample_count;
    uint8_t index;
    uint8_t count;
    uint32_t sum;
} MovingAverageU16_t;

static void NtcTempSensor_AdcPowerDown(void);
static uint16_t NtcTempSensor_MovingAverage(uint16_t raw);
static int16_t NtcTempSensor_LutTemperatureX10(uint16_t raw);
static int16_t NtcTempSensor_RoundTemperatureX10(int16_t temperature_x10);
#if defined(NTC_CAL_RAW) && defined(NTC_CAL_TEMP_X10)
static uint16_t NtcTempSensor_LutRawForTemperatureX10(int16_t temp_x10);
static uint16_t NtcTempSensor_ApplySinglePointCalibration(uint16_t raw);
#endif

static void NtcTempSensor_AdcPowerDown(void)
{
    R8_ADC_CFG = 0;
    R8_ADC_CONVERT = 0;
    R8_TKEY_CFG &= ~RB_TKEY_PWR_ON;
    R8_TEM_SENSOR = 0;
}

uint16_t NtcTempSensor_GetRaw(void)
{
    uint16_t adc;

    GPIOAGPPCfg(ENABLE, NTC_MEASURE_ADC_IE);
    GPIOA_ModeCfg(NTC_MEASURE_PIN, GPIO_ModeIN_Floating);
    GPIOA_ModeCfg(NTC_GND_PIN, GPIO_ModeOut_PP_5mA);
    GPIOA_ResetBits(NTC_GND_PIN);

    /*
     * With NTC to VCC and a 33k resistor to GND, the -30C..+60C range spans
     * roughly 0.05V..1.88V on PA15, which fits well into the 0dB ADC range.
     */
    ADC_ExtSingleChSampInit(SampleFreq_3_2, ADC_PGA_0);
    ADC_ChannelCfg(NTC_MEASURE_ADC_CH);

    mDelaymS(NTC_SETTLE_MS);

    ADC_ExcutSingleConver();
    adc = ADC_ExcutSingleConver();

    GPIOAGPPCfg(DISABLE, NTC_MEASURE_ADC_IE);
    NtcTempSensor_AdcPowerDown();

    GPIOA_ModeCfg(NTC_GND_PIN, GPIO_ModeIN_Floating);
    GPIOA_ModeCfg(NTC_MEASURE_PIN, GPIO_ModeIN_Floating);

    return adc;
}

static uint16_t NtcTempSensor_MovingAverage(uint16_t raw)
{
    static uint16_t samples[NTC_MOVING_AVG_SAMPLES];
    static MovingAverageU16_t average = {samples, NTC_MOVING_AVG_SAMPLES, 0, 0, 0};

    average.sum = average.sum - average.samples[average.index] + raw;
    average.samples[average.index] = raw;
    average.index = (uint8_t)((average.index + 1) % average.sample_count);

    if(average.count < average.sample_count)
    {
        average.count++;
    }

    return (uint16_t)(average.sum / average.count);
}

static int16_t NtcTempSensor_RoundTemperatureX10(int16_t temperature_x10)
{
#if NTC_TEMPERATURE_DECIMAL_PLACES == 0U
    if(temperature_x10 >= 0)
    {
        return (int16_t)(((temperature_x10 + 5) / 10) * 10);
    }

    return (int16_t)(((temperature_x10 - 5) / 10) * 10);
#else
    return temperature_x10;
#endif
}

static int16_t NtcTempSensor_LutTemperatureX10(uint16_t raw)
{
    static const uint16_t raw_points[] = {104, 144, 195, 262, 345, 449, 575, 727, 905, 1110, 1341, 1597, 1874, 2166, 2469, 2777, 3084, 3383, 3671};
    static const int16_t temp_points_x10[] = {-300, -250, -200, -150, -100, -50, 0, 50, 100, 150, 200, 250, 300, 350, 400, 450, 500, 550, 600};
    const uint8_t point_count = (uint8_t)(sizeof(raw_points) / sizeof(raw_points[0]));

    if(raw <= raw_points[0])
    {
        return (int16_t)(temp_points_x10[0] +
                         (((int32_t)(raw - raw_points[0]) * (temp_points_x10[1] - temp_points_x10[0])) /
                          (raw_points[1] - raw_points[0])));
    }

    if(raw >= raw_points[point_count - 1])
    {
        return (int16_t)(temp_points_x10[point_count - 1] +
                         (((int32_t)(raw - raw_points[point_count - 1]) *
                           (temp_points_x10[point_count - 1] - temp_points_x10[point_count - 2])) /
                          (raw_points[point_count - 1] - raw_points[point_count - 2])));
    }

    for(uint8_t i = 0; i < point_count - 1; i++)
    {
        uint16_t raw_lo = raw_points[i];
        uint16_t raw_hi = raw_points[i + 1];
        int16_t temp_lo = temp_points_x10[i];
        int16_t temp_hi = temp_points_x10[i + 1];

        if(raw >= raw_lo && raw <= raw_hi)
        {
            return (int16_t)(temp_lo + (((int32_t)(raw - raw_lo) * (temp_hi - temp_lo)) / (raw_hi - raw_lo)));
        }
    }

    return temp_points_x10[point_count - 1];
}

#if defined(NTC_CAL_RAW) && defined(NTC_CAL_TEMP_X10)
static uint16_t NtcTempSensor_LutRawForTemperatureX10(int16_t temp_x10)
{
    static const uint16_t raw_points[] = {104, 144, 195, 262, 345, 449, 575, 727, 905, 1110, 1341, 1597, 1874, 2166, 2469, 2777, 3084, 3383, 3671};
    static const int16_t temp_points_x10[] = {-300, -250, -200, -150, -100, -50, 0, 50, 100, 150, 200, 250, 300, 350, 400, 450, 500, 550, 600};
    const uint8_t point_count = (uint8_t)(sizeof(raw_points) / sizeof(raw_points[0]));
    uint8_t i;

    if(temp_x10 <= temp_points_x10[0])
    {
        i = 0;
    }
    else if(temp_x10 >= temp_points_x10[point_count - 1])
    {
        i = (uint8_t)(point_count - 2);
    }
    else
    {
        for(i = 0; i < point_count - 1; i++)
        {
            if(temp_x10 <= temp_points_x10[i + 1])
            {
                break;
            }
        }
    }

    return (uint16_t)(raw_points[i] +
                      (((int32_t)(temp_x10 - temp_points_x10[i]) * (raw_points[i + 1] - raw_points[i])) /
                       (temp_points_x10[i + 1] - temp_points_x10[i])));
}

static uint16_t NtcTempSensor_ApplySinglePointCalibration(uint16_t raw)
{
    const uint16_t adc_max = 4095U;
    uint16_t reference_raw = NtcTempSensor_LutRawForTemperatureX10(NTC_CAL_TEMP_X10);
    uint32_t calibration_numerator;
    uint32_t calibration_denominator;
    uint64_t denominator;
    uint64_t corrected_raw;

    if(raw == 0U || raw >= adc_max || NTC_CAL_RAW == 0U || NTC_CAL_RAW >= adc_max ||
       reference_raw == 0U || reference_raw >= adc_max)
    {
        return raw;
    }

    /*
     * The LUT represents the nominal divider. A single reference point gives
     * the ratio between the actual and nominal fixed divider resistor. Map
     * the measured ADC code back to its equivalent nominal LUT code so the
     * NTC curve and its non-linearity remain intact.
     */
    calibration_numerator = (uint32_t)NTC_CAL_RAW * (adc_max - reference_raw);
    calibration_denominator = (uint32_t)reference_raw * (adc_max - NTC_CAL_RAW);
    denominator = ((uint64_t)raw * calibration_denominator) +
                  ((uint64_t)calibration_numerator * (adc_max - raw));

    if(denominator == 0ULL)
    {
        return raw;
    }

    corrected_raw = (((uint64_t)adc_max * raw * calibration_denominator) + (denominator / 2ULL)) / denominator;

    if(corrected_raw > adc_max)
    {
        return adc_max;
    }

    return (uint16_t)corrected_raw;
}
#endif

int16_t NtcTempSensor_GetTemperatureX10(uint16_t raw)
{
    uint16_t filtered_raw = NtcTempSensor_MovingAverage(raw);
    int16_t temperature_x10;

#if defined(NTC_CAL_RAW) && defined(NTC_CAL_TEMP_X10)
    temperature_x10 = NtcTempSensor_LutTemperatureX10(NtcTempSensor_ApplySinglePointCalibration(filtered_raw));
#else
    temperature_x10 = NtcTempSensor_LutTemperatureX10(filtered_raw);
#endif

    return NtcTempSensor_RoundTemperatureX10(temperature_x10);
}
