# Examples

## soil_sensor

`examples/soil_sensor` is a standalone CH573 MounRiver Studio project. It advertises soil moisture, NTC temperature, battery voltage, battery percentage, and low-battery state using BTHome v2 service data.

Import `examples/soil_sensor` directly into MounRiver Studio and build the `obj` configuration.

## temp_hygro

Reserved for a future temperature and humidity sensor example.

## temp_ntc

`examples/temp_ntc` is a standalone CH573 MounRiver Studio project. It advertises NTC temperature, battery voltage, battery percentage, and low-battery state using BTHome v2 service data.

Import `examples/temp_ntc` directly into MounRiver Studio and build the `obj` configuration.

## door_window_sensor

`examples/door_window_sensor` is a standalone CH573 contact sensor project.
The PA9 reed-contact input wakes the sleeping MCU on either edge. After an
open/close transition it emits a six-frame BTHome burst (three at 100 ms, then
three at 500 ms) and disables advertising. A five-minute RTC heartbeat sends
the state and battery data when no transition occurs.

Import `examples/door_window_sensor` directly into MounRiver Studio and build
the `obj` configuration.

## scene_switch

Reserved for a future BTHome scene switch example.
