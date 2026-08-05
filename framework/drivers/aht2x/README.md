# aht2x

Lekki driver dla czujnikow temperatury i wilgotnosci AHT20/AHT21.

Driver uzywa `soft_i2c`, wykonuje pojedynczy blokujacy pomiar i zwraca wynik w fixed-point:

- `temperature_x100`: 2345 oznacza 23.45 C
- `humidity_x100`: 5678 oznacza 56.78 %RH

## Przyklad

```c
aht2x_data_t data;

if(aht2x_init() && aht2x_read(&data))
{
    /* data.temperature_x100, data.humidity_x100 */
}
```

## Konfiguracja pinow

Domyslne piny mozna nadpisac w `app_config.h`.

```c
#define AHT2X_I2C_SDA_PORT   SOFT_I2C_PORT_A
#define AHT2X_I2C_SDA_PIN    GPIO_Pin_12
#define AHT2X_I2C_SCL_PORT   SOFT_I2C_PORT_A
#define AHT2X_I2C_SCL_PIN    GPIO_Pin_13
#define AHT2X_I2C_SPEED_HZ   100000UL
```

Adres domyslny:

```c
#define AHT2X_I2C_ADDR       0x38U
```

## API

```c
typedef struct
{
    int16_t temperature_x100;
    uint16_t humidity_x100;
} aht2x_data_t;

bool aht2x_init(void);
bool aht2x_init_with_config(const SoftI2cConfig_t *i2c_config);
bool aht2x_read(aht2x_data_t *data);
bool aht2x_read_status(uint8_t *status);
```

## Uwagi

- brak float, malloc, przerwan i zadan w tle
- CRC z ramki pomiarowej nie jest sprawdzane
- `aht2x_read()` czeka `AHT2X_MEASURE_DELAY_MS` po wyzwoleniu pomiaru
- `aht2x_init()` czeka `AHT2X_POWER_ON_DELAY_MS` po starcie zasilania
