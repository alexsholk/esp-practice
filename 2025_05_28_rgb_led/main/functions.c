#include <stdio.h>
#include <stdint.h>
#include "functions.h"
#include "led_strip.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_err.h"

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

inline uint8_t blend(const uint8_t start, const uint8_t end, const uint8_t step, const uint8_t steps)
{
    return (start < end)
               ? start + (end - start) * step / (steps - 1)
               : start - (start - end) * step / (steps - 1);
}

void generate_gradient(const color_t start, const color_t end, const palette_t* gradient)
{
    for (uint8_t i = 0; i < gradient->len; i++)
    {
        gradient->colors[i].r = blend(start.r, end.r, i, gradient->len);
        gradient->colors[i].g = blend(start.g, end.g, i, gradient->len);
        gradient->colors[i].b = blend(start.b, end.b, i, gradient->len);
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

        printf("| %3u | %3u | %3u | %3u | #%06x |\n",
               i, c.r, c.g, c.b,
               (c.r << 16) | (c.g << 8) | c.b);
    }
    printf("+-----+-----+-----+-----+---------+\n");
}
