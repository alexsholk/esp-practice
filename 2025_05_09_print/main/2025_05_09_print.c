#include <stdio.h>
#include "esp_chip_info.h"

void print_chip_info(void)
{
    esp_chip_info_t chip_info;
    esp_chip_info(&chip_info);
    printf("This is %s chip with %d CPU core(s)\n", CONFIG_IDF_TARGET, chip_info.cores);
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
