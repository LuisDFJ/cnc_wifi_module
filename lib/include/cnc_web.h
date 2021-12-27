#ifndef CNC_WEB_H
#define CNC_WEB_H

#include <string.h>
#include <stdlib.h>
#include "esp_event.h"
#include "esp_log.h"
#include "esp_system.h"
#include <esp_http_server.h>
#include <sys/param.h>
#include "cnc_sd.h"
#include "cnc_codes.h"

#define MAX_PARAM_SIZE 32
#define OPEN_HEADER 54
#define CLONE_HEADER 56
#define MAX_BUFFER_SIZE 1024

#define CLEAR( buf )    \
    free( buf );        \
    buf = NULL;

void set_run_flag       ( void );
void clear_run_flag     ( void );
bool get_run_flag       ( void );
unsigned char get_uris  ( httpd_uri_t**, void* );


#endif