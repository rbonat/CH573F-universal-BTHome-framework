# Door/window sensor example

Standalone CH573 BTHome v2 door/window sensor for MounRiver Studio. It uses a
reed contact on PA9, battery measurement, GPIO wake-up, and non-connectable BLE
advertising.

## Wiring and configuration

- `PA9` is a floating input: use an **external** pull-up or pull-down resistor.
  The NC/NO contact may then connect the pin to either GND or VCC.
- Set `SENSOR_OPEN_LEVEL` to the stable level that means **open** (`1` for VCC,
  `0` for GND). Change `SENSOR_PIN` for another input.
- Configure the battery divider and its calibration in the same file.

## Radio and sleep behaviour

At boot and after a debounced open/close transition, the device sends one
BTHome frame with the opening state, battery percentage, voltage, and low
battery flag. It advertises three times at a 100 ms interval, then three more
times at a 500 ms interval. The radio is then disabled completely.

The fast group improves delivery to an already-scanning nearby receiver; the
spaced group covers BLE scan windows that were not active at the instant of the
state change. Between bursts the CH573 sleeps and wakes from either PA9 edge.
If no edge occurs, the RTC wakes it five minutes after the last burst to send a
fresh status/battery frame.

Import this directory as an existing MounRiver Studio project and build the
`obj` configuration.
