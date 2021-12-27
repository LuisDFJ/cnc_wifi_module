#include "cnc_server.h"

static const char *TAG = "HTTP-SERVER";

httpd_handle_t start_webserver( uri_array_t *uris )
{
    httpd_handle_t server = NULL;
    httpd_config_t config = HTTPD_DEFAULT_CONFIG();
    config.lru_purge_enable = true;
    ESP_LOGI(TAG, "Starting server on port: '%d'", config.server_port);
    if (httpd_start(&server, &config) == ESP_OK) {
        ESP_LOGI(TAG, "Registering URI handlers");
        for( uint8_t i = 0; i < uris->size; i++ )
        {
            httpd_uri_t *uri = uris->uri + i;
            httpd_register_uri_handler(server, uri );
        }
        return server;
    }
    ESP_LOGI(TAG, "Error starting server!");
    return NULL;
}

void stop_webserver(httpd_handle_t server)
{
    httpd_stop(server);
}