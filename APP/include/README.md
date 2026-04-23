# BTHome V2 Builder

Ta biblioteka sluzy tylko do budowy komunikatow BTHome V2.
Nie zawiera logiki BLE, advertisingu, sleep ani integracji z SDK.

## Pliki

- `bthome_v2.h` - publiczne API, enumy `object id` i deklaracje funkcji
- `bthone_encoder.c` - implementacja buildera BTHome V2

## Co potrafi

- budowa `service data` BTHome V2
- dodawanie obiektow liczbowych unsigned i signed
- dodawanie obiektow bool
- dodawanie `packet id`
- dodawanie `text`, `raw`, `button event`, `dimmer event`
- budowa kompletnego legacy BLE advertisement z opcjonalna nazwa urzadzenia

## Czego nie robi

- nie wysyla reklam BLE
- nie zarzadza radiem ani interwalami advertising
- nie czyta sensorow
- nie utrzymuje stanu aplikacji poza buforem buildera

## Podstawowe uzycie

```c
#include "bthome_v2.h"

uint8_t service_data[20];
uint8_t adv[31];
uint8_t len;
static uint8_t packet_id;

bthome_v2_builder_t bthome;

bthome_v2_init(&bthome, service_data, sizeof(service_data), false, false);
bthome_v2_add_packet_id(&bthome, packet_id++);
bthome_v2_add_object_uint(&bthome, BTHOME_ID_BATTERY_PERCENT, 87);
bthome_v2_add_object_uint(&bthome, BTHOME_ID_VOLTAGE_0_001, 4012);
bthome_v2_add_object_uint(&bthome, BTHOME_ID_MOISTURE_0_01, 5234);

len = bthome_v2_build_legacy_advertisement(adv,
                                           sizeof(adv),
                                           bthome_v2_data(&bthome),
                                           bthome_v2_size(&bthome),
                                           "BT_SOIL_1",
                                           true);
```

Potem `adv` i `len` przekazujesz do swojego SDK BLE.

## Kolejnosc obiektow

BTHome wymaga rosnacej kolejnosci `object id`.
Builder tego pilnuje i odrzuci obiekt dodany poza kolejnoscia.

Przyklad poprawnej kolejnosci:

- `BTHOME_ID_PACKET_ID`
- `BTHOME_ID_BATTERY_PERCENT`
- `BTHOME_ID_VOLTAGE_0_001`
- `BTHOME_ID_MOISTURE_0_01`

## Uwagi o buforach

- bufor `service_data` musi pomiescic dane BTHome
- bufor `adv` dla legacy BLE ma maksymalnie 31 bajtow
- jesli dodajesz nazwe urzadzenia, zostaje mniej miejsca na dane

## Najwazniejsze funkcje

- `bthome_v2_init(...)` - inicjalizacja buildera
- `bthome_v2_add_packet_id(...)` - dodanie packet id
- `bthome_v2_add_object_uint(...)` - dodanie obiektu unsigned
- `bthome_v2_add_object_sint(...)` - dodanie obiektu signed
- `bthome_v2_add_boolean(...)` - dodanie obiektu bool
- `bthome_v2_build_legacy_advertisement(...)` - zlozenie gotowego legacy advertisement

## Format danych

ID obiektow i formaty sa zdefiniowane w `bthome_v2.h`.
Biblioteka udostepnia wspolne enumy dla najpopularniejszych typow BTHome V2.

