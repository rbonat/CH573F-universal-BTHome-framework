# Soil Sensor

Samodzielny projekt MounRiver Studio dla CH573F. Cyklicznie publikuje BTHome v2: wilgotność gleby, temperaturę NTC, napięcie i procent baterii oraz `battery_low`.

## Sprzęt

- Sonda wilgotności: PWM PB14, wejście ADC PA9 / ADC13.
- NTC: przełączana masa PA5, wejście ADC PA15 / ADC5.
- Bateria: przełączana masa PA5, wejście ADC PA4 / ADC0.
- LED stanu: PA8, aktywny stan niski.

## Ramka BTHome

`packet_id`, `battery`, `voltage` (0,001 V), `moisture` (0,01 %), `battery_low`, `temperature` (0,1 °C). Reklama jest niepołączeniowa (`ADV_NONCONN_IND`); nazwa urządzenia jest dodawana tylko przy dostępnej przestrzeni ramki.

## Praca i wybudzanie

Wzorzec czasowy: pierwszy pomiar następuje po 10 ms TMOS, następne co `SOIL_PERIOD_MS`. Pomiędzy pomiarami bieżąca ramka jest reklamowana z interwałem `ADV_INTERVAL` (jednostka 625 µs).

## Konfiguracja i kalibracja

`config/app_config.h` zawiera piny, okresy, liczbę próbek i `BTHOME_NAME`. Ustaw `VBAT_CAL_POINT*` dla kalibracji baterii, `NTC_CAL_RAW`/`NTC_CAL_TEMP_X10` dla jednopunktowej kalibracji NTC oraz `SOIL_RAW_DRY`/`SOIL_RAW_WET` dla zakresu sondy. Opcjonalna kompensacja temperatury jest sterowana przez `SOIL_TEMPERATURE_CORRECTION_ENABLED`, `SOIL_TEMP_REF_X10`, `SOIL_TEMP_GAIN_X100` i `SOIL_TEMP_OFFSET_X100`.

## Budowanie

Zaimportuj `examples/soil_sensor` jako istniejący projekt i zbuduj konfigurację `obj`. Home Assistant wymaga pasywnego odbioru reklam BTHome.
