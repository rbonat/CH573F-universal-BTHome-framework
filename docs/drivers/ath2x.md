\# ATH2X Driver Notes



\## Overview



ATH2X is a digital temperature and humidity sensor communicating over I²C.



Target use case:



\* battery powered BTHome sensors

\* one-shot measurements only

\* deep sleep between measurements

\* minimal code size

\* blocking implementation is acceptable



\## Device Information



I²C address:



```c

\#define ATH2X\\_I2C\\_ADDR 0x38

```



Supply voltage:



```text

2.0V - 5.5V

```



\## Status Register



Read status:



```text

Command: 0x71

Read: 1 byte

```



Status bits:



```text

Bit\[7] BUSY

&#x20; 1 = measurement in progress

&#x20; 0 = idle



Bit\[3] CALIBRATED

&#x20; 1 = calibrated and ready

```



Sensor is ready when:



```c

(status \& 0x18) == 0x18

```



\## Initialization



After power-up:



```text

wait >= 40 ms

```



Read status.



If calibration bit is not set:



```text

Write:

0xBE 0x08 0x00

```



Then:



```text

wait >= 10 ms

```



Initialization sequence only needs to be executed when required.



\## Trigger Measurement



Start one-shot measurement:



```text

Write:

0xAC 0x33 0x00

```



Wait:



```text

80 ms

```



or poll BUSY bit until cleared.



\## Read Measurement



Read 7 bytes:



```text

Byte0 = status



Byte1

Byte2

Byte3



= 20-bit humidity raw value



Byte3

Byte4

Byte5



= 20-bit temperature raw value



Byte6 = CRC

```



For minimal implementation CRC may be ignored.



\## Data Extraction



Humidity raw:



```c

humidity\_raw =

&#x20;   ((uint32\_t)buf\[1] << 12) |

&#x20;   ((uint32\_t)buf\[2] << 4)  |

&#x20;   ((uint32\_t)buf\[3] >> 4);

```



Temperature raw:



```c

temperature\_raw =

&#x20;   (((uint32\_t)buf\[3] \& 0x0F) << 16) |

&#x20;   ((uint32\_t)buf\[4] << 8) |

&#x20;   ((uint32\_t)buf\[5]);

```



\## Conversion



Humidity:



```c

humidity =

&#x20;   (humidity\_raw \* 100.0f) / 1048576.0f;

```



Temperature:



```c

temperature =

&#x20;   ((temperature\_raw \* 200.0f) / 1048576.0f) - 50.0f;

```



1048576 = 2^20



\## Recommended Fixed Point Output



Avoid floating point.



Store:



```c

int16\_t temperature\_x100;

uint16\_t humidity\_x100;

```



Examples:



```text

2345  = 23.45 °C

5678  = 56.78 %RH

```



Fixed-point conversion:



```c

temperature\_x100 =

&#x20;   ((int32\_t)temperature\_raw \* 20000L) / 1048576L - 5000;



humidity\_x100 =

&#x20;   ((uint32\_t)humidity\_raw \* 10000UL) / 1048576UL;

```



\## Minimal Driver API



```c

typedef struct {

&#x20;   int16\_t temperature\_x100;

&#x20;   uint16\_t humidity\_x100;

} ath2x\\_data\\_t;



bool ath2x\\_init(void);



bool ath2x\\_read(ath2x\\_data\\_t \*data);

```



\## Power Optimization



For battery-powered BTHome devices:



1\. Wake from deep sleep.

2\. Initialize I²C.

3\. Read ATH2X.

4\. Encode BTHome payload.

5\. Advertise BLE.

6\. Return to deep sleep.



Only one measurement per wake cycle.



No continuous mode.

No periodic polling.

No background tasks.



\## Codex Requirements



Generate:



```text

drivers/ath2x/ath2x.h

drivers/ath2x/ath2x.c

```



Constraints:



```text

\- C99

\- no malloc

\- no float

\- no RTOS

\- no interrupts

\- blocking implementation

\- use existing project I2C HAL

\- optimize for code size

\- optimize for low power operation

```



