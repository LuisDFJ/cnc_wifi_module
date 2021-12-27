#include "cnc_utils.h"

static const char *TAG = "UTILS";

static void sta_disconnect_handler( void *arg )
{
    ESP_LOGI( TAG, "LED OFF" );
    esp_wifi_connect();
    xEventGroupClearBits( get_wifi_event_group(), get_connected_bit() );
    utils_args_t *utils = (utils_args_t*)arg;
    if ( *(utils->server) ) {
        ESP_LOGI(TAG, "Stopping webserver");
        stop_webserver( *(utils->server) );
        *(utils->server) = NULL;
    }
}

static void got_ip_handler( void *arg )
{
    ESP_LOGI( TAG, "LED ON" );
    xEventGroupSetBits( get_wifi_event_group(), get_connected_bit() );
    utils_args_t *utils = (utils_args_t*)arg;
    if (*(utils->server) == NULL) {
        ESP_LOGI(TAG, "Starting webserver");
        *(utils->server) = start_webserver( utils->uris );
    }
}

void event_handler(void* arg, esp_event_base_t event_base, int32_t event_id, void* event_data)
{
    if        (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_START)
        esp_wifi_connect();    
    else if   (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_DISCONNECTED)
        sta_disconnect_handler( arg );
    else if   (event_base == IP_EVENT && event_id == IP_EVENT_STA_GOT_IP) 
        got_ip_handler( arg );
}


static void print_task(void *pvParameters)
{
    unsigned char *status = (unsigned char*) pvParameters;
    struct stat st;
    const char *filename = MOUNT_POINT"/file.txt";
    SET_BIT( *status, BIT2_STATUS );
    if( stat( filename, &st ) == 0 ){
        ESP_LOGI( TAG, "File %s exists", filename );
        FILE *f = fopen( filename, "r" );
        if( f != NULL ){
            ESP_LOGI( TAG, "PRINTING" );
            char buf[256];
            while( REG_BIT( *status, BIT2_STATUS ) ){
                int i;
                for( i = 0; i < sizeof(buf) - 1; i++ ){
                    char c = fgetc( f );
                    if( feof( f ) ){
                        CLR_BIT( *status, BIT2_STATUS );
                        break;
                    }
                    buf[ i ] = c;
                    if( c == '\n' ){
                        i++;
                        break;
                    }
                }
                buf[i] = '\0';
                printf( "%s", buf );
            }
        }
    }
    ESP_LOGI( TAG, "Deleting Print Task" );
    SET_BIT( *status, BIT1_STATUS );
    vTaskDelete( NULL );
}


void main_task(void *pvParameters)
{
    unsigned char *status = (unsigned char*) pvParameters;
    esp_netif_ip_info_t ip;
    memset(&ip, 0, sizeof(esp_netif_ip_info_t));
    gpio_reset_pin( GPIO_STATUS_LED );
    gpio_set_direction( GPIO_STATUS_LED, GPIO_MODE_OUTPUT );
    SET_BIT( *status, BIT1_STATUS );
    vTaskDelay(2000 / portTICK_PERIOD_MS);

    while (1) {
        vTaskDelay(2000 / portTICK_PERIOD_MS);

        if (esp_netif_get_ip_info( get_netif(), &ip) == 0) {
            ESP_LOGI(TAG, "~~~~~~~~~~~");
            ESP_LOGI(TAG, "IP:"IPSTR, IP2STR(&ip.ip));
            ESP_LOGI(TAG, "MASK:"IPSTR, IP2STR(&ip.netmask));
            ESP_LOGI(TAG, "GW:"IPSTR, IP2STR(&ip.gw));
            ESP_LOGI(TAG, "~~~~~~~~~~~");
        }
        EventBits_t flag = xEventGroupGetBits( get_wifi_event_group() );
        ESP_LOGI( TAG, "0x%08x", flag );
        if( flag & get_connected_bit() ) {
            gpio_set_level( GPIO_STATUS_LED, 1 );
            ESP_LOGI( TAG, "ON" );
        }
        else {
            gpio_set_level( GPIO_STATUS_LED, 0 );
            ESP_LOGI( TAG, "OFF" );
        }

        ESP_LOGI( TAG, "STATUS: 0x%02x", *status );
        if ( REG_BIT( *status, BRUN_STATUS ) && REG_BIT( *status, BIT1_STATUS ) ){
            CLR_BIT( *status, BRUN_STATUS );
            CLR_BIT( *status, BIT1_STATUS );
            ESP_LOGI( TAG, "Creating Print Task" );
            xTaskCreate( print_task, "PrintTask", 4096, pvParameters, 2, NULL );
        }
    }
}

