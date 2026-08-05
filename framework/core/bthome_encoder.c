#include "bthome_v2.h"

static const bthome_v2_object_info_t bthome_v2_object_table[] = {
    {BTHOME_ID_PACKET_ID, BTHOME_V2_FMT_UINT8},
    {BTHOME_ID_BATTERY_PERCENT, BTHOME_V2_FMT_UINT8},
    {BTHOME_ID_TEMPERATURE_0_01, BTHOME_V2_FMT_SINT16},
    {BTHOME_ID_HUMIDITY_0_01, BTHOME_V2_FMT_UINT16},
    {BTHOME_ID_PRESSURE_0_01, BTHOME_V2_FMT_UINT24},
    {BTHOME_ID_ILLUMINANCE_0_01, BTHOME_V2_FMT_UINT24},
    {BTHOME_ID_MASS_KG_0_01, BTHOME_V2_FMT_UINT16},
    {BTHOME_ID_MASS_LB_0_01, BTHOME_V2_FMT_UINT16},
    {BTHOME_ID_DEWPOINT_0_01, BTHOME_V2_FMT_SINT16},
    {BTHOME_ID_COUNT_UINT8, BTHOME_V2_FMT_UINT8},
    {BTHOME_ID_ENERGY_KWH_0_001, BTHOME_V2_FMT_UINT24},
    {BTHOME_ID_POWER_W_0_01, BTHOME_V2_FMT_UINT24},
    {BTHOME_ID_VOLTAGE_0_001, BTHOME_V2_FMT_UINT16},
    {BTHOME_ID_PM2_5, BTHOME_V2_FMT_UINT16},
    {BTHOME_ID_PM10, BTHOME_V2_FMT_UINT16},
    {BTHOME_ID_GENERIC_BOOLEAN, BTHOME_V2_FMT_UINT8},
    {BTHOME_ID_POWER_BOOLEAN, BTHOME_V2_FMT_UINT8},
    {BTHOME_ID_OPENING_BOOLEAN, BTHOME_V2_FMT_UINT8},
    {BTHOME_ID_CO2, BTHOME_V2_FMT_UINT16},
    {BTHOME_ID_TVOC, BTHOME_V2_FMT_UINT16},
    {BTHOME_ID_MOISTURE_0_01, BTHOME_V2_FMT_UINT16},
    {BTHOME_ID_BATTERY_LOW_BOOLEAN, BTHOME_V2_FMT_UINT8},
    {BTHOME_ID_BATTERY_CHARGING_BOOLEAN, BTHOME_V2_FMT_UINT8},
    {BTHOME_ID_CARBON_MONOXIDE_BOOLEAN, BTHOME_V2_FMT_UINT8},
    {BTHOME_ID_COLD_BOOLEAN, BTHOME_V2_FMT_UINT8},
    {BTHOME_ID_CONNECTIVITY_BOOLEAN, BTHOME_V2_FMT_UINT8},
    {BTHOME_ID_DOOR_BOOLEAN, BTHOME_V2_FMT_UINT8},
    {BTHOME_ID_GARAGE_DOOR_BOOLEAN, BTHOME_V2_FMT_UINT8},
    {BTHOME_ID_GAS_BOOLEAN, BTHOME_V2_FMT_UINT8},
    {BTHOME_ID_HEAT_BOOLEAN, BTHOME_V2_FMT_UINT8},
    {BTHOME_ID_LIGHT_BOOLEAN, BTHOME_V2_FMT_UINT8},
    {BTHOME_ID_LOCK_BOOLEAN, BTHOME_V2_FMT_UINT8},
    {BTHOME_ID_MOISTURE_BOOLEAN, BTHOME_V2_FMT_UINT8},
    {BTHOME_ID_MOTION_BOOLEAN, BTHOME_V2_FMT_UINT8},
    {BTHOME_ID_MOVING_BOOLEAN, BTHOME_V2_FMT_UINT8},
    {BTHOME_ID_OCCUPANCY_BOOLEAN, BTHOME_V2_FMT_UINT8},
    {BTHOME_ID_PLUG_BOOLEAN, BTHOME_V2_FMT_UINT8},
    {BTHOME_ID_PRESENCE_BOOLEAN, BTHOME_V2_FMT_UINT8},
    {BTHOME_ID_PROBLEM_BOOLEAN, BTHOME_V2_FMT_UINT8},
    {BTHOME_ID_RUNNING_BOOLEAN, BTHOME_V2_FMT_UINT8},
    {BTHOME_ID_SAFETY_BOOLEAN, BTHOME_V2_FMT_UINT8},
    {BTHOME_ID_SMOKE_BOOLEAN, BTHOME_V2_FMT_UINT8},
    {BTHOME_ID_SOUND_BOOLEAN, BTHOME_V2_FMT_UINT8},
    {BTHOME_ID_TAMPER_BOOLEAN, BTHOME_V2_FMT_UINT8},
    {BTHOME_ID_VIBRATION_BOOLEAN, BTHOME_V2_FMT_UINT8},
    {BTHOME_ID_WINDOW_BOOLEAN, BTHOME_V2_FMT_UINT8},
    {BTHOME_ID_HUMIDITY_1, BTHOME_V2_FMT_UINT8},
    {BTHOME_ID_MOISTURE_1, BTHOME_V2_FMT_UINT8},
    {BTHOME_ID_BUTTON_EVENT, BTHOME_V2_FMT_EVENT},
    {BTHOME_ID_DIMMER_EVENT, BTHOME_V2_FMT_DIMMER_EVENT},
    {BTHOME_ID_COUNT_UINT16, BTHOME_V2_FMT_UINT16},
    {BTHOME_ID_COUNT_UINT32, BTHOME_V2_FMT_UINT32},
    {BTHOME_ID_ROTATION_0_1, BTHOME_V2_FMT_SINT16},
    {BTHOME_ID_DISTANCE_MM, BTHOME_V2_FMT_UINT16},
    {BTHOME_ID_DISTANCE_M_0_1, BTHOME_V2_FMT_UINT16},
    {BTHOME_ID_DURATION_S_0_001, BTHOME_V2_FMT_UINT24},
    {BTHOME_ID_CURRENT_A_0_001, BTHOME_V2_FMT_UINT16},
    {BTHOME_ID_SPEED_MPS_0_01, BTHOME_V2_FMT_UINT16},
    {BTHOME_ID_TEMPERATURE_0_1, BTHOME_V2_FMT_SINT16},
    {BTHOME_ID_UV_INDEX_0_1, BTHOME_V2_FMT_UINT8},
    {BTHOME_ID_VOLUME_L_0_1, BTHOME_V2_FMT_UINT16},
    {BTHOME_ID_VOLUME_ML, BTHOME_V2_FMT_UINT16},
    {BTHOME_ID_VOLUME_FLOW_RATE_0_001, BTHOME_V2_FMT_UINT16},
    {BTHOME_ID_VOLTAGE_0_1, BTHOME_V2_FMT_UINT16},
    {BTHOME_ID_GAS_M3_0_001_UINT24, BTHOME_V2_FMT_UINT24},
    {BTHOME_ID_GAS_M3_0_001_UINT32, BTHOME_V2_FMT_UINT32},
    {BTHOME_ID_ENERGY_KWH_0_001_UINT32, BTHOME_V2_FMT_UINT32},
    {BTHOME_ID_VOLUME_L_0_001, BTHOME_V2_FMT_UINT32},
    {BTHOME_ID_WATER_L_0_001, BTHOME_V2_FMT_UINT32},
    {BTHOME_ID_TIMESTAMP, BTHOME_V2_FMT_UINT32},
    {BTHOME_ID_ACCELERATION_0_001, BTHOME_V2_FMT_UINT16},
    {BTHOME_ID_GYROSCOPE_0_001, BTHOME_V2_FMT_UINT16},
    {BTHOME_ID_TEXT, BTHOME_V2_FMT_TEXT},
    {BTHOME_ID_RAW, BTHOME_V2_FMT_RAW},
    {BTHOME_ID_VOLUME_STORAGE_L_0_001, BTHOME_V2_FMT_UINT32},
    {BTHOME_ID_CONDUCTIVITY, BTHOME_V2_FMT_UINT16},
    {BTHOME_ID_TEMPERATURE_S8_1, BTHOME_V2_FMT_SINT8},
    {BTHOME_ID_TEMPERATURE_S8_0_35, BTHOME_V2_FMT_SINT8},
    {BTHOME_ID_COUNT_SINT8, BTHOME_V2_FMT_SINT8},
    {BTHOME_ID_COUNT_SINT16, BTHOME_V2_FMT_SINT16},
    {BTHOME_ID_COUNT_SINT32, BTHOME_V2_FMT_SINT32},
    {BTHOME_ID_POWER_W_0_01_SINT32, BTHOME_V2_FMT_SINT32},
    {BTHOME_ID_CURRENT_A_0_001_SINT16, BTHOME_V2_FMT_SINT16},
    {BTHOME_ID_DIRECTION_0_01, BTHOME_V2_FMT_UINT16},
    {BTHOME_ID_PRECIPITATION_MM_0_1, BTHOME_V2_FMT_UINT16},
    {BTHOME_ID_CHANNEL, BTHOME_V2_FMT_UINT8},
    {BTHOME_ID_ROTATIONAL_SPEED_RPM, BTHOME_V2_FMT_UINT16},
    {BTHOME_ID_SPEED_MPS_0_000001_SINT32, BTHOME_V2_FMT_SINT32},
    {BTHOME_ID_ACCELERATION_0_000001_SINT32, BTHOME_V2_FMT_SINT32},
    {BTHOME_ID_LIGHT_LEVEL, BTHOME_V2_FMT_UINT8},
    {BTHOME_ID_DEVICE_TYPE_ID, BTHOME_V2_FMT_UINT16},
    {BTHOME_ID_FIRMWARE_VERSION_UINT32, BTHOME_V2_FMT_UINT32},
    {BTHOME_ID_FIRMWARE_VERSION_UINT24, BTHOME_V2_FMT_UINT24},
};

