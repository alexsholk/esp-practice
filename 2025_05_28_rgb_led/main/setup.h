#pragma once

#include <stdint.h>
#include "led_strip.h"

void configure_led(led_strip_handle_t* led, uint8_t gpio_num);
