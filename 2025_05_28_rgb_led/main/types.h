#pragma once

#include <stdint.h>

typedef struct
{
    uint8_t r;
    uint8_t g;
    uint8_t b;
} color_t;

typedef struct
{
    color_t* colors;
    uint8_t len;
} palette_t;
