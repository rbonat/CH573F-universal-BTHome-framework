# NTC Temperature Example

Standalone CH573 BTHome v2 temperature sensor project for MounRiver Studio.

It advertises NTC temperature (0.1 C), battery voltage, battery percentage,
and the low-battery boolean in non-connectable BLE advertising packets.

## Hardware

- NTC divider low side: PA5.
- NTC ADC input: PA15 / ADC5.
- Battery divider low side: PA5.
- Battery ADC input: PA4 / ADC0.
- Status LED: PA8, active low.

Adjust GPIO assignments, battery calibration, and the one-point NTC reference
(`NTC_CAL_RAW`, `NTC_CAL_TEMP_X10`) in `config/app_config.h`. Import this
directory as an existing MounRiver Studio project and build the `obj` configuration.
