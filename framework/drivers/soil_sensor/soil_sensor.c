#include "CONFIG.h"
#include "soil_sensor.h"
#include "measurement_precision.h"

typedef struct
{
    uint16_t *samples;
    uint8_t sample_count;
    uint8_t index;
    uint8_t count;
    uint32_t sum;
} MovingAverageU16_t;

static uint16_t MovingAverageU16(MovingAverageU16_t *average, uint16_t raw);
static void SoilSensor_AdcPowerDown(void);

static uint16_t SoilSensor_RoundPercentX100(uint16_t percent_x100)
{
#if SOIL_MOISTURE_DECIMAL_PLACES == 0U
    return (uint16_t)((((uint32_t)percent_x100 + 50U) / 100U) * 100U);
#elif SOIL_MOISTURE_DECIMAL_PLACES == 1U
    return (uint16_t)((((uint32_t)percent_x100 + 5U) / 10U) * 10U);
#else
    return percent_x100;
#endif
}

static void SoilSensor_AdcPowerDown(void)
{
    R8_ADC_CFG = 0;
    R8_ADC_CONVERT = 0;
    R8_TKEY_CFG &= ~RB_TKEY_PWR_ON;
    R8_TEM_SENSOR = 0;
}

uint16_t SoilSensor_GetSoilRaw(void)
{
    uint16_t adc;
    uint32_t sys_clk = GetSysClock();
    uint32_t pwm_period_counts = 31UL;
    uint8_t pwm_cfg = (3 << 2) | 0x01;
    uint64_t pwm_period_product = (uint64_t)SOIL_PWM_FREQ_HZ * pwm_period_counts;
    uint32_t pwm_div_floor;
    uint32_t pwm_div_ceil;
    uint32_t pwm_div_u32;
    uint8_t pwm_div;
    uint8_t pwm_duty = (uint8_t)((pwm_period_counts * SOIL_PWM_DUTY_PERCENT + 50U) / 100U);
    uint32_t freq_floor;
    uint32_t freq_ceil;
    uint32_t err_floor;
    uint32_t err_ceil;

    if(pwm_period_product == 0ULL)
    {
        pwm_period_product = 1ULL;
    }

    pwm_div_floor = (uint32_t)((uint64_t)sys_clk / pwm_period_product);
    pwm_div_ceil = (uint32_t)(((uint64_t)sys_clk + pwm_period_product - 1ULL) / pwm_period_product);

    if(pwm_div_floor < 1UL)
    {
        pwm_div_floor = 1UL;
    }
    else if(pwm_div_floor > 255UL)
    {
        pwm_div_floor = 255UL;
    }

    if(pwm_div_ceil < 1UL)
    {
        pwm_div_ceil = 1UL;
    }
    else if(pwm_div_ceil > 255UL)
    {
        pwm_div_ceil = 255UL;
    }

    freq_floor = sys_clk / (pwm_div_floor * pwm_period_counts);
    freq_ceil = sys_clk / (pwm_div_ceil * pwm_period_counts);
    err_floor = (freq_floor > SOIL_PWM_FREQ_HZ) ? (freq_floor - SOIL_PWM_FREQ_HZ) : (SOIL_PWM_FREQ_HZ - freq_floor);
    err_ceil = (freq_ceil > SOIL_PWM_FREQ_HZ) ? (freq_ceil - SOIL_PWM_FREQ_HZ) : (SOIL_PWM_FREQ_HZ - freq_ceil);

    pwm_div_u32 = (err_floor <= err_ceil) ? pwm_div_floor : pwm_div_ceil;
    pwm_div = (uint8_t)pwm_div_u32;

    GPIOAGPPCfg(ENABLE, SOIL_MEASURE_ADC_IE);
    GPIOA_ModeCfg(SOIL_MEASURE_PIN, GPIO_ModeIN_Floating);
    GPIOB_ModeCfg(SOIL_REVERSE_PIN, GPIO_ModeOut_PP_5mA);

    PWMX_CLKCfg(pwm_div);
    R8_PWM_CONFIG = (R8_PWM_CONFIG & 0xF0) | pwm_cfg;
    R8_PWM_POLAR &= ~RB_PWM10_POLAR;
    R8_PWM10_DATA = pwm_duty;
    R8_PWM_OUT_EN |= RB_PWM10_OUT_EN;

    /* 0 dB gives much better code spread for the expected ~0.8 V to ~2.1 V soil range. */
    ADC_ExtSingleChSampInit(SampleFreq_3_2, ADC_PGA_0);
    ADC_ChannelCfg(SOIL_MEASURE_ADC_CH);

    mDelaymS(SOIL_PWM_SETTLE_MS);

    ADC_ExcutSingleConver();
    adc = ADC_ExcutSingleConver();

    R8_PWM_OUT_EN &= ~RB_PWM10_OUT_EN;
    GPIOAGPPCfg(DISABLE, SOIL_MEASURE_ADC_IE);
    SoilSensor_AdcPowerDown();

    GPIOA_ModeCfg(SOIL_MEASURE_PIN, GPIO_ModeIN_Floating);
    GPIOB_ModeCfg(SOIL_REVERSE_PIN, GPIO_ModeIN_Floating);

    return adc;
}

