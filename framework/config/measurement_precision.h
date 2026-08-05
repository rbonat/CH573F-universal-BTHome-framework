#ifndef MEASUREMENT_PRECISION_H
#define MEASUREMENT_PRECISION_H

/*
 * Precision of values returned by sensor drivers.  The fixed-point units of
 * the APIs do not change: battery voltage is mV, NTC temperature is x10 C,
 * and humidity/soil moisture/AHT temperature are x100.
 *
 * Define any of these macros in app_config.h to override its default.
 */
#ifndef BATTERY_VOLTAGE_DECIMAL_PLACES
#define BATTERY_VOLTAGE_DECIMAL_PLACES       2U
#endif

#ifndef SOIL_MOISTURE_DECIMAL_PLACES
#define SOIL_MOISTURE_DECIMAL_PLACES         0U
#endif

#ifndef NTC_TEMPERATURE_DECIMAL_PLACES
#define NTC_TEMPERATURE_DECIMAL_PLACES       1U
#endif

#ifndef AHT_TEMPERATURE_DECIMAL_PLACES
#define AHT_TEMPERATURE_DECIMAL_PLACES       1U
#endif

#ifndef AHT_HUMIDITY_DECIMAL_PLACES
#define AHT_HUMIDITY_DECIMAL_PLACES          0U
#endif

#if (BATTERY_VOLTAGE_DECIMAL_PLACES > 3U)
#error "BATTERY_VOLTAGE_DECIMAL_PLACES must be between 0 and 3"
#endif

#if (SOIL_MOISTURE_DECIMAL_PLACES > 2U)
#error "SOIL_MOISTURE_DECIMAL_PLACES must be between 0 and 2"
#endif

#if (NTC_TEMPERATURE_DECIMAL_PLACES > 1U)
#error "NTC_TEMPERATURE_DECIMAL_PLACES must be between 0 and 1"
#endif

#if (AHT_TEMPERATURE_DECIMAL_PLACES > 2U)
#error "AHT_TEMPERATURE_DECIMAL_PLACES must be between 0 and 2"
#endif

#if (AHT_HUMIDITY_DECIMAL_PLACES > 2U)
#error "AHT_HUMIDITY_DECIMAL_PLACES must be between 0 and 2"
#endif

#endif
