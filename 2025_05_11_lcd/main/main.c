#include "lcd.h"

void app_main(void)
{
    init_display();
    clear_display();
    draw_sierpinski(25000);
    invert_colors(true);
}
