#include "functions.h"
#include <stdio.h>
#include <stdint.h>
#include "led_strip.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_err.h"

void configure_led(led_strip_handle_t* led, const uint8_t gpio_num)
{
    const led_strip_config_t strip_config = {
        .strip_gpio_num = gpio_num, // The GPIO that connected to the LED strip's data line
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
    ESP_ERROR_CHECK(led_strip_new_spi_device(&strip_config, &spi_config, led));
}

void show_color(struct led_strip_t* const led, const color_t color, const uint32_t duration)
{
    ESP_ERROR_CHECK(led_strip_set_pixel(led, 0, color.r, color.g, color.b));
    ESP_ERROR_CHECK(led_strip_refresh(led));
    vTaskDelay(pdMS_TO_TICKS(duration));
}

void show_palette(struct led_strip_t* const led, palette_t const* palette, const uint32_t duration)
{
    for (uint8_t i = 0; i < palette->len; i++)
    {
        show_color(led, palette->colors[i], duration);
    }
}

void show_palette_smooth(struct led_strip_t* const led,
                         palette_t const* palette,
                         const uint32_t duration,
                         const uint8_t steps)
{
    palette_t gradient;
    gradient.len = steps;
    gradient.colors = malloc(sizeof(color_t) * gradient.len);

    for (uint8_t i = 0; i < palette->len; i++)
    {
        generate_gradient(
            palette->colors[i % palette->len],
            palette->colors[(i + 1) % palette->len],
            &gradient
        );

        show_palette(led, &gradient, duration / steps);
    }

    free(gradient.colors);
}

void generate_gradient(const color_t start, const color_t end, palette_t const* gradient)
{
    for (uint8_t i = 0; i < gradient->len; i++)
    {
        gradient->colors[i].r = start.r + (end.r - start.r) * (i + (start.r > end.r)) / (gradient->len - 1);
        gradient->colors[i].g = start.g + (end.g - start.g) * (i + (start.g > end.g)) / (gradient->len - 1);
        gradient->colors[i].b = start.b + (end.b - start.b) * (i + (start.b > end.b)) / (gradient->len - 1);
    }
}

void print_palette(palette_t const* palette)
{
    printf("+-----+-----+-----+-----+---------+\n");
    printf("|     |  R  |  G  |  B  |   Hex   |\n");
    printf("+-----+-----+-----+-----+---------+\n");
    for (uint8_t i = 0; i < palette->len; i++)
    {
        const color_t c = palette->colors[i];
        const uint8_t r = (uint8_t)c.r;
        const uint8_t g = (uint8_t)c.g;
        const uint8_t b = (uint8_t)c.b;

        printf("| %3d | %3d | %3d | %3d | #%02x%02x%02x |\n", i, r, g, b, r, g, b);
    }
    printf("+-----+-----+-----+-----+---------+\n");
}
