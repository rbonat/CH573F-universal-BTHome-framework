#include "CONFIG.h"
#include "soft_i2c.h"

static bool SoftI2c_IsSinglePin(uint32_t pin)
{
    return (pin != 0U) && ((pin & (pin - 1U)) == 0U);
}

static bool SoftI2c_IsValidPin(const SoftI2cPin_t *pin)
{
    return (pin != 0) &&
           ((pin->port == SOFT_I2C_PORT_A) || (pin->port == SOFT_I2C_PORT_B)) &&
           SoftI2c_IsSinglePin(pin->pin);
}

static bool SoftI2c_IsSamePin(const SoftI2cPin_t *a, const SoftI2cPin_t *b)
{
    return (a->port == b->port) && (a->pin == b->pin);
}

static bool SoftI2c_IsValidConfig(const SoftI2cConfig_t *cfg)
{
    return (cfg != 0) &&
           SoftI2c_IsValidPin(&cfg->sda) &&
           SoftI2c_IsValidPin(&cfg->scl) &&
           !SoftI2c_IsSamePin(&cfg->sda, &cfg->scl);
}

static bool SoftI2c_IsValidBus(const SoftI2cBus_t *bus)
{
    return (bus != 0) &&
           bus->initialized &&
           SoftI2c_IsValidConfig(&bus->cfg);
}

static void SoftI2c_PinInputPullup(const SoftI2cPin_t *pin)
{
    if(pin->port == SOFT_I2C_PORT_A)
    {
        GPIOA_ModeCfg(pin->pin, GPIO_ModeIN_PU);
    }
    else
    {
        GPIOB_ModeCfg(pin->pin, GPIO_ModeIN_PU);
    }
}

static void SoftI2c_PinDriveLow(const SoftI2cPin_t *pin)
{
    if(pin->port == SOFT_I2C_PORT_A)
    {
        GPIOA_ResetBits(pin->pin);
        GPIOA_ModeCfg(pin->pin, GPIO_ModeOut_PP_5mA);
    }
    else
    {
        GPIOB_ResetBits(pin->pin);
        GPIOB_ModeCfg(pin->pin, GPIO_ModeOut_PP_5mA);
    }
}

static bool SoftI2c_PinRead(const SoftI2cPin_t *pin)
{
    if(pin->port == SOFT_I2C_PORT_A)
    {
        return GPIOA_ReadPortPin(pin->pin) != 0U;
    }

    return GPIOB_ReadPortPin(pin->pin) != 0U;
}

static void SoftI2c_Delay(const SoftI2cBus_t *bus)
{
    mDelayuS(bus->half_period_us);
}

static void SoftI2c_SetSda(const SoftI2cBus_t *bus, bool high)
{
    if(high)
    {
        SoftI2c_PinInputPullup(&bus->cfg.sda);
    }
    else
    {
        SoftI2c_PinDriveLow(&bus->cfg.sda);
    }
}

static void SoftI2c_SetSclLow(const SoftI2cBus_t *bus)
{
    SoftI2c_PinDriveLow(&bus->cfg.scl);
}

static SoftI2cStatus_t SoftI2c_SetSclHigh(const SoftI2cBus_t *bus)
{
    uint32_t timeout = SOFT_I2C_CLOCK_STRETCH_TIMEOUT_US;

    SoftI2c_PinInputPullup(&bus->cfg.scl);
    while(!SoftI2c_PinRead(&bus->cfg.scl))
    {
        if(timeout == 0U)
        {
            return SOFT_I2C_ERR_TIMEOUT;
        }

        timeout--;
        mDelayuS(1);
    }

    SoftI2c_Delay(bus);
    return SOFT_I2C_OK;
}

static SoftI2cStatus_t SoftI2c_Start(SoftI2cBus_t *bus)
{
    SoftI2cStatus_t status;

    SoftI2c_SetSda(bus, true);
    status = SoftI2c_SetSclHigh(bus);
    if(status != SOFT_I2C_OK)
    {
        return status;
    }

    if(!SoftI2c_PinRead(&bus->cfg.sda))
    {
        return SOFT_I2C_ERR_BUS;
    }

    SoftI2c_SetSda(bus, false);
    SoftI2c_Delay(bus);
    SoftI2c_SetSclLow(bus);
    SoftI2c_Delay(bus);
    return SOFT_I2C_OK;
}

