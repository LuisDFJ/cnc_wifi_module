#ifndef CNC_UTILS_H
#define CNC_UTILS_H

#include "driver/gpio.h"
#include "cnc_server.h"
#include "cnc_wifi.h"
#include "cnc_sd.h"
#include "cnc_codes.h"

#define GPIO_STATUS_LED 21

typedef struct {
    httpd_handle_t  *server;
    uri_array_t     *uris;
} utils_args_t;

void main_task( void* );
void event_handler( void*, esp_event_base_t, int32_t, void* );

#endif