#ifndef BTHOME_V2_H
#define BTHOME_V2_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#define BTHOME_V2_UUID_LSB 0xD2
#define BTHOME_V2_UUID_MSB 0xFC
#define BTHOME_V2_DEVICE_INFO_BASE 0x40

typedef enum
{
    BTHOME_V2_FMT_UINT8,
    BTHOME_V2_FMT_UINT16,
    BTHOME_V2_FMT_UINT24,
    BTHOME_V2_FMT_UINT32,
    BTHOME_V2_FMT_SINT8,
    BTHOME_V2_FMT_SINT16,
    BTHOME_V2_FMT_SINT24,
    BTHOME_V2_FMT_SINT32,
    BTHOME_V2_FMT_TEXT,
    BTHOME_V2_FMT_RAW,
    BTHOME_V2_FMT_EVENT,
    BTHOME_V2_FMT_DIMMER_EVENT,
} bthome_v2_format_t;

typedef enum
{
    BTHOME_ID_PACKET_ID = 0x00,
    BTHOME_ID_BATTERY_PERCENT = 0x01,
    BTHOME_ID_TEMPERATURE_0_01 = 0x02,
    BTHOME_ID_HUMIDITY_0_01 = 0x03,
    BTHOME_ID_PRESSURE_0_01 = 0x04,
    BTHOME_ID_ILLUMINANCE_0_01 = 0x05,
    BTHOME_ID_MASS_KG_0_01 = 0x06,
    BTHOME_ID_MASS_LB_0_01 = 0x07,
    BTHOME_ID_DEWPOINT_0_01 = 0x08,
    BTHOME_ID_COUNT_UINT8 = 0x09,
    BTHOME_ID_ENERGY_KWH_0_001 = 0x0A,
    BTHOME_ID_POWER_W_0_01 = 0x0B,
    BTHOME_ID_VOLTAGE_0_001 = 0x0C,
    BTHOME_ID_PM2_5 = 0x0D,
    BTHOME_ID_PM10 = 0x0E,
    BTHOME_ID_GENERIC_BOOLEAN = 0x0F,
    BTHOME_ID_POWER_BOOLEAN = 0x10,
    BTHOME_ID_OPENING_BOOLEAN = 0x11,
    BTHOME_ID_CO2 = 0x12,
    BTHOME_ID_TVOC = 0x13,
    BTHOME_ID_MOISTURE_0_01 = 0x14,
    BTHOME_ID_BATTERY_LOW_BOOLEAN = 0x15,
    BTHOME_ID_BATTERY_CHARGING_BOOLEAN = 0x16,
    BTHOME_ID_CARBON_MONOXIDE_BOOLEAN = 0x17,
    BTHOME_ID_COLD_BOOLEAN = 0x18,
    BTHOME_ID_CONNECTIVITY_BOOLEAN = 0x19,
    BTHOME_ID_DOOR_BOOLEAN = 0x1A,
    BTHOME_ID_GARAGE_DOOR_BOOLEAN = 0x1B,
    BTHOME_ID_GAS_BOOLEAN = 0x1C,
    BTHOME_ID_HEAT_BOOLEAN = 0x1D,
    BTHOME_ID_LIGHT_BOOLEAN = 0x1E,
    BTHOME_ID_LOCK_BOOLEAN = 0x1F,
    BTHOME_ID_MOISTURE_BOOLEAN = 0x20,
    BTHOME_ID_MOTION_BOOLEAN = 0x21,
    BTHOME_ID_MOVING_BOOLEAN = 0x22,
    BTHOME_ID_OCCUPANCY_BOOLEAN = 0x23,
    BTHOME_ID_PLUG_BOOLEAN = 0x24,
    BTHOME_ID_PRESENCE_BOOLEAN = 0x25,
    BTHOME_ID_PROBLEM_BOOLEAN = 0x26,
    BTHOME_ID_RUNNING_BOOLEAN = 0x27,
    BTHOME_ID_SAFETY_BOOLEAN = 0x28,
    BTHOME_ID_SMOKE_BOOLEAN = 0x29,
    BTHOME_ID_SOUND_BOOLEAN = 0x2A,
    BTHOME_ID_TAMPER_BOOLEAN = 0x2B,
    BTHOME_ID_VIBRATION_BOOLEAN = 0x2C,
    BTHOME_ID_WINDOW_BOOLEAN = 0x2D,
    BTHOME_ID_HUMIDITY_1 = 0x2E,
    BTHOME_ID_MOISTURE_1 = 0x2F,
    BTHOME_ID_BUTTON_EVENT = 0x3A,
    BTHOME_ID_COUNT_UINT16 = 0x3D,
    BTHOME_ID_COUNT_UINT32 = 0x3E,
    BTHOME_ID_ROTATION_0_1 = 0x3F,
    BTHOME_ID_DISTANCE_MM = 0x40,
    BTHOME_ID_DISTANCE_M_0_1 = 0x41,
    BTHOME_ID_DURATION_S_0_001 = 0x42,
    BTHOME_ID_CURRENT_A_0_001 = 0x43,
    BTHOME_ID_SPEED_MPS_0_01 = 0x44,
    BTHOME_ID_TEMPERATURE_0_1 = 0x45,
    BTHOME_ID_UV_INDEX_0_1 = 0x46,
    BTHOME_ID_VOLUME_L_0_1 = 0x47,
    BTHOME_ID_VOLUME_ML = 0x48,
    BTHOME_ID_VOLUME_FLOW_RATE_0_001 = 0x49,
    BTHOME_ID_VOLTAGE_0_1 = 0x4A,
    BTHOME_ID_GAS_M3_0_001_UINT24 = 0x4B,
    BTHOME_ID_GAS_M3_0_001_UINT32 = 0x4C,
    BTHOME_ID_ENERGY_KWH_0_001_UINT32 = 0x4D,
    BTHOME_ID_VOLUME_L_0_001 = 0x4E,
    BTHOME_ID_WATER_L_0_001 = 0x4F,
    BTHOME_ID_TIMESTAMP = 0x50,
    BTHOME_ID_ACCELERATION_0_001 = 0x51,
    BTHOME_ID_GYROSCOPE_0_001 = 0x52,
    BTHOME_ID_TEXT = 0x53,
    BTHOME_ID_RAW = 0x54,
    BTHOME_ID_VOLUME_STORAGE_L_0_001 = 0x55,
    BTHOME_ID_CONDUCTIVITY = 0x56,
    BTHOME_ID_TEMPERATURE_S8_1 = 0x57,
    BTHOME_ID_TEMPERATURE_S8_0_35 = 0x58,
    BTHOME_ID_COUNT_SINT8 = 0x59,
    BTHOME_ID_COUNT_SINT16 = 0x5A,
    BTHOME_ID_COUNT_SINT32 = 0x5B,
    BTHOME_ID_POWER_W_0_01_SINT32 = 0x5C,
    BTHOME_ID_CURRENT_A_0_001_SINT16 = 0x5D,
    BTHOME_ID_DIRECTION_0_01 = 0x5E,
    BTHOME_ID_PRECIPITATION_MM_0_1 = 0x5F,
    BTHOME_ID_CHANNEL = 0x60,
    BTHOME_ID_ROTATIONAL_SPEED_RPM = 0x61,
    BTHOME_ID_SPEED_MPS_0_000001_SINT32 = 0x62,
    BTHOME_ID_ACCELERATION_0_000001_SINT32 = 0x63,
    BTHOME_ID_LIGHT_LEVEL = 0x64,
    BTHOME_ID_DIMMER_EVENT = 0x3C,
    BTHOME_ID_DEVICE_TYPE_ID = 0xF0,
    BTHOME_ID_FIRMWARE_VERSION_UINT32 = 0xF1,
    BTHOME_ID_FIRMWARE_VERSION_UINT24 = 0xF2
} bthome_v2_object_id_t;

