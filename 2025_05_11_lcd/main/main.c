#include "lcd.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

void app_main(void)
{
    init_display();
    clear_display();
    draw_square_fractal(25000);
    vTaskDelay(2000 / portTICK_PERIOD_MS);
    clear_display();
    draw_sierpinski(25000);
    invert_colors(true);
}
