#ifndef CNC_WIFI_H
#define CNC_WIFI_H

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "esp_wifi.h"
#include "esp_event.h"
#include "esp_log.h"
#include "esp_system.h"
#include "esp_netif.h"

typedef struct {
    wifi_config_t   wifi_config;
    void            (*event_handler) ( void*, esp_event_base_t, int32_t, void* );
    void            *args;
} cnc_wifi_config_t;

esp_netif_t *get_netif( void );
void initialise_wifi( cnc_wifi_config_t *config );
int get_connected_bit( void );
EventGroupHandle_t get_wifi_event_group( void );

#endif