static SoftI2cStatus_t SoftI2c_Stop(SoftI2cBus_t *bus)
{
    SoftI2cStatus_t status;

    SoftI2c_SetSda(bus, false);
    SoftI2c_Delay(bus);
    status = SoftI2c_SetSclHigh(bus);
    if(status != SOFT_I2C_OK)
    {
        return status;
    }

    SoftI2c_SetSda(bus, true);
    SoftI2c_Delay(bus);

    return SoftI2c_PinRead(&bus->cfg.sda) ? SOFT_I2C_OK : SOFT_I2C_ERR_BUS;
}

static SoftI2cStatus_t SoftI2c_WriteByte(SoftI2cBus_t *bus, uint8_t value)
{
    SoftI2cStatus_t status;

    for(uint8_t mask = 0x80U; mask != 0U; mask >>= 1)
    {
        SoftI2c_SetSda(bus, (value & mask) != 0U);
        SoftI2c_Delay(bus);
        status = SoftI2c_SetSclHigh(bus);
        if(status != SOFT_I2C_OK)
        {
            return status;
        }
        SoftI2c_SetSclLow(bus);
        SoftI2c_Delay(bus);
    }

    SoftI2c_SetSda(bus, true);
    SoftI2c_Delay(bus);
    status = SoftI2c_SetSclHigh(bus);
    if(status != SOFT_I2C_OK)
    {
        return status;
    }

    status = SoftI2c_PinRead(&bus->cfg.sda) ? SOFT_I2C_ERR_NACK : SOFT_I2C_OK;
    SoftI2c_SetSclLow(bus);
    SoftI2c_Delay(bus);
    return status;
}

static SoftI2cStatus_t SoftI2c_ReadByte(SoftI2cBus_t *bus, uint8_t *value, bool ack)
{
    SoftI2cStatus_t status;
    uint8_t byte = 0;

    SoftI2c_SetSda(bus, true);

    for(uint8_t i = 0; i < 8U; i++)
    {
        byte <<= 1;
        status = SoftI2c_SetSclHigh(bus);
        if(status != SOFT_I2C_OK)
        {
            return status;
        }
        if(SoftI2c_PinRead(&bus->cfg.sda))
        {
            byte |= 1U;
        }
        SoftI2c_SetSclLow(bus);
        SoftI2c_Delay(bus);
    }

    SoftI2c_SetSda(bus, !ack);
    SoftI2c_Delay(bus);
    status = SoftI2c_SetSclHigh(bus);
    if(status != SOFT_I2C_OK)
    {
        SoftI2c_SetSda(bus, true);
        return status;
    }

    SoftI2c_SetSclLow(bus);
    SoftI2c_SetSda(bus, true);
    SoftI2c_Delay(bus);
    *value = byte;
    return SOFT_I2C_OK;
}

static SoftI2cStatus_t SoftI2c_WriteAddress(SoftI2cBus_t *bus, uint8_t addr_7bit, bool read)
{
    if(addr_7bit > 0x7FU)
    {
        return SOFT_I2C_ERR_PARAM;
    }

    return SoftI2c_WriteByte(bus, (uint8_t)((addr_7bit << 1) | (read ? 1U : 0U)));
}

void SoftI2c_Init(SoftI2cBus_t *bus, const SoftI2cConfig_t *cfg)
{
    uint32_t speed_hz;
    uint32_t half_period_us;

    if(bus == 0)
    {
        return;
    }

    bus->initialized = false;

    if(!SoftI2c_IsValidConfig(cfg))
    {
        return;
    }

    speed_hz = (cfg->speed_hz == 0U) ? SOFT_I2C_DEFAULT_SPEED_HZ : cfg->speed_hz;
    if(speed_hz == 0U)
    {
        speed_hz = 1U;
    }

    half_period_us = 500000UL / speed_hz;
    if(half_period_us == 0U)
    {
        half_period_us = 1U;
    }
    else if(half_period_us > 65535UL)
    {
        half_period_us = 65535UL;
    }

    bus->cfg = *cfg;
    bus->cfg.speed_hz = speed_hz;
    bus->half_period_us = (uint16_t)half_period_us;
    bus->initialized = true;

    SoftI2c_SetSda(bus, true);
    SoftI2c_PinInputPullup(&bus->cfg.scl);
    SoftI2c_Delay(bus);
}

