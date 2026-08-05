# soft_i2c

Minimalny, blokujacy driver software I2C dla CH57x.

## Cechy

- brak malloc, przerwan i zadan w tle
- jedna konfiguracja na uchwyt `SoftI2cBus_t`
- piny wybierane jako port `SOFT_I2C_PORT_A/B` + `GPIO_Pin_x`
- predkosc w `speed_hz`, wartosc `0` oznacza `SOFT_I2C_DEFAULT_SPEED_HZ`
- linie emulowane jako open-drain: niski = output low, wysoki = input pull-up

## Przyklad

```c
static SoftI2cBus_t i2c;

static const SoftI2cConfig_t i2c_cfg = {
    { SOFT_I2C_PORT_A, GPIO_Pin_12 },
    { SOFT_I2C_PORT_A, GPIO_Pin_13 },
    100000UL,
};

SoftI2c_Init(&i2c, &i2c_cfg);
SoftI2c_Write(&i2c, 0x38, data, len);
```

## API

```c
void SoftI2c_Init(SoftI2cBus_t *bus, const SoftI2cConfig_t *cfg);
void SoftI2c_Deinit(SoftI2cBus_t *bus);

SoftI2cStatus_t SoftI2c_Write(SoftI2cBus_t *bus, uint8_t addr_7bit, const uint8_t *data, uint16_t len);
SoftI2cStatus_t SoftI2c_Read(SoftI2cBus_t *bus, uint8_t addr_7bit, uint8_t *data, uint16_t len);
SoftI2cStatus_t SoftI2c_WriteRead(SoftI2cBus_t *bus,
                                  uint8_t addr_7bit,
                                  const uint8_t *write_data,
                                  uint16_t write_len,
                                  uint8_t *read_data,
                                  uint16_t read_len);
```

## Konfiguracja

Makra domyslne mozna nadpisac w `app_config.h` przed dolaczeniem `default_config.h`.

```c
#define SOFT_I2C_DEFAULT_SPEED_HZ             100000UL
#define SOFT_I2C_CLOCK_STRETCH_TIMEOUT_US     1000U
```