typedef struct
{
    uint8_t object_id;
    bthome_v2_format_t format;
} bthome_v2_object_info_t;

typedef struct
{
    uint8_t *buffer;
    uint8_t capacity;
    uint8_t length;
    uint8_t last_object_id;
} bthome_v2_builder_t;

void bthome_v2_init(bthome_v2_builder_t *builder,
                    uint8_t *buffer,
                    uint8_t capacity,
                    bool encrypted,
                    bool trigger_based);

const uint8_t *bthome_v2_data(const bthome_v2_builder_t *builder);
uint8_t bthome_v2_size(const bthome_v2_builder_t *builder);

const bthome_v2_object_info_t *bthome_v2_get_object_info(uint8_t object_id);

bool bthome_v2_add_object_uint(bthome_v2_builder_t *builder, uint8_t object_id, uint32_t value);
bool bthome_v2_add_object_sint(bthome_v2_builder_t *builder, uint8_t object_id, int32_t value);
bool bthome_v2_add_boolean(bthome_v2_builder_t *builder, uint8_t object_id, bool value);
bool bthome_v2_add_packet_id(bthome_v2_builder_t *builder, uint8_t packet_id);
bool bthome_v2_add_text(bthome_v2_builder_t *builder, const char *text);
bool bthome_v2_add_raw(bthome_v2_builder_t *builder, const uint8_t *data, uint8_t data_len);
bool bthome_v2_add_button_event(bthome_v2_builder_t *builder, uint8_t event_id);
bool bthome_v2_add_dimmer_event(bthome_v2_builder_t *builder, uint8_t event_id, uint8_t steps);

uint8_t bthome_v2_build_legacy_advertisement(uint8_t *adv,
                                             uint8_t adv_capacity,
                                             const uint8_t *service_data,
                                             uint8_t service_data_len,
                                             const char *local_name,
                                             bool complete_name);

#endif