static bool bthome_v2_has_space(const bthome_v2_builder_t *builder, uint8_t extra_len)
{
    return builder && builder->buffer && (builder->length + extra_len <= builder->capacity);
}

static bool bthome_v2_begin_object(bthome_v2_builder_t *builder, uint8_t object_id, uint8_t payload_len)
{
    if(!bthome_v2_has_space(builder, (uint8_t)(payload_len + 1)))
    {
        return false;
    }

    if(builder->length > 3 && object_id < builder->last_object_id)
    {
        return false;
    }

    builder->buffer[builder->length++] = object_id;
    builder->last_object_id = object_id;
    return true;
}

static void bthome_v2_write_le(uint8_t *dst, uint32_t value, uint8_t size)
{
    uint8_t i;

    for(i = 0; i < size; i++)
    {
        dst[i] = (uint8_t)(value & 0xFF);
        value >>= 8;
    }
}

static bool bthome_v2_add_numeric(bthome_v2_builder_t *builder,
                                  uint8_t object_id,
                                  uint32_t value,
                                  uint8_t size)
{
    if(!bthome_v2_begin_object(builder, object_id, size))
    {
        return false;
    }

    bthome_v2_write_le(&builder->buffer[builder->length], value, size);
    builder->length = (uint8_t)(builder->length + size);
    return true;
}

