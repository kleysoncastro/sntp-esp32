#include <stdio.h>
#include "sntp.hpp"

static const char *TAG_ntp = "--- NTP ---";

sntp::sntp()
{
}

sntp::~sntp()
{
}

// Atualiza data e hora  via sntp se o rtc interno estiver dezatualizado
void sntp::update_date_time()
{

    time(&now);
    localtime_r(&now, &timeinfo);
    // Is time set? If not, tm_year will be (1970 - 1900).
    if (timeinfo.tm_year < (2016 - 1900))
    {
        ESP_LOGI(TAG_ntp, "Time is not set yet. Connecting to WiFi and getting time over NTP.");
        obtain_time();

        time(&now);
    }

    char strftime_buf[64];

    setenv("TZ", "WGT3WGST,M3.5.0/-2,M10.5.0/-1", 1);
    tzset();
    localtime_r(&now, &timeinfo);
    strftime(strftime_buf, sizeof(strftime_buf), "%c", &timeinfo);
    ESP_LOGI(TAG_ntp, "Brasil SC: %s", strftime_buf);

    if (sntp_get_sync_mode() == SNTP_SYNC_MODE_SMOOTH)
    {
        struct timeval outdelta;
        while (sntp_get_sync_status() == SNTP_SYNC_STATUS_IN_PROGRESS)
        {
            adjtime(NULL, &outdelta);
            ESP_LOGI(TAG_ntp, "Waiting for adjusting time ... outdelta = %li sec: %li ms: %li us",
                     (long)outdelta.tv_sec,
                     outdelta.tv_usec / 1000,
                     outdelta.tv_usec % 1000);
            vTaskDelay(2000 / portTICK_PERIOD_MS);
        }
    }
}

void sntp::obtain_time(void)
{

    initialize_sntp();

    time_t now = 0;
    struct tm timeinfo = {
        .tm_sec = 0,
        .tm_min = 0,
        .tm_hour = 0,
        .tm_mday = 0,
        .tm_mon = 0,
        .tm_year = 0,
        .tm_wday = 1,
        .tm_yday = 1,
        .tm_isdst = 1,
    };
    int retry = 0;
    const int retry_count = 10;
    while (sntp_get_sync_status() == SNTP_SYNC_STATUS_RESET && ++retry < retry_count)
    {
        ESP_LOGI(TAG_ntp, "Aguardando a hora do sistema ser definida... (%d/%d)", retry, retry_count);
        vTaskDelay(2000 / portTICK_PERIOD_MS);
    }
    time(&now);
    localtime_r(&now, &timeinfo);
}

static void time_sync_notification_cb(struct timeval *tv)
{
    ESP_LOGI(TAG_ntp, "time_sync_notification_cb");
}
void sntp::initialize_sntp(void)
{
    ESP_LOGI(TAG_ntp, "SNTP Iniciado");
    sntp_setoperatingmode(SNTP_OPMODE_POLL);
    sntp_setservername(0, "a.st1.ntp.br");
    sntp_set_time_sync_notification_cb(time_sync_notification_cb);
    sntp_init();
}

// Horas em formato 24 [0-23]
uint8_t sntp::get_hour(void)
{

    sntp::update_time_internal();
    return sntp::timeinfo.tm_hour;
}

// Minutos de 0-59
uint8_t sntp::get_min(void)
{

    sntp::update_time_internal();
    return sntp::timeinfo.tm_min;
}

// Dias da semanda 0-6
uint8_t sntp::get_week_day()
{

    sntp::update_time_internal();
    return sntp::timeinfo.tm_wday;
}
// Meses do ano 0-11
uint8_t sntp::get_mon()
{

    sntp::update_time_internal();
    return sntp::timeinfo.tm_mon;
}

// Segundos 0-60
uint8_t sntp::get_seg(void)
{
    sntp::update_time_internal();
    return sntp::timeinfo.tm_sec;
}

// Retorna ano apartir de 1900
int sntp::get_year(void)
{

    sntp::update_time_internal();
    return sntp::timeinfo.tm_year + 1900;
}

// Passo o clock da CPU para strutura tm
void sntp::update_time_internal(void)
{
    time(&now);
    localtime_r(&now, &timeinfo);
}
