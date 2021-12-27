#ifndef CNC_SD_H
#define CNC_SD_H

#include <string.h>
#include <sys/unistd.h>
#include <sys/stat.h>
#include "esp_vfs_fat.h"
#include "sdmmc_cmd.h"
#include "driver/sdmmc_host.h"

#define MOUNT_POINT "/sdcard"

esp_err_t init_sd(sdmmc_card_t *, bool);

#endif