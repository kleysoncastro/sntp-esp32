/* 
    Componente configura hora e date via sntp
    @outhor kleysoncastro99@gmail.com
 */
#ifndef  DATE_SNTP_H
#define  DATE_SNTP_H

#include <time.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "esp_event.h"
#include "esp_log.h"
#include "esp_attr.h"
#include "esp_sleep.h"
#include "esp_sntp.h"

// classe para iniciar sntp e atualizar hora-data
class sntp {

    public:
    sntp();
    ~sntp();
    void update_date_time();

    uint8_t get_hour(void);
    uint8_t get_min(void);
    uint8_t get_week_day(void);
    uint8_t get_mon(void);
    uint8_t get_seg(void);
    int get_year(void);

    private:
    static void obtain_time(void);
    static void initialize_sntp(void);
    void update_time_internal(void);
    time_t now;
    tm timeinfo;

};


#endif
