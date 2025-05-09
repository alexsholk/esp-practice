#include <stdio.h>
#include "esp_chip_info.h"
#include "chip_info.h"
#include "log_demo.h"

void app_main(void)
{
    printf("Hello world!\n");

    esp_chip_info_t chip_info;
    esp_chip_info(&chip_info);

    print_chip_info(&chip_info);

    log_demo();
}