void bthome_v2_init(bthome_v2_builder_t *builder,
                    uint8_t *buffer,
                    uint8_t capacity,
                    bool encrypted,
                    bool trigger_based)
{
    uint8_t device_info = BTHOME_V2_DEVICE_INFO_BASE;

    if(encrypted)
    {
        device_info |= 0x01;
    }

    if(trigger_based)
    {
        device_info |= 0x04;
    }

    builder->buffer = buffer;
    builder->capacity = capacity;
    builder->length = 0;
    builder->last_object_id = 0;

    if(capacity < 3)
    {
        return;
    }

    builder->buffer[builder->length++] = BTHOME_V2_UUID_LSB;
    builder->buffer[builder->length++] = BTHOME_V2_UUID_MSB;
    builder->buffer[builder->length++] = device_info;
}

const uint8_t *bthome_v2_data(const bthome_v2_builder_t *builder)
{
    return builder ? builder->buffer : 0;
}

uint8_t bthome_v2_size(const bthome_v2_builder_t *builder)
{
    return builder ? builder->length : 0;
}

const bthome_v2_object_info_t *bthome_v2_get_object_info(uint8_t object_id)
{
    uint8_t i;

    for(i = 0; i < (uint8_t)(sizeof(bthome_v2_object_table) / sizeof(bthome_v2_object_table[0])); i++)
    {
        if(bthome_v2_object_table[i].object_id == object_id)
        {
            return &bthome_v2_object_table[i];
        }
    }

    return 0;
}

bool bthome_v2_add_object_uint(bthome_v2_builder_t *builder, uint8_t object_id, uint32_t value)
{
    const bthome_v2_object_info_t *info = bthome_v2_get_object_info(object_id);

    if(!info)
    {
        return false;
    }

    switch(info->format)
    {
        case BTHOME_V2_FMT_UINT8:
            return value <= 0xFFU ? bthome_v2_add_numeric(builder, object_id, value, 1) : false;
        case BTHOME_V2_FMT_UINT16:
            return value <= 0xFFFFU ? bthome_v2_add_numeric(builder, object_id, value, 2) : false;
        case BTHOME_V2_FMT_UINT24:
            return value <= 0xFFFFFFUL ? bthome_v2_add_numeric(builder, object_id, value, 3) : false;
        case BTHOME_V2_FMT_UINT32:
            return bthome_v2_add_numeric(builder, object_id, value, 4);
        default:
            return false;
    }
}

bool bthome_v2_add_object_sint(bthome_v2_builder_t *builder, uint8_t object_id, int32_t value)
{
    const bthome_v2_object_info_t *info = bthome_v2_get_object_info(object_id);

    if(!info)
    {
        return false;
    }

    switch(info->format)
    {
        case BTHOME_V2_FMT_SINT8:
            return (value >= -128) && (value <= 127)
                       ? bthome_v2_add_numeric(builder, object_id, (uint32_t)(uint8_t)value, 1)
                       : false;
        case BTHOME_V2_FMT_SINT16:
            return (value >= -32768) && (value <= 32767)
                       ? bthome_v2_add_numeric(builder, object_id, (uint32_t)(uint16_t)value, 2)
                       : false;
        case BTHOME_V2_FMT_SINT24:
            return (value >= -8388608L) && (value <= 8388607L)
                       ? bthome_v2_add_numeric(builder, object_id, (uint32_t)value & 0xFFFFFFUL, 3)
                       : false;
        case BTHOME_V2_FMT_SINT32:
            return bthome_v2_add_numeric(builder, object_id, (uint32_t)value, 4);
        default:
            return false;
    }
}

bool bthome_v2_add_boolean(bthome_v2_builder_t *builder, uint8_t object_id, bool value)
{
    return bthome_v2_add_object_uint(builder, object_id, value ? 1U : 0U);
}

bool bthome_v2_add_packet_id(bthome_v2_builder_t *builder, uint8_t packet_id)
{
    return bthome_v2_add_object_uint(builder, BTHOME_ID_PACKET_ID, packet_id);
}

bool bthome_v2_add_text(bthome_v2_builder_t *builder, const char *text)
{
    uint8_t text_len = 0;
    const char *p = text;

    if(!text)
    {
        return false;
    }

    while(*p++)
    {
        text_len++;
    }

    if(!bthome_v2_begin_object(builder, BTHOME_ID_TEXT, (uint8_t)(text_len + 1)))
    {
        return false;
    }

    builder->buffer[builder->length++] = text_len;
    bthome_v2_write_le(&builder->buffer[builder->length], 0, 0);
    while(text_len--)
    {
        builder->buffer[builder->length++] = (uint8_t)*text++;
    }

    return true;
}

bool bthome_v2_add_raw(bthome_v2_builder_t *builder, const uint8_t *data, uint8_t data_len)
{
    uint8_t i;

    if(!data || !bthome_v2_begin_object(builder, BTHOME_ID_RAW, (uint8_t)(data_len + 1)))
    {
        return false;
    }

    builder->buffer[builder->length++] = data_len;
    for(i = 0; i < data_len; i++)
    {
        builder->buffer[builder->length++] = data[i];
    }

    return true;
}

bool bthome_v2_add_button_event(bthome_v2_builder_t *builder, uint8_t event_id)
{
    if(!bthome_v2_begin_object(builder, BTHOME_ID_BUTTON_EVENT, 1))
    {
        return false;
    }

    builder->buffer[builder->length++] = event_id;
    return true;
}

bool bthome_v2_add_dimmer_event(bthome_v2_builder_t *builder, uint8_t event_id, uint8_t steps)
{
    if(!bthome_v2_begin_object(builder, BTHOME_ID_DIMMER_EVENT, 2))
    {
        return false;
    }

    builder->buffer[builder->length++] = event_id;
    builder->buffer[builder->length++] = steps;
    return true;
}

uint8_t bthome_v2_build_legacy_advertisement(uint8_t *adv,
                                             uint8_t adv_capacity,
                                             const uint8_t *service_data,
                                             uint8_t service_data_len,
                                             const char *local_name,
                                             bool complete_name)
{
    uint8_t i = 0;
    uint8_t name_len = 0;
    const char *p = local_name;

    if(!adv || !service_data || adv_capacity < 3)
    {
        return 0;
    }

    adv[i++] = 0x02;
    adv[i++] = 0x01;
    adv[i++] = 0x06;

    if((uint16_t)i + service_data_len + 2U > adv_capacity)
    {
        return 0;
    }

    if(local_name)
    {
        while(*p++)
        {
            name_len++;
        }

        /* Reserve space for the mandatory BTHome service data first. */
        if((uint16_t)i + name_len + 2U + service_data_len + 2U <= adv_capacity)
        {
            adv[i++] = (uint8_t)(name_len + 1);
            adv[i++] = complete_name ? 0x09 : 0x08;

            while(name_len--)
            {
                adv[i++] = (uint8_t)*local_name++;
            }
        }
    }

    adv[i++] = (uint8_t)(service_data_len + 1);
    adv[i++] = 0x16;
    bthome_v2_write_le(&adv[i], 0, 0);

    while(service_data_len--)
    {
        adv[i++] = *service_data++;
    }

    return i;
}
