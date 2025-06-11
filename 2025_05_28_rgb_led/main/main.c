#include "functions.h"
#include "esp_log.h"
#include "esp_err.h"

#define BUILT_IN_RGB_GPIO 48

static const char* TAG = "RGB LED";

const palette_t palette_8 = {
    .colors = (color_t[]){
        {.r = 255, .g = 255, .b = 255},
        {.r = 0, .g = 0, .b = 0},
        {.r = 255, .g = 0, .b = 0},
        {.r = 0, .g = 255, .b = 0},
        {.r = 0, .g = 0, .b = 255},
        {.r = 255, .g = 255, .b = 0},
        {.r = 0, .g = 255, .b = 255},
        {.r = 255, .g = 0, .b = 255},
    },
    .len = 8,
};

const palette_t palette_16 = {
    .colors = (color_t[]){
        {.r = 255, .g = 255, .b = 255},
        {.r = 170, .g = 170, .b = 170},
        {.r = 85, .g = 85, .b = 85},
        {.r = 0, .g = 0, .b = 0},
        {.r = 255, .g = 255, .b = 85},
        {.r = 0, .g = 170, .b = 0},
        {.r = 85, .g = 255, .b = 85},
        {.r = 255, .g = 85, .b = 85},
        {.r = 170, .g = 0, .b = 0},
        {.r = 170, .g = 85, .b = 0},
        {.r = 170, .g = 0, .b = 170},
        {.r = 255, .g = 85, .b = 255},
        {.r = 85, .g = 255, .b = 255},
        {.r = 0, .g = 170, .b = 170},
        {.r = 0, .g = 0, .b = 170},
        {.r = 85, .g = 85, .b = 255},
    },
    .len = 16,
};

void app_main(void)
{
    ESP_LOGI(TAG, "Configure LED");
    led_strip_handle_t led;
    configure_led(&led, BUILT_IN_RGB_GPIO);

    ESP_LOGI(TAG, "Show 8-colors Palette");
    print_palette(&palette_8);
    show_palette_smooth(led, &palette_8, 1000, 20);

    ESP_LOGI(TAG, "Show 16-colors Palette");
    print_palette(&palette_16);
    show_palette_smooth(led, &palette_16, 1000, 20);

    ESP_ERROR_CHECK(led_strip_del(led));
}
