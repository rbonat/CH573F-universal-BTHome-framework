# ch57x-bthome-framework

Reusable BTHome firmware framework for WCH CH57x BLE microcontrollers, starting from a CH573 soil sensor application.

The repository keeps the original CH573 firmware behavior while reorganizing the code into shared framework modules, vendor SDK files, and standalone MounRiver Studio examples. The first example advertises soil moisture, temperature, battery voltage, battery percentage, and low-battery state using BTHome v2 service data for Home Assistant discovery.

## Features

- BTHome v2 advertisement payload encoder.
- CH57x BLE peripheral startup and GAP advertising support.
- Low-power RTC sleep support from the original CH573 project.
- Soil moisture measurement using the existing PWM/ADC algorithm.
- NTC temperature measurement with the existing lookup and calibration flow.
- Battery voltage, battery percentage, and low-battery hysteresis helpers.
- Standalone MounRiver Studio soil sensor example.
- Standalone MounRiver Studio door/window sensor example with GPIO wake-up.
- Repository layout prepared for future CH582 support.

## Supported MCUs

- CH573F: supported by the included soil sensor example.
- CH582: planned. The layout keeps vendor SDK and framework code separated so another SDK/profile target can be added without changing application structure.

## Repository Structure

```text
framework/              Shared BTHome, BLE, HAL, driver, and default config code
vendor/ch57x_sdk/       WCH CH57x SDK files used by the examples
examples/soil_sensor/   Standalone MounRiver Studio CH573 soil sensor project
examples/temp_ntc/      Standalone MounRiver Studio CH573 NTC temperature project
examples/door_window_sensor/ CH573 door/window contact project
templates/new_sensor/   Minimal starting point for new sensor examples
docs/                   Architecture, porting, and example documentation
tools/                  Utility scripts can be added here
```

## Build Instructions

1. Clone this repository.
2. Open MounRiver Studio.
3. Import `examples/soil_sensor` as an existing project.
4. Build the `obj` configuration.

The example project uses relative linked resources for `framework/` and `vendor/`, so it can be imported from a fresh clone without machine-specific paths.

## Home Assistant Compatibility

The soil sensor example emits BTHome v2 service data in non-connectable BLE advertising packets. Home Assistant can discover the sensor through a Bluetooth adapter or Bluetooth proxy that supports passive BLE advertisements.

Advertised entities:

- Battery percentage
- Battery voltage
- Soil moisture
- Battery low
- Temperature

## Power-Saving Design

The framework preserves the original CH573 low-power behavior:

- RTC/TMOS timing remains in the HAL layer.
- Sleep configuration remains controlled by the CH57x BLE configuration macros.
- Sensor GPIOs are returned to floating/input states after measurements.
- ADC and touch-key related analog blocks are powered down after sensor reads.

## Roadmap

- Add a CH582 example once the target SDK and linker setup are validated.
- Add temperature/humidity and scene-switch examples.
- Add optional helper scripts for consistency checks and generated MounRiver metadata.
- Expand reusable BLE advertisement helpers as more examples are added.
