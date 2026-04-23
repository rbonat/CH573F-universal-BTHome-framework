# BTHome V2 Builder

This library is responsible only for building BTHome V2 messages.
It does not contain BLE stack logic, advertising scheduling, sleep handling, or SDK integration logic.

## Files

- `bthome_v2.h` - public API, `object id` enums, and function declarations
- `bthone_encoder.c` - BTHome V2 builder implementation

## Features

- builds BTHome V2 `service data`
- adds unsigned and signed numeric objects
- adds boolean objects
- adds `packet id`
- adds `text`, `raw`, `button event`, and `dimmer event`
- builds a complete legacy BLE advertisement with optional device name

## Non-goals

- does not send BLE advertisements
- does not control radio or advertising intervals
- does not read sensors
- does not manage application state beyond the builder buffer

## Basic usage

```c
#include "bthome_v2.h"

uint8_t service_data[20];
uint8_t adv[31];
uint8_t len;
static uint8_t packet_id;

bthome_v2_builder_t bthome;

bthome_v2_init(&bthome, service_data, sizeof(service_data), false, false);
bthome_v2_add_packet_id(&bthome, packet_id++);
bthome_v2_add_object_uint(&bthome, BTHOME_ID_BATTERY_PERCENT, 87);
bthome_v2_add_object_uint(&bthome, BTHOME_ID_VOLTAGE_0_001, 4012);
bthome_v2_add_object_uint(&bthome, BTHOME_ID_MOISTURE_0_01, 5234);

len = bthome_v2_build_legacy_advertisement(adv,
                                           sizeof(adv),
                                           bthome_v2_data(&bthome),
                                           bthome_v2_size(&bthome),
                                           "BT_SOIL_1",
                                           true);
```

After that, pass `adv` and `len` to your BLE SDK.

## Object ordering

BTHome requires objects to be added in ascending `object id` order.
The builder enforces this and rejects objects added out of order.

Example of valid ordering:

- `BTHOME_ID_PACKET_ID`
- `BTHOME_ID_BATTERY_PERCENT`
- `BTHOME_ID_VOLTAGE_0_001`
- `BTHOME_ID_MOISTURE_0_01`

## Buffer notes

- the `service_data` buffer must be large enough for the BTHome payload
- the `adv` buffer for legacy BLE is limited to 31 bytes
- adding a device name leaves less room for sensor data

## Main functions

- `bthome_v2_init(...)` - initialize the builder
- `bthome_v2_add_packet_id(...)` - add packet id
- `bthome_v2_add_object_uint(...)` - add unsigned object
- `bthome_v2_add_object_sint(...)` - add signed object
- `bthome_v2_add_boolean(...)` - add boolean object
- `bthome_v2_build_legacy_advertisement(...)` - build final legacy advertisement

## Data format

Object IDs and formats are defined in `bthome_v2.h`.
The library exposes shared enums for common BTHome V2 object types.

