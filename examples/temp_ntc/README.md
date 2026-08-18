# NTC Temperature Sensor

Samodzielny projekt MounRiver Studio dla CH573F. Cyklicznie publikuje BTHome v2: temperaturę NTC, napięcie i procent baterii oraz `battery_low`.

## Sprzęt

- NTC: przełączana masa PA5, wejście ADC PA15 / ADC5.
- Bateria: przełączana masa PA5, wejście ADC PA4 / ADC0.
- LED stanu: PA8, aktywny stan niski.

## Ramka BTHome

`packet_id`, `battery`, `voltage` (0,001 V), `battery_low`, `temperature` (0,1 °C). Reklama jest niepołączeniowa (`ADV_NONCONN_IND`); nazwa urządzenia jest dodawana tylko przy dostępnej przestrzeni ramki.

## Praca i wybudzanie

Wzorzec czasowy: pierwszy pomiar następuje po 10 ms TMOS, następne co `TEMP_NTC_PERIOD_MS`. Pomiędzy pomiarami bieżąca ramka jest reklamowana z interwałem `ADV_INTERVAL` (jednostka 625 µs).

## Konfiguracja i kalibracja

`config/app_config.h` zawiera piny, okresy, liczbę próbek i `BTHOME_NAME`. Ustaw `VBAT_CAL_POINT*` dla kalibracji baterii. `NTC_CAL_RAW` i `NTC_CAL_TEMP_X10` definiują punkt odniesienia NTC blisko temperatury pracy; sterownik zachowuje nieliniową charakterystykę LUT.

## Budowanie

Zaimportuj `examples/temp_ntc` jako istniejący projekt i zbuduj konfigurację `obj`. Home Assistant wymaga pasywnego odbioru reklam BTHome.
