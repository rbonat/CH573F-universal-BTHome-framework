#ifndef APP_CONFIG_H
#define APP_CONFIG_H

/* TMOS/GAP time units are 625 us. */
#define TEMP_NTC_PERIOD_MS       (60 * 1600)
#define ADV_INTERVAL             (10 * 1600)
#define CALIBRATION_MODE         0
#define BTHOME_NAME              "NTC_TEMP_1"
#define ADC_VREF_MV              1050U

#define TEMP_NTC_EVT             0x4000

/* NTC divider: NTC to VCC, 33k fixed resistor to the switched low side. */
#define NTC_GND_PIN              GPIO_Pin_5
#define NTC_MEASURE_PIN          GPIO_Pin_15
#define NTC_MEASURE_ADC_IE       RB_PIN_ADC4_5_IE
#define NTC_MEASURE_ADC_CH       CH_EXTIN_5
#define NTC_SETTLE_MS            3U
#define NTC_MOVING_AVG_SAMPLES   5U

/* One-point NTC calibration near the normal operating temperature. */
#define NTC_CAL_RAW              1654U
#define NTC_CAL_TEMP_X10         199

/* Battery divider. */
#define STATUS_LED_PIN            GPIO_Pin_8
#define VBAT_GND_PIN              GPIO_Pin_5
#define VBAT_MEASURE_PIN          GPIO_Pin_4
#define VBAT_MEASURE_ADC_IE       RB_PIN_ADC0_IE
#define VBAT_MEASURE_ADC_CH       CH_EXTIN_0
#define VBAT_DIVIDER_RATIO        2U
#define VBAT_SETTLE_MS            3U

#define VBAT_CAL_POINT1_RAW       2716U
#define VBAT_CAL_POINT1_REAL_MV   4000U
#define VBAT_CAL_POINT2_RAW       2948U
#define VBAT_CAL_POINT2_REAL_MV   4500U

#endif
