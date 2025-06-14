#include "setup.h"
#include "functions.h"
#include "palettes.h"
#include "esp_log.h"
#include "esp_err.h"

static const char* TAG = "RGB LED";

void app_main(void)
{
    ESP_LOGI(TAG, "Configure LED");
    led_strip_handle_t led;
    configure_led(&led, CONFIG_RGB_LED_GPIO);

    ESP_LOGI(TAG, "Show 8-colors Palette");
    print_palette(&palette_8);
    show_palette(led, &palette_8, 1000);

    ESP_LOGI(TAG, "Show 16-colors Palette");
    print_palette(&palette_16);
    show_palette(led, &palette_16, 1000);

    ESP_LOGI(TAG, "Show 8-colors Palette (smooth)");
    print_palette(&palette_8);
    show_palette_smooth(led, &palette_8, 1000, 100);

    ESP_LOGI(TAG, "Show 16-colors Palette (smooth)");
    print_palette(&palette_16);
    show_palette_smooth(led, &palette_16, 1000, 100);

    ESP_LOGI(TAG, "Turn off");
    ESP_ERROR_CHECK(led_strip_clear(led));
    ESP_ERROR_CHECK(led_strip_del(led));
}
