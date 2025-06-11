#pragma once

#include <stdint.h>
#include "led_strip.h"

typedef struct
{
    uint32_t r;
    uint32_t g;
    uint32_t b;
} color_t;

typedef struct
{
    color_t* colors;
    uint8_t len;
} palette_t;

void configure_led(led_strip_handle_t* led, uint8_t gpio_num);
void show_color(struct led_strip_t* led, color_t color, uint32_t duration);
void show_palette(struct led_strip_t* led, palette_t const* palette, uint32_t duration);
void show_palette_smooth(struct led_strip_t* led, palette_t const* palette, uint32_t duration, uint8_t steps);
void generate_gradient(color_t start, color_t end, palette_t const* gradient);
void print_palette(palette_t const* palette);
