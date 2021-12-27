#ifndef CNC_SERVER_H
#define CNC_SERVER_H

#include <string.h>
#include <stdlib.h>
#include "esp_event.h"
#include "esp_log.h"
#include "esp_system.h"
#include <esp_http_server.h>

typedef struct {
    httpd_uri_t *uri;
    uint8_t     size;
} uri_array_t;

httpd_handle_t start_webserver( uri_array_t* );
void stop_webserver( httpd_handle_t );

#endif