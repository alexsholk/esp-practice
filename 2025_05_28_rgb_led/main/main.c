#include "led_strip.h"
#include "esp_random.h"
#include "esp_log.h"
#include "esp_err.h"

#define BUILT_IN_RGB_GPIO 48

static const char* TAG = "RGB LED";

typedef struct
{
    uint32_t r;
    uint32_t g;
    uint32_t b;
} color_t;

typedef struct
{
    color_t* colors;
    size_t len;
} palette_t;

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

led_strip_handle_t configure_led(void);
void show_color(led_strip_handle_t led, color_t color, uint32_t duration);
void show_palette(led_strip_handle_t led, palette_t palette, uint32_t duration);

void app_main(void)
{
    ESP_LOGI(TAG, "Configure LED");
    struct led_strip_t* const led = configure_led();

    ESP_LOGI(TAG, "Show 8-colors Palette");
    show_palette(led, palette_8, 1000);
    vTaskDelay(pdMS_TO_TICKS(2000));

    ESP_LOGI(TAG, "Show 16-colors Palette");
    show_palette(led, palette_16, 1000);
    vTaskDelay(pdMS_TO_TICKS(2000));

    ESP_ERROR_CHECK(led_strip_del(led));
}

led_strip_handle_t configure_led(void)
{
    const led_strip_config_t strip_config = {
        .strip_gpio_num = BUILT_IN_RGB_GPIO, // The GPIO that connected to the LED strip's data line
        .max_leds = 1, // The number of LEDs in the strip,
        .led_model = LED_MODEL_WS2812, // LED strip model
        // set the color order of the strip: GRB
        .color_component_format = {
            .format = {
                .r_pos = 1, // red is the second byte in the color data
                .g_pos = 0, // green is the first byte in the color data
                .b_pos = 2, // blue is the third byte in the color data
                .num_components = 3, // total 3 color components
            },
        },
        .flags = {
            .invert_out = false, // don't invert the output signal
        }
    };

    // LED strip backend configuration: SPI
    const led_strip_spi_config_t spi_config = {
        .clk_src = SPI_CLK_SRC_DEFAULT, // different clock source can lead to different power consumption
        .spi_bus = SPI2_HOST, // SPI bus ID
        .flags = {
            .with_dma = true, // Using DMA can improve performance and help drive more LEDs
        }
    };

    // LED Strip handle
    led_strip_handle_t led_strip;
    ESP_ERROR_CHECK(led_strip_new_spi_device(&strip_config, &spi_config, &led_strip));

    return led_strip;
}

void show_color(struct led_strip_t* const led, const color_t color, const uint32_t duration)
{
    ESP_ERROR_CHECK(led_strip_set_pixel(led, 0, color.r, color.g, color.b));
    ESP_ERROR_CHECK(led_strip_refresh(led));
    vTaskDelay(pdMS_TO_TICKS(duration));
}

void show_palette(struct led_strip_t* const led, const palette_t palette, const uint32_t duration)
{
    for (size_t i = 0; i < palette.len; i++)
    {
        show_color(led, palette.colors[i], duration);
    }

    led_strip_clear(led);
}
