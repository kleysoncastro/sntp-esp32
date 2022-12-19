## SNTP para esp32, platformIO, ESP-IDE

### Para usar essa lib, basta incluir no projeto

- PlatformIO na past `lib`
- ESP-IDE qualquer pasta.

## Exemplo de uso

```c
#include "sntp.hpp"

sntp time_sntp; //

// main, setup, app_main
time_sntp.update_date_time();

Serial.printf("Hora %d:%d:%d ", time_sntp.get_hour(), time_sntp.get_min(), time_sntp.get_seg());


```
