#include <stdio.h>
#include "esp_chip_info.h"

char* get_chip_name(const esp_chip_info_t* chip_info)
{
    switch (chip_info->model)
    {
    case CHIP_ESP32: return "ESP32";
    case CHIP_ESP32S2: return "ESP32-S2";
    case CHIP_ESP32S3: return "ESP32-S3";
    case CHIP_ESP32C2: return "ESP32-C2";
    case CHIP_ESP32C3: return "ESP32-C3";
    case CHIP_ESP32C5: return "ESP32-C5";
    case CHIP_ESP32C6: return "ESP32-C6";
    case CHIP_ESP32C61: return "ESP32-C61";
    case CHIP_ESP32H2: return "ESP32-H2";
    case CHIP_ESP32P4: return "ESP32-P4";
    case CHIP_POSIX_LINUX: return "POSIX/Linux simulator";
    default: return "Unknown";
    }
}

void print_chip_info(void)
{
    esp_chip_info_t chip_info;
    esp_chip_info(&chip_info);
    printf("This is %s chip with %d CPU core(s)\n", get_chip_name(&chip_info), chip_info.cores);
    printf("Chip has embedded flash memory: %s\n",  (chip_info.features & CHIP_FEATURE_EMB_FLASH)  ? "yes" : "no");
    printf("Chip has 2.4GHz WiFi: %s\n",            (chip_info.features & CHIP_FEATURE_WIFI_BGN)   ? "yes" : "no");
    printf("Chip has Bluetooth LE: %s\n",           (chip_info.features & CHIP_FEATURE_BLE)        ? "yes" : "no");
    printf("Chip has Bluetooth Classic: %s\n",      (chip_info.features & CHIP_FEATURE_BT)         ? "yes" : "no");
    printf("Chip has IEEE 802.15.4: %s\n",          (chip_info.features & CHIP_FEATURE_IEEE802154) ? "yes" : "no");
    printf("Chip has embedded psram: %s\n",         (chip_info.features & CHIP_FEATURE_EMB_PSRAM)  ? "yes" : "no");
}

void app_main(void)
{
    printf("Hello world!\n");

    print_chip_info();
}
