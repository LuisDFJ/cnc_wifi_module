#include "cnc_sd.h"

static const char *TAG = "SD-CARD";

esp_err_t init_sd(sdmmc_card_t *card, bool format)
{
    esp_err_t ret;
    esp_vfs_fat_sdmmc_mount_config_t mount_config = {
        .format_if_mount_failed = format,
        .max_files = 5,
        .allocation_unit_size = 16 * 1024
    };
    const char mount_point[] = MOUNT_POINT;
    ESP_LOGI(TAG, "Initializing SD card");
    ESP_LOGI(TAG, "Using SDMMC peripheral");
    sdmmc_host_t host = SDMMC_HOST_DEFAULT();
    sdmmc_slot_config_t slot_config = SDMMC_SLOT_CONFIG_DEFAULT();
    
    slot_config.width = 1;
    gpio_set_pull_mode(15, GPIO_PULLUP_ONLY);
    gpio_set_pull_mode(2, GPIO_PULLUP_ONLY);
    gpio_set_pull_mode(4, GPIO_PULLUP_ONLY);
    gpio_set_pull_mode(12, GPIO_PULLUP_ONLY);
    gpio_set_pull_mode(13, GPIO_PULLUP_ONLY);

    ret = esp_vfs_fat_sdmmc_mount(mount_point, &host, &slot_config, &mount_config, &card);

    if (ret != ESP_OK) {
        if (ret == ESP_FAIL) {
            ESP_LOGE(TAG, "Failed to mount filesystem. " );
        } else {
            ESP_LOGE(TAG, "Failed to initialize the card (%s).", esp_err_to_name(ret));
        }
        return ESP_FAIL;
    }
    return ESP_OK;
}