void SoftI2c_Deinit(SoftI2cBus_t *bus)
{
    if(bus == 0)
    {
        return;
    }

    if(SoftI2c_IsValidPin(&bus->cfg.sda))
    {
        SoftI2c_PinInputPullup(&bus->cfg.sda);
    }
    if(SoftI2c_IsValidPin(&bus->cfg.scl))
    {
        SoftI2c_PinInputPullup(&bus->cfg.scl);
    }

    bus->initialized = false;
}

SoftI2cStatus_t SoftI2c_Write(SoftI2cBus_t *bus, uint8_t addr_7bit, const uint8_t *data, uint16_t len)
{
    SoftI2cStatus_t status;
    SoftI2cStatus_t stop_status;

    if(!SoftI2c_IsValidBus(bus) || ((data == 0) && (len != 0U)))
    {
        return SOFT_I2C_ERR_PARAM;
    }

    status = SoftI2c_Start(bus);
    if(status == SOFT_I2C_OK)
    {
        status = SoftI2c_WriteAddress(bus, addr_7bit, false);
    }
    for(uint16_t i = 0; (status == SOFT_I2C_OK) && (i < len); i++)
    {
        status = SoftI2c_WriteByte(bus, data[i]);
    }

    stop_status = SoftI2c_Stop(bus);
    if(status == SOFT_I2C_OK)
    {
        status = stop_status;
    }

    return status;
}

SoftI2cStatus_t SoftI2c_Read(SoftI2cBus_t *bus, uint8_t addr_7bit, uint8_t *data, uint16_t len)
{
    SoftI2cStatus_t status;
    SoftI2cStatus_t stop_status;

    if(!SoftI2c_IsValidBus(bus) || ((data == 0) && (len != 0U)))
    {
        return SOFT_I2C_ERR_PARAM;
    }

    status = SoftI2c_Start(bus);
    if(status == SOFT_I2C_OK)
    {
        status = SoftI2c_WriteAddress(bus, addr_7bit, true);
    }
    for(uint16_t i = 0; (status == SOFT_I2C_OK) && (i < len); i++)
    {
        status = SoftI2c_ReadByte(bus, &data[i], i < (uint16_t)(len - 1U));
    }

    stop_status = SoftI2c_Stop(bus);
    if(status == SOFT_I2C_OK)
    {
        status = stop_status;
    }

    return status;
}

SoftI2cStatus_t SoftI2c_WriteRead(SoftI2cBus_t *bus,
                                  uint8_t addr_7bit,
                                  const uint8_t *write_data,
                                  uint16_t write_len,
                                  uint8_t *read_data,
                                  uint16_t read_len)
{
    SoftI2cStatus_t status;
    SoftI2cStatus_t stop_status;

    if(!SoftI2c_IsValidBus(bus) ||
       ((write_data == 0) && (write_len != 0U)) ||
       ((read_data == 0) && (read_len != 0U)))
    {
        return SOFT_I2C_ERR_PARAM;
    }

    if(read_len == 0U)
    {
        return SoftI2c_Write(bus, addr_7bit, write_data, write_len);
    }
    if(write_len == 0U)
    {
        return SoftI2c_Read(bus, addr_7bit, read_data, read_len);
    }

    status = SoftI2c_Start(bus);
    if(status == SOFT_I2C_OK)
    {
        status = SoftI2c_WriteAddress(bus, addr_7bit, false);
    }
    for(uint16_t i = 0; (status == SOFT_I2C_OK) && (i < write_len); i++)
    {
        status = SoftI2c_WriteByte(bus, write_data[i]);
    }
    if(status == SOFT_I2C_OK)
    {
        status = SoftI2c_Start(bus);
    }
    if(status == SOFT_I2C_OK)
    {
        status = SoftI2c_WriteAddress(bus, addr_7bit, true);
    }
    for(uint16_t i = 0; (status == SOFT_I2C_OK) && (i < read_len); i++)
    {
        status = SoftI2c_ReadByte(bus, &read_data[i], i < (uint16_t)(read_len - 1U));
    }

    stop_status = SoftI2c_Stop(bus);
    if(status == SOFT_I2C_OK)
    {
        status = stop_status;
    }

    return status;
}
