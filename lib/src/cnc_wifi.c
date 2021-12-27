#include "cnc_wifi.h"

static const char *TAG = "WiFi-Handler";
static EventGroupHandle_t wifi_event_group;
static esp_netif_t *sta_netif = NULL;
const int CONNECTED_BIT = BIT0;

esp_netif_t *get_netif( void )
{
    return sta_netif;
}

int get_connected_bit( void )
{
    return CONNECTED_BIT;
}

EventGroupHandle_t get_wifi_event_group( void )
{
    return wifi_event_group;
}

void initialise_wifi( cnc_wifi_config_t *config )
{
    ESP_ERROR_CHECK(esp_netif_init());
    wifi_event_group = xEventGroupCreate();
    ESP_ERROR_CHECK(esp_event_loop_create_default());
    sta_netif = esp_netif_create_default_wifi_sta();
    assert(sta_netif);

    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK( esp_wifi_init(&cfg) );
    ESP_ERROR_CHECK( esp_event_handler_register( ESP_EVENT_ANY_BASE, ESP_EVENT_ANY_ID, config->event_handler, config->args ) );
    ESP_ERROR_CHECK( esp_wifi_set_storage(WIFI_STORAGE_RAM) );
    ESP_LOGI(TAG, "Setting WiFi configuration SSID %s...", config->wifi_config.sta.ssid);
    ESP_ERROR_CHECK( esp_wifi_set_mode(WIFI_MODE_STA) );
    ESP_ERROR_CHECK( esp_wifi_set_config(WIFI_IF_STA, &( config->wifi_config ) ) );
    ESP_ERROR_CHECK( esp_wifi_start() );
}