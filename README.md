# CH57x BTHome Framework

Modułowy framework firmware BTHome v2 dla WCH CH57x. Każdy katalog w `examples/` jest niezależnym projektem MounRiver Studio dla CH573F.

## Zakres

- BTHome v2 w niepołączeniowych ramkach BLE legacy advertising.
- Kodowanie obiektów, nazwa urządzenia i licznik pakietów.
- Pomiary ADC z przełączanym dzielnikiem oraz histereza `battery_low`.
- Uśpienie RTC i wybudzanie GPIO.
- Konfiguracja pinów, kalibracji i harmonogramu per przykład.

## Architektura

```text
framework/
|-- core/               enkoder BTHome v2
|-- ble/                inicjalizacja BLE i profile wspólne
|-- hal/                MCU, RTC, sen, LED i wejścia
`-- drivers/            sterowniki pomiarowe
examples/<nazwa>/       aplikacja, konfiguracja, kalibracja i projekt IDE
vendor/ch57x_sdk/       niezmodyfikowany SDK WCH i pliki linkera
```

Kod aplikacji wyłącznie orkiestruje pomiar, harmonogram i publikację. Sterowniki nie tworzą zadań ani przerwań; wykonują synchroniczny pomiar i zwracają dane całkowitoliczbowe.

## Sterowniki

- `battery` — ADC dzielnika, kalibracja dwupunktowa, napięcie, procent i histereza niskiej baterii.
- `ntc_temp` — ADC NTC, filtr średniej kroczącej, LUT i kalibracja jednopunktowa.
- `soil_sensor` — PWM/ADC sondy pojemnościowej, filtracja oraz opcjonalna kompensacja temperatury.
- `soft_i2c` — blokujący programowy I²C typu open-drain.
- `aht2x` — blokujący odczyt AHT20/AHT21 przez `soft_i2c`, temperatura i wilgotność fixed-point.

## Wzorce pracy

| Wzorzec | Przykłady | Wybudzanie | Publikacja |
| --- | --- | --- | --- |
| Czasowy | `soil_sensor`, `temp_ntc`, `temp_hygro` | TMOS po starcie i co okres aplikacji | Bieżąca ramka pozostaje reklamowana niepołączeniowo; interwał BLE definiuje `ADV_INTERVAL`. |
| Zdarzeniowy | `door_window_sensor` | Zbocze GPIO kontaktronu; RTC wymusza heartbeat | Po starcie, zmianie stabilnego stanu i heartbeatcie tworzona jest nowa ramka. Reklama przechodzi przez fazę szybką i wolną, po czym jest wyłączana. |

Wartości harmonogramu są definiowane w `config/app_config.h`. Czas TMOS/GAP jest podawany w jednostkach 625 µs; okres RTC kontaktronu — w tickach 32,768 kHz. `door_window_sensor` przełącza kierunek przerwania po każdym zboczu, więc obsługuje zarówno otwarcie, jak i zamknięcie.

## Przykłady

| Projekt | Dane BTHome | Źródło pomiaru / zdarzenia |
| --- | --- | --- |
| `soil_sensor` | wilgotność gleby, temperatura NTC, napięcie i procent baterii, `battery_low` | sonda PWM/ADC, NTC, ADC baterii |
| `temp_ntc` | temperatura NTC, napięcie i procent baterii, `battery_low` | NTC i ADC baterii |
| `temp_hygro` | temperatura, wilgotność, napięcie i procent baterii, `battery_low` | AHT20/AHT21 po software I²C, ADC baterii |
| `door_window_sensor` | stan otwarcia, napięcie i procent baterii, `battery_low` | kontaktron GPIO, ADC baterii |

Szczegóły sprzętu, konfiguracji, ramki i trybu pracy znajdują się w README danego przykładu.

## Budowanie

1. Zaimportuj wybrany katalog `examples/<nazwa>` jako istniejący projekt w MounRiver Studio.
2. Zbuduj konfigurację `obj`.

Projekty używają względnych linked resources do `framework/` i `vendor/`; import z czystego klonu nie wymaga ścieżek lokalnych. Konfigurację urządzenia zmieniaj w `examples/<nazwa>/config/app_config.h`.

## Integracja

Ramki są BTHome v2 w niepołączeniowej reklamie BLE. Home Assistant wymaga odbiornika Bluetooth lub Bluetooth Proxy odbierającego reklamy pasywne. Nazwa `BTHOME_NAME` jest dodawana tylko, jeśli mieści się w 31-bajtowej ramce BLE legacy.

## Obsługiwane układy

- CH573F — obsługiwany przez wszystkie bieżące przykłady.
- CH582 — planowany; rozdzielenie aplikacji, frameworka i SDK przygotowuje osobny profil docelowy.
