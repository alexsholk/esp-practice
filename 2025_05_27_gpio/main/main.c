#include <stdio.h>
#include "driver/gpio.h"

void app_main(void)
{
    // Dump IO configuration for 0..21 and 26..48 (45 in total)
    gpio_dump_io_configuration(stdout, SOC_GPIO_VALID_GPIO_MASK);
}
