#include "nvs_flash.h"
#include "cnc_utils.h"
#include "cnc_web.h"
#include "cnc_sd.h"
#include "driver/gpio.h"

#define WIFI_SSID "INFINITUM2D1E"
#define WIFI_PSWD "Aa1Hh8Sq8w"

unsigned char status = 0;

void app_main(void)
{
    sdmmc_card_t *card = NULL;
    // URIS INIT
    httpd_uri_t *uri_array = NULL;
    unsigned char size = get_uris( &uri_array, &status );
    uri_array_t uris = {
        .uri = uri_array,
        .size = size
    };
    // SERVER INIT
    static httpd_handle_t server = NULL;
    // ARGS INIT
    utils_args_t args = {
        .server = &server,
        .uris = &uris
    };
    // WIFI CONFIG INIT
    cnc_wifi_config_t config = {
        .wifi_config = {
            .sta = {
                .ssid = WIFI_SSID,
                .password = WIFI_PSWD
            }
        },
        .event_handler = event_handler,
        .args = &args
    };
    // NVS FLASH INIT
    ESP_ERROR_CHECK( nvs_flash_init() );
    ESP_ERROR_CHECK( init_sd( card, false ) );
    // INIT WIFI
    initialise_wifi( &config );
    // MAIN TASK
    xTaskCreate( &main_task, "main_task", 4096, &status, 5, NULL );
    // START SERVER
    server = start_webserver( &uris );
}