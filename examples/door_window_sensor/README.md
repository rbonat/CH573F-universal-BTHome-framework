# Door/Window Sensor

Samodzielny projekt MounRiver Studio dla CH573F. Publikuje BTHome v2 po zmianie stanu kontaktronu oraz okresowo jako heartbeat RTC.

## Sprzęt

- Kontaktron: PA9, wejście pływające. Należy zastosować zewnętrzny rezystor pull-up albo pull-down; styk NC/NO łączy wejście odpowiednio z VCC albo GND.
- Bateria: przełączana masa PA5, wejście ADC PA4 / ADC0.
- LED stanu: PA8, aktywny stan niski.

## Ramka BTHome

`packet_id`, `battery`, `voltage` (0,001 V), `opening`, `battery_low`. Ramka jest niepołączeniowa (`ADV_NONCONN_IND`) i oznaczona jako oparta na zdarzeniu BTHome. Nazwa urządzenia jest dodawana tylko przy dostępnej przestrzeni ramki.

## Praca i wybudzanie

Wzorzec zdarzeniowy: PA9 wybudza układ na obu zboczach; procedura ISR uzbraja przeciwne zbocze. Po filtracji `DOOR_DEBOUNCE_TMOS` nowa ramka powstaje wyłącznie, gdy stabilny stan się zmieni. RTC budzi układ najpóźniej po `DOOR_STATUS_PERIOD_S`, aby wysłać heartbeat ze stanem i baterią.

Po starcie, zmianie stanu lub heartbeatcie reklama jest włączana z `ADV_FAST_INTERVAL`, następnie przełączana na `ADV_SLOW_INTERVAL` po `ADV_FAST_PHASE_TMOS`, po czym wyłączana po `ADV_SLOW_PHASE_TMOS`. Podczas bezczynności źródłami wybudzenia pozostają tylko GPIO i RTC. Wszystkie wartości TMOS/GAP mają jednostkę 625 µs; heartbeat RTC ma taktowanie 32,768 kHz.

## Konfiguracja i kalibracja

`config/app_config.h` zawiera piny, poziom `SENSOR_OPEN_LEVEL`, czasy debounce/reklamy/heartbeat, `BTHOME_NAME` i parametry baterii. Dostosuj `SENSOR_OPEN_LEVEL` do elektrycznego znaczenia stanu otwartego oraz `VBAT_CAL_POINT*` do dzielnika napięcia.

## Budowanie

Zaimportuj `examples/door_window_sensor` jako istniejący projekt i zbuduj konfigurację `obj`. Home Assistant wymaga pasywnego odbioru reklam BTHome.
