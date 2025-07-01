#include <stdio.h>
#include <string.h>

#include "esp_mac.h"

void print_mac_addresses(void);

void app_main(void)
{
    printf("Receiver\n");
    print_mac_addresses();
}

void print_mac_addresses(void)
{
    static const struct
    {
        esp_mac_type_t type;
        char* description;
    } mac_types[] = {
            {ESP_MAC_BASE, "Base MAC for that used for other MAC types"},
            {ESP_MAC_WIFI_STA, "MAC for WiFi Station"},
            {ESP_MAC_WIFI_SOFTAP, "MAC for WiFi Soft-AP"},
            {ESP_MAC_BT, "MAC for Bluetooth"},
            {ESP_MAC_ETH, "MAC for Ethernet"},
            {ESP_MAC_EFUSE_FACTORY, "MAC_FACTORY eFuse which was burned by Espressif in production"},
            {ESP_MAC_EFUSE_CUSTOM, "MAC_CUSTOM eFuse which was can be burned by customer"},
        };

    uint8_t mac[6];
    for (int i = 0; i < sizeof(mac_types) / sizeof(mac_types[0]); i++)
    {
        if (ESP_OK == esp_read_mac(mac, mac_types[i].type))
        {
            printf("%s: " MACSTR "\n", mac_types[i].description, MAC2STR(mac));
        }
    }
}
