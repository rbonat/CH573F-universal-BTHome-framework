#ifndef APP_CONFIG_H
#define APP_CONFIG_H

/* Soil sensor application timing. TMOS/GAP units are 625 us. */
#define SOIL_PERIOD_MS       (60 * 1600)
#define ADV_INTERVAL         (10 * 1600)
#define CALIBRATION_MODE     0
#define BTHOME_NAME          "SOIL_PWM_1"
#define ADC_VREF_MV          1050U

#define SOIL_EVT             0x4000

/* GPIO and ADC assignments for the soil sensor example. */
#define STATUS_LED_PIN       GPIO_Pin_8

#define SOIL_MEASURE_PIN     GPIO_Pin_9
#define SOIL_MEASURE_ADC_IE  RB_PIN_ADC13_IE
#define SOIL_MEASURE_ADC_CH  CH_EXTIN_13
#define SOIL_REVERSE_PIN     GPIO_Pin_14
#define SOIL_PWM_SETTLE_MS   15
#define SOIL_PWM_FREQ_HZ     1600000UL
#define SOIL_PWM_DUTY_PERCENT 30U
#define SOIL_MOVING_AVG_SAMPLES 5U

#define NTC_GND_PIN          GPIO_Pin_5
#define NTC_MEASURE_PIN      GPIO_Pin_15
#define NTC_MEASURE_ADC_IE   RB_PIN_ADC4_5_IE
#define NTC_MEASURE_ADC_CH   CH_EXTIN_5
#define NTC_SETTLE_MS        3
#define NTC_MOVING_AVG_SAMPLES 5U

#define VBAT_GND_PIN         GPIO_Pin_5
#define VBAT_MEASURE_PIN     GPIO_Pin_4
#define VBAT_MEASURE_ADC_IE  RB_PIN_ADC0_IE
#define VBAT_MEASURE_ADC_CH  CH_EXTIN_0
#define VBAT_DIVIDER_RATIO   2U
#define VBAT_SETTLE_MS       3

/* Calibration values for this soil sensor example. */
#define VBAT_CAL_POINT1_RAW      2716U
#define VBAT_CAL_POINT1_REAL_MV  4000U
#define VBAT_CAL_POINT2_RAW      2948U
#define VBAT_CAL_POINT2_REAL_MV  4500U

/* Single-point NTC calibration near normal operating temperature. */
#define NTC_CAL_RAW              1654U
#define NTC_CAL_TEMP_X10         199

#define SOIL_RAW_DRY             3700
#define SOIL_RAW_WET             1350
#define SOIL_TEMP_REF_X10        200
#define SOIL_TEMP_GAIN_X100      150
#define SOIL_TEMP_OFFSET_X100    0

#endif
