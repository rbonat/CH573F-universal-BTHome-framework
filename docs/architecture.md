# Architecture

This repository separates reusable firmware code from application examples and vendor SDK files.

## Framework Layer

`framework/core` contains BTHome-independent and BTHome-specific reusable logic. The current core module is the BTHome v2 encoder and legacy advertisement payload builder.

`framework/ble` contains reusable BLE profile and service code carried forward from the original project.

`framework/hal` contains CH57x hardware support used across applications: MCU/BLE initialization helpers, RTC timing, sleep support, LED helpers, and key helpers.

`framework/drivers` contains reusable sensor and hardware measurement drivers. Each driver has its own directory, for example `drivers/soil_sensor`, `drivers/ntc_temp`, `drivers/battery`, and `drivers/ath2x`.

The soil sensor driver keeps the original PWM/ADC soil measurement, temperature compensation, and soil filtering behavior. The NTC temperature driver keeps the NTC ADC read, filtering, lookup conversion, and single-point divider-resistor calibration behavior. The battery driver keeps the original switched-divider ADC read, voltage conversion, percentage mapping, and low-battery hysteresis behavior.

`framework/config/default_config.h` contains default CH57x/BLE settings. Example projects provide their own `CONFIG.h` wrapper and app-specific `app_config.h`.

## Example Layer

Each directory under `examples/` is a standalone MounRiver Studio project. The example owns application logic, GPIO assignments, calibration values, and project metadata.

The soil sensor example links `framework/` and `vendor/` through relative Eclipse linked resources, allowing the shared code to compile as part of the example without copying it into the application folder.

## Vendor SDK Layer

`vendor/ch57x_sdk` contains WCH SDK files used by the CH573 example:

- `StdPeriphDriver`
- `Startup`
- `RVMSIS`
- `LIB`
- `Ld`

Vendor files are kept unchanged except for project-level include and linker path references.
