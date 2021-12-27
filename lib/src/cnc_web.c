#include "cnc_web.h"

static const char *TAG = "WEB SERVER";

static void get_header( httpd_req_t *req, char **buf, const char *field )
{
    size_t len = httpd_req_get_hdr_value_len(req, field) + 1;
    if (len > 1) {
        *buf = malloc(len);
        if (httpd_req_get_hdr_value_str(req, field, *buf, len) == ESP_OK) {
            ESP_LOGI(TAG, "%s: %s", field, *buf);
        }
    }
}

static void get_query( httpd_req_t *req, char **buf )
{
    size_t len = httpd_req_get_url_query_len(req) + 1;
    if (len > 1) {
        *buf = malloc(len);
        if (httpd_req_get_url_query_str(req, *buf, len) == ESP_OK) {
            ESP_LOGI(TAG, "Query: %s", *buf);
        }
        else {
            free(*buf);
            *buf = NULL;
        }
    }
}

static void get_param( char *query, char *param, char **buf )
{
    *buf = malloc( MAX_PARAM_SIZE );
    if (httpd_query_key_value( query, param, *buf, MAX_PARAM_SIZE ) == ESP_OK) {
        ESP_LOGI(TAG, "%s: %s", param, *buf);
    }
}

static void get_recv_header( httpd_req_t *req, int *remaining )
{
    int ret;
    *remaining = req->content_len;
    char discard[OPEN_HEADER];
    ret = httpd_req_recv(req, discard, OPEN_HEADER);
    *remaining -= ret;
    char buf[100];
    ret = httpd_req_recv(req, buf, MIN( sizeof(buf), *remaining ) );
    *remaining -= ret;
    ESP_LOGI(TAG, "=========== RECEIVED HEADER ==========");
    ESP_LOGI(TAG, "%.*s", ret, buf);
    ESP_LOGI(TAG, "====================================");
}

static esp_err_t get_recv_body( httpd_req_t *req, int *remaining )
{
    char buf[ MAX_BUFFER_SIZE ];
    int ret;
    *remaining -= CLONE_HEADER;

    struct stat st;
    const char *filename = MOUNT_POINT"/file.txt";
    if( stat( filename, &st ) == 0 )
        unlink( filename );
    FILE *f = fopen( filename, "a" );
    while (*remaining > 0) {
        if ( (ret = httpd_req_recv(req, buf, MIN(*remaining, sizeof(buf)))) <= 0) {
            if (ret == HTTPD_SOCK_ERR_TIMEOUT) {
                continue;
            }
            return ESP_FAIL;
        }
        *remaining -= ret;
        httpd_resp_send_chunk(req, buf, ret);
        fprintf( f, "%.*s", ret, buf );
        ESP_LOGI(TAG, "=========== RECEIVED DATA ==========");
        ESP_LOGI(TAG, "%.*s", ret, buf);
        ESP_LOGI(TAG, "====================================");
    }
    httpd_resp_send_chunk(req, NULL, 0);
    fclose(f);
    return ESP_OK;
}

static esp_err_t status_get_handler(httpd_req_t *req)
{
    unsigned char *status = (unsigned char*) req->user_ctx;
    char*  buf;
    get_header( req, &buf, "Host" );
    CLEAR( buf )
    get_header( req, &buf, "Custom-header" );
    CLEAR( buf )

    get_query( req, &buf );
    if( buf != NULL )
    {
        char *param;
        int sbit = 0, cbit = 0;
        get_param( buf, "set", &param );
        sbit = atoi( param );
        CLEAR( param )
        get_param( buf, "clear", &param );
        cbit = atoi( param );
        CLEAR( param )
        ESP_LOGI( TAG, "Set: %d", sbit );
        ESP_LOGI( TAG, "Clr: %d", cbit );

        DEC_REG( sbit - 1, *status, SET_BIT );
        DEC_REG( cbit - 1, *status, CLR_BIT );

        ESP_LOGI( TAG, "0x%02x", *status );

        CLEAR( buf )
    }
    
    char resp_str[64];
    char bstatus[16]; 
    
    itoa( *status, bstatus, 2 );
    memset( resp_str, 0, sizeof(resp_str) );
    strcat( resp_str, "STATUS: " );
    strcat( resp_str, bstatus );
    httpd_resp_send(req, resp_str, HTTPD_RESP_USE_STRLEN);

    return ESP_OK;
}

static httpd_uri_t status = {
    .uri       = "/status",
    .method    = HTTP_GET,
    .handler   = status_get_handler,
    .user_ctx  = NULL
};

static esp_err_t file_post_handler(httpd_req_t *req)
{
    int remaining;
    get_recv_header( req, &remaining );
    esp_err_t res = get_recv_body( req, &remaining );
    return res;
}

static httpd_uri_t file = {
    .uri       = "/file",
    .method    = HTTP_POST,
    .handler   = file_post_handler,
    .user_ctx  = NULL
};


static esp_err_t format_get_handler(httpd_req_t *req)
{
    sdmmc_card_t *card = NULL;
    esp_vfs_fat_sdmmc_unmount();
    ESP_ERROR_CHECK( init_sd( card, true ) );
    const char *resp_str = "CARD FORMATED!";
    httpd_resp_send(req, resp_str, HTTPD_RESP_USE_STRLEN);
    return ESP_OK;
}

static httpd_uri_t format = {
    .uri       = "/format",
    .method    = HTTP_GET,
    .handler   = format_get_handler,
    .user_ctx  = NULL
};

unsigned char get_uris( httpd_uri_t **uris, void *ctx )
{
    *uris = (httpd_uri_t*)malloc( sizeof( httpd_uri_t ) * 3 );
    file.user_ctx = ctx;
    status.user_ctx = ctx;
    *(*uris + 0) = file;
    *(*uris + 1) = status;
    *(*uris + 2) = format;
    return 3;
}