# Temperature and Humidity Sensor

Samodzielny projekt MounRiver Studio dla CH573F. Cyklicznie publikuje BTHome v2: temperaturę i wilgotność AHT, napięcie i procent baterii oraz `battery_low`.

## Sprzęt

- AHT20/AHT21: software I²C, SDA PB13 i SCL PB12.
- Bateria: przełączana masa PA5, wejście ADC PA4 / ADC0.
- LED stanu: PA8, aktywny stan niski.

## Ramka BTHome

`packet_id`, `battery`, `temperature` (0,01 °C), `humidity` (0,01 %RH), `voltage` (0,001 V), `battery_low`. Przy błędzie odczytu AHT ramka zawiera wyłącznie dane baterii. Reklama jest niepołączeniowa (`ADV_NONCONN_IND`); nazwa urządzenia jest dodawana tylko przy dostępnej przestrzeni ramki.

## Praca i wybudzanie

Wzorzec czasowy: po inicjalizacji AHT pierwszy pomiar następuje po 10 ms TMOS, następne co `TEMP_HYGRO_PERIOD_MS`. Magistrala I²C jest zwalniana po inicjalizacji i każdym odczycie. Bieżąca ramka jest reklamowana z interwałem `ADV_INTERVAL` (jednostka 625 µs).

## Konfiguracja i kalibracja

`config/app_config.h` zawiera piny I²C, prędkość `AHT2X_I2C_SPEED_HZ`, okresy, `BTHOME_NAME` i parametry baterii. Ustaw `VBAT_CAL_POINT*` do zastosowanego dzielnika. Sterownik AHT zwraca wartości fixed-point bez zmiennoprzecinkowej arytmetyki.

## Budowanie

Zaimportuj `examples/temp_hygro` jako istniejący projekt i zbuduj konfigurację `obj`. Home Assistant wymaga pasywnego odbioru reklam BTHome.
