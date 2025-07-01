#include <stdio.h>
#include <string.h>

#include "esp_mac.h"

void app_main(void)
{
    printf("Receiver\n");

    uint8_t mac[6];

    esp_read_mac(mac, ESP_MAC_BASE);
    printf("Base MAC for that used for other MAC types: %X:%X:%X:%X:%X:%X\n", mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
    memset(mac, 0, sizeof(mac));

    esp_read_mac(mac, ESP_MAC_WIFI_STA);
    printf("MAC for WiFi Station: %X:%X:%X:%X:%X:%X\n", mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
    memset(mac, 0, sizeof(mac));

    esp_read_mac(mac, ESP_MAC_WIFI_SOFTAP);
    printf("MAC for WiFi Soft-AP: %X:%X:%X:%X:%X:%X\n", mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
    memset(mac, 0, sizeof(mac));

    esp_read_mac(mac, ESP_MAC_BT);
    printf("MAC for Bluetooth: %X:%X:%X:%X:%X:%X\n", mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
    memset(mac, 0, sizeof(mac));

    esp_read_mac(mac, ESP_MAC_ETH);
    printf("MAC for Ethernet: %X:%X:%X:%X:%X:%X\n", mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
    memset(mac, 0, sizeof(mac));

    esp_read_mac(mac, ESP_MAC_EFUSE_FACTORY);
    printf("MAC_FACTORY eFuse which was burned by Espressif in production: %X:%X:%X:%X:%X:%X\n", mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
    memset(mac, 0, sizeof(mac));

    esp_read_mac(mac, ESP_MAC_EFUSE_CUSTOM);
    printf("MAC_CUSTOM eFuse which was can be burned by customer: %X:%X:%X:%X:%X:%X\n", mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
    memset(mac, 0, sizeof(mac));
}
