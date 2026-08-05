#ifndef APP_CONFIG_H
#define APP_CONFIG_H

/* Soil sensor application timing. TMOS/GAP units are 625 us. */
#define TEMP_HYGRO_PERIOD_MS       (60 * 1600)
#define ADV_INTERVAL         (10 * 1600)
#define CALIBRATION_MODE     0
#define BTHOME_NAME          "TEMP_1"
#define ADC_VREF_MV          1050U

#define TEMP_HYGRO_EVT             0x4000

/* GPIO and ADC assignments for the temperature/humidity example. */
#define STATUS_LED_PIN       GPIO_Pin_8

#define AHT2X_I2C_SDA_PORT   SOFT_I2C_PORT_B
#define AHT2X_I2C_SDA_PIN    GPIO_Pin_13
#define AHT2X_I2C_SCL_PORT   SOFT_I2C_PORT_B
#define AHT2X_I2C_SCL_PIN    GPIO_Pin_12
#define AHT2X_I2C_SPEED_HZ   100000UL

#define VBAT_GND_PIN         GPIO_Pin_5
#define VBAT_MEASURE_PIN     GPIO_Pin_4
#define VBAT_MEASURE_ADC_IE  RB_PIN_ADC0_IE
#define VBAT_MEASURE_ADC_CH  CH_EXTIN_0
#define VBAT_DIVIDER_RATIO   2U
#define VBAT_SETTLE_MS       3

/* Calibration values for this example. */
/* Vbat divider: vbat - 1M - ADC - 750k - GND   */
#define VBAT_CAL_POINT1_REAL_MV  3600U
#define VBAT_CAL_POINT1_RAW      2950U
#define VBAT_CAL_POINT2_REAL_MV  4500U
#define VBAT_CAL_POINT2_RAW      3702U



#endif
