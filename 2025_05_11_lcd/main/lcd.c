#include <stdint.h>
#include <stdbool.h>
#include "driver/i2c_master.h"
#include "driver/i2c_types.h"
#include "esp_lcd_panel_io.h"
#include "esp_lcd_panel_vendor.h"
#include "esp_lcd_panel_ops.h"
#include "soc/clk_tree_defs.h"
#include "esp_random.h"
#include "esp_log.h"
#include "esp_err.h"
#include "lcd.h"

#define I2C_BUS_PORT 0
#define PIN_NUM_SDA 4
#define PIN_NUM_SCL 5
#define I2C_HW_ADDR 0x3C
#define LCD_PIXEL_CLOCK_HZ 400000
#define LCD_CMD_BITS 8
#define LCD_PARAM_BITS 8
#define LCD_H_RES 128
#define LCD_V_RES 64

static const char* TAG = "I2C LCD";

static uint8_t buffer[LCD_H_RES * LCD_V_RES / 8] = {0};

i2c_master_bus_handle_t i2c_bus = NULL;
esp_lcd_panel_io_handle_t io_handle = NULL;
esp_lcd_panel_handle_t panel = NULL;

void init_ic2()
{
    ESP_LOGI(TAG, "Initialize I2C bus");
    const i2c_master_bus_config_t bus_config = {
        .clk_source = I2C_CLK_SRC_DEFAULT,
        .glitch_ignore_cnt = 7,
        .i2c_port = I2C_BUS_PORT,
        .sda_io_num = PIN_NUM_SDA,
        .scl_io_num = PIN_NUM_SCL,
        .flags.enable_internal_pullup = true,
    };

    ESP_ERROR_CHECK(i2c_new_master_bus(&bus_config, &i2c_bus));
}

void init_panel_io()
{
    ESP_LOGI(TAG, "Install panel IO");
    const esp_lcd_panel_io_i2c_config_t io_config = {
        .dev_addr = I2C_HW_ADDR,
        .scl_speed_hz = LCD_PIXEL_CLOCK_HZ,
        .control_phase_bytes = 1,
        .lcd_cmd_bits = LCD_CMD_BITS,
        .lcd_param_bits = LCD_PARAM_BITS,
        .dc_bit_offset = 6,
    };

    ESP_ERROR_CHECK(esp_lcd_new_panel_io_i2c(i2c_bus, &io_config, &io_handle));
}

void init_lcd()
{
    ESP_LOGI(TAG, "Install SSD1306 panel driver");
    esp_lcd_panel_dev_config_t panel_config = {
        .bits_per_pixel = 1,
        .reset_gpio_num = -1,
    };
    esp_lcd_panel_ssd1306_config_t ssd1306_config = {
        .height = LCD_V_RES,
    };
    panel_config.vendor_config = &ssd1306_config;

    ESP_ERROR_CHECK(esp_lcd_new_panel_ssd1306(io_handle, &panel_config, &panel));
    ESP_ERROR_CHECK(esp_lcd_panel_reset(panel));
    ESP_ERROR_CHECK(esp_lcd_panel_init(panel));
    ESP_ERROR_CHECK(esp_lcd_panel_disp_on_off(panel, true));
}

void init_display()
{
    init_ic2();
    init_panel_io();
    init_lcd();
}

void clear_display()
{
    ESP_LOGI(TAG, "Clearing display");
    ESP_ERROR_CHECK(esp_lcd_panel_draw_bitmap(panel, 0, 0, LCD_H_RES, LCD_V_RES, NULL));
}

void set_pixel(const uint8_t x, const uint8_t y)
{
    if (x < LCD_H_RES && y < LCD_V_RES)
    {
        buffer[y / 8 * LCD_H_RES + x] |= 1 << (y % 8);
    }
}

void invert_colors(bool invert)
{
    ESP_LOGI(TAG, "Invert colors: %s", invert ? "ON" : "OFF");
    ESP_ERROR_CHECK(esp_lcd_panel_invert_color(panel, invert));
}

void draw_sierpinski(uint16_t iterations)
{
    ESP_LOGI(TAG, "Start drawing Sierpinski triangle...");

    point vertices[] = {
        {LCD_H_RES / 2, 0},
        {LCD_H_RES / 2 - 37, LCD_V_RES - 1},
        {LCD_H_RES / 2 + 37, LCD_V_RES - 1},
    };

    chaos_game(vertices, 3, 0.5F, iterations);
    ESP_LOGI(TAG, "Finished drawing Sierpinski triangle.");
}

void draw_square_fractal(uint16_t iterations)
{
    ESP_LOGI(TAG, "Start drawing square fractal...");

    point vertices[] = {
        {31, 0},
        {95, 0},
        {95, LCD_V_RES - 1},
        {31, LCD_V_RES - 1},
    };

    chaos_game(vertices, 4, 0.33F, iterations);
    ESP_LOGI(TAG, "Finished drawing square fractal.");
}

void chaos_game(point vertices[], uint8_t vertex_count, float multiplier, uint16_t iterations)
{
    ESP_LOGI(TAG, "Start chaos game...");

    point p = vertices[0];
    point v = {};

    for (uint16_t i = 0; i < iterations; i++)
    {
        v = vertices[esp_random() % vertex_count];
        p.x = (uint8_t) ((float) (p.x + v.x) * multiplier);
        p.y = (uint8_t) ((float) (p.y + v.y) * multiplier);

        set_pixel(p.x, p.y);

        if (i % 250 == 0)
        {
            ESP_ERROR_CHECK(esp_lcd_panel_draw_bitmap(panel, 0, 0, LCD_H_RES, LCD_V_RES, buffer));
            ESP_LOGI(TAG, "Iteration %d", i);
        }
    }

    ESP_ERROR_CHECK(esp_lcd_panel_draw_bitmap(panel, 0, 0, LCD_H_RES, LCD_V_RES, buffer));
    ESP_LOGI(TAG, "Finished chaos game.");
}
