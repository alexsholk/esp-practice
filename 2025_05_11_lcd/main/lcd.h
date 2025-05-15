#pragma once

#include <stdint.h>
#include <stdbool.h>

typedef struct
{
    uint8_t x, y;
} point;

void init_display();
void clear_display();
void set_pixel(uint8_t x, uint8_t y);
void invert_colors(bool invert);
void draw_sierpinski(uint16_t iterations);
void draw_square_fractal(uint16_t iterations);
void chaos_game(point vertices[], uint8_t vertex_count, float multiplier, uint16_t iterations);