uint16_t SoilSensor_FilterSoilRaw(uint16_t raw)
{
    static uint16_t samples[SOIL_MOVING_AVG_SAMPLES];
    static MovingAverageU16_t average = {samples, SOIL_MOVING_AVG_SAMPLES, 0, 0, 0};

    return MovingAverageU16(&average, raw);
}

static uint16_t MovingAverageU16(MovingAverageU16_t *average, uint16_t raw)
{
    average->sum = average->sum - average->samples[average->index] + raw;
    average->samples[average->index] = raw;
    average->index = (uint8_t)((average->index + 1) % average->sample_count);

    if(average->count < average->sample_count)
    {
        average->count++;
    }

    return (uint16_t)(average->sum / average->count);
}

uint16_t SoilSensor_SoilPercentX100(uint16_t raw)
{
    uint16_t linear_percent_x100;

    if(SOIL_RAW_WET > SOIL_RAW_DRY)
    {
        if(raw <= SOIL_RAW_DRY)
        {
            return SoilSensor_RoundPercentX100(0);
        }

        if(raw >= SOIL_RAW_WET)
        {
            return SoilSensor_RoundPercentX100(10000);
        }

        linear_percent_x100 = (uint16_t)(((uint32_t)(raw - SOIL_RAW_DRY) * 10000U) / (SOIL_RAW_WET - SOIL_RAW_DRY));
    }
    else
    {
        if(raw >= SOIL_RAW_DRY)
        {
            return SoilSensor_RoundPercentX100(0);
        }

        if(raw <= SOIL_RAW_WET)
        {
            return SoilSensor_RoundPercentX100(10000);
        }

        linear_percent_x100 = (uint16_t)(((uint32_t)(SOIL_RAW_DRY - raw) * 10000U) / (SOIL_RAW_DRY - SOIL_RAW_WET));
    }
    return SoilSensor_RoundPercentX100(linear_percent_x100);
    /*
     * Approximate logarithmic correction:
     * the raw sensor response grows too fast in the low-moisture region,
     * so after linear normalization we remap it with a hand-tuned piecewise
     * curve. This avoids floating-point math and real log() calls while still
     * strongly compressing mid/high linear values (for example ~80% linear
     * becomes roughly ~28% reported).
     */
    /*
    {
        static const uint16_t in_points[] = {0, 1000, 2000, 3000, 4000, 5000, 6000, 7000, 8000, 9000, 10000};
        static const uint16_t out_points[] = {0, 70, 200, 415, 750, 1275, 2000, 3000, 4350, 6450, 10000};

        for(uint8_t i = 0; i < 10; i++)
        {
            if(linear_percent_x100 <= in_points[i + 1])
            {
                uint16_t in_lo = in_points[i];
                uint16_t in_hi = in_points[i + 1];
                uint16_t out_lo = out_points[i];
                uint16_t out_hi = out_points[i + 1];
                uint16_t corrected;

                corrected = (uint16_t)(out_lo + (((uint32_t)(linear_percent_x100 - in_lo) * (out_hi - out_lo)) / (in_hi - in_lo)));

                return (uint16_t)(((corrected + 5U) / 10U) * 10U);
            }
        }
    }

    return 10000;
    */
}

uint16_t SoilSensor_ApplyTemperatureCorrectionX100(uint16_t soil_x100, int16_t temp_x10)
{
    int32_t scale_x10000;
    int32_t corrected_x100;

    /*
     * Temperature compensation around the soil calibration point:
     * y = a * x + b
     * where:
     * - x is the current soil value in x100 percent
     * - a changes linearly with temperature around SOIL_TEMP_REF_X10
     * - b is a fixed offset in x100 percent
     *
     * SOIL_TEMP_GAIN_X100 is expressed as x100 % / 1C, so:
     * 100 = 1.00% soil change per 1C.
     *
     * The drift is inverse to temperature, so above the calibration
     * temperature the reported soil value is scaled up, and below it
     * scaled down.
     */
    scale_x10000 = 10000 +
                   ((((int32_t)temp_x10 - SOIL_TEMP_REF_X10) * SOIL_TEMP_GAIN_X100) / 10);

    if(scale_x10000 < 0)
    {
        scale_x10000 = 0;
    }

    corrected_x100 = (((int32_t)soil_x100 * scale_x10000) + 5000) / 10000;
    corrected_x100 += SOIL_TEMP_OFFSET_X100;

    if(corrected_x100 < 0)
    {
        return SoilSensor_RoundPercentX100(0);
    }

    if(corrected_x100 > 10000)
    {
        return SoilSensor_RoundPercentX100(10000);
    }

    return SoilSensor_RoundPercentX100((uint16_t)corrected_x100);
}
