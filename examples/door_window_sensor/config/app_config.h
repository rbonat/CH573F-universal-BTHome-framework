#ifndef APP_CONFIG_H
#define APP_CONFIG_H

/* TMOS/GAP time units are 625 us. */
#define BTHOME_NAME                         "DOOR_1"
#define ADC_VREF_MV                         1050U
#define ADV_FAST_INTERVAL                   (160U)  /* 100 ms */
#define ADV_SLOW_INTERVAL                   (800U)  /* 500 ms */

/* Three fast advertisements, followed by three spaced 500 ms apart. */
#define ADV_FAST_PHASE_TMOS                 (320U * 1600U)
#define ADV_SLOW_PHASE_TMOS                 (1600U * 1600U)
#define DOOR_DEBOUNCE_TMOS                  (30U * 1600U)
#define DOOR_STATUS_PERIOD_S                (5UL * 60UL)
#define DOOR_STATUS_PERIOD_RTC_TICKS        (DOOR_STATUS_PERIOD_S * 32768UL)

#define DOOR_DEBOUNCE_EVT                   0x4000
#define DOOR_ADV_SLOW_EVT                   0x2000
#define DOOR_ADV_STOP_EVT                   0x1000
#define DOOR_STATUS_EVT                     0x0800

/*
 * Reed contact input. No internal pull-up/down is enabled: add an external
 * pull resistor and connect the NC/NO contact to either GND or VCC.
 * SENSOR_OPEN_LEVEL is the stable input level that represents an open contact.
 */
#define SENSOR_PIN                          GPIO_Pin_9
#define SENSOR_OPEN_LEVEL                   1U

/* Battery divider. */
#define STATUS_LED_PIN                      GPIO_Pin_8
#define VBAT_GND_PIN                        GPIO_Pin_5
#define VBAT_MEASURE_PIN                    GPIO_Pin_4
#define VBAT_MEASURE_ADC_IE                 RB_PIN_ADC0_IE
#define VBAT_MEASURE_ADC_CH                 CH_EXTIN_0
#define VBAT_DIVIDER_RATIO                  2U
#define VBAT_SETTLE_MS                      3U
#define VBAT_CAL_POINT1_RAW                 2716U
#define VBAT_CAL_POINT1_REAL_MV             4000U
#define VBAT_CAL_POINT2_RAW                 2948U
#define VBAT_CAL_POINT2_REAL_MV             4500U

#endif
