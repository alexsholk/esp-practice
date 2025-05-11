#include <stdio.h>
#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

void app_main(void)
{
    const gpio_num_t LED_O1 = GPIO_NUM_1;
    const gpio_num_t LED_O2 = GPIO_NUM_2;

    const gpio_config_t io_conf = {
        .pin_bit_mask = (1ULL << LED_O1) | (1ULL << LED_O2),
        .mode = GPIO_MODE_OUTPUT,
        .pull_up_en = GPIO_PULLUP_DISABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type = GPIO_INTR_DISABLE
    };
    gpio_config(&io_conf);

    while (1)
    {
        gpio_set_level(LED_O1, 1);
        gpio_set_level(LED_O2, 0);
        printf("LED1 ON, LED2 OFF\n");
        vTaskDelay(pdMS_TO_TICKS(500));

        gpio_set_level(LED_O1, 0);
        gpio_set_level(LED_O2, 1);
        printf("LED1 OFF, LED2 ON\n");
        vTaskDelay(pdMS_TO_TICKS(500));
    }
}
