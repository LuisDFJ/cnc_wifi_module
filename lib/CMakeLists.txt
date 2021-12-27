idf_component_register(SRCS "src/cnc_wifi.c" "src/cnc_server.c" "src/cnc_utils.c" "src/cnc_web.c" "src/cnc_sd.c"
                    INCLUDE_DIRS "include"
                    REQUIRES esp_http_server fatfs
                    )
