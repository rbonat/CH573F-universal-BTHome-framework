# Soil Sensor Example

Standalone CH573 BTHome soil sensor example for MounRiver Studio.

## Hardware

The example targets the original CH573 soil sensor hardware used by this project. It measures:

- Soil moisture through a PWM-driven probe and ADC input.
- Temperature through an NTC divider.
- Battery voltage through a switched divider.

## GPIO Configuration

GPIO and ADC assignments live in `config/app_config.h`.

Current assignments:

- Status LED: PA8, active low.
- Soil ADC input: PA9 / ADC13.
- Soil PWM drive: PB14.
- NTC divider low side: PA5.
- NTC ADC input: PA15 / ADC5.
- Battery divider low side: PA5.
- Battery ADC input: PA4 / ADC0.

## BTHome Entities

The advertisement payload contains:

- Packet ID
- Battery percentage
- Battery voltage, 0.001 V resolution
- Moisture, 0.01 percent resolution
- Battery low boolean
- Temperature, 0.1 C resolution

## Home Assistant Integration

Use a Home Assistant Bluetooth adapter or Bluetooth proxy with passive BLE advertisement support. The device advertises BTHome v2 service data in non-connectable advertising packets.

Calibration and GPIO changes should be made in `config/app_config.h`.
