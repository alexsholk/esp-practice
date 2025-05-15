#pragma once

#include <stdint.h>
#include <stdbool.h>

void init_display();
void clear_display();
void set_pixel(uint8_t x, uint8_t y);
void invert_colors(bool invert);
void draw_sierpinski(uint16_t iterations);