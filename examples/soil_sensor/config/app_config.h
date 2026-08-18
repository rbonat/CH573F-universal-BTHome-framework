#ifndef APP_CONFIG_H
#define APP_CONFIG_H

/* Soil sensor application timing. TMOS/GAP units are 625 us. */
#define SOIL_PERIOD_MS       (60 * 1600)
#define ADV_INTERVAL         (10 * 1600)
#define CALIBRATION_MODE     0
#define BTHOME_NAME          "SOIL_PWM_1"
#define ADC_VREF_MV          1050U

/* Decimal places retained by the soil sensor drivers. */
#define BATTERY_VOLTAGE_DECIMAL_PLACES 2U
#define SOIL_MOISTURE_DECIMAL_PLACES   0U
#define NTC_TEMPERATURE_DECIMAL_PLACES 1U

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

// Battery
#define VBAT_CAL_POINT1_RAW      2716U     // surowy odczyt ADC dla pierwszego punktu kalibracji
#define VBAT_CAL_POINT1_REAL_MV  4000U     // rzeczywiste napiecie pierwszego punktu kalibracji [mV]
#define VBAT_CAL_POINT2_RAW      2948U     // surowy odczyt ADC dla drugiego punktu kalibracji
#define VBAT_CAL_POINT2_REAL_MV  4500U     // rzeczywiste napiecie drugiego punktu kalibracji [mV]

/* Single-point NTC calibration near normal operating temperature. */
#define NTC_CAL_RAW              1654U
#define NTC_CAL_TEMP_X10         236

//Soil callibration
#define SOIL_RAW_DRY         3700         // surowy odczyt ADC odpowiadajacy 0% wilgotnosci
#define SOIL_RAW_WET         1350        // surowy odczyt po filtracji odpowiadajacy 100% wilgotno?ci
#define SOIL_TEMP_REF_X10    200          // temperatura kalibracji soil [0.1C], np. 20.0C
#define SOIL_TEMP_GAIN_X100  150          // wspolczynnik dryftu soil [x100 % / 1C], 100 = 1.00% / C
#define SOIL_TEMP_OFFSET_X100 0           // staly offset korekty soil po kompensacji temperaturowej [x100 %]
#endif
