#include <stdio.h>
#include <string.h>

#include "esp_log.h"
#include "driver/i2c_master.h"
#include "driver/i2c_types.h"
#include "esp_lcd_panel_io.h"
#include "esp_lcd_panel_ops.h"
#include "esp_lcd_panel_vendor.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#define I2C_HW_ADDR 0x3C
#define LCD_PIXEL_CLOCK_HZ 400000
#define LCD_CMD_BITS 8
#define LCD_PARAM_BITS 8
#define LCD_H_RES 128
#define LCD_V_RES 64

static const char* TAG = "I2C LCD";

static uint8_t buffer[LCD_H_RES * LCD_V_RES / 8] = {0};

void set_pixel(const uint8_t x, const uint8_t y)
{
    if (x < LCD_H_RES && y < LCD_V_RES)
    {
        buffer[y / 8 * LCD_H_RES + x] |= 1 << (y % 8);
    }
}

void app_main(void)
{
    ESP_LOGI(TAG, "Initialize I2C bus");
    i2c_master_bus_handle_t i2c_bus;
    const i2c_master_bus_config_t bus_config = {
        .clk_source = I2C_CLK_SRC_DEFAULT,
        .glitch_ignore_cnt = 7,
        .i2c_port = CONFIG_I2C_BUS_PORT,
        .sda_io_num = CONFIG_I2C_SDA_GPIO,
        .scl_io_num = CONFIG_I2C_SCL_GPIO,
        .flags.enable_internal_pullup = true,
    };
    ESP_ERROR_CHECK(i2c_new_master_bus(&bus_config, &i2c_bus));

    ESP_LOGI(TAG, "Install panel IO");
    esp_lcd_panel_io_handle_t io_handle;
    const esp_lcd_panel_io_i2c_config_t io_config = {
        .dev_addr = I2C_HW_ADDR,
        .scl_speed_hz = LCD_PIXEL_CLOCK_HZ,
        .control_phase_bytes = 1,
        .lcd_cmd_bits = LCD_CMD_BITS,
        .lcd_param_bits = LCD_PARAM_BITS,
        .dc_bit_offset = 6,
    };
    ESP_ERROR_CHECK(esp_lcd_new_panel_io_i2c(i2c_bus, &io_config, &io_handle));

    ESP_LOGI(TAG, "Install SSD1306 panel driver");
    esp_lcd_panel_handle_t panel;
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

    for (uint8_t offset = 1; ; offset += 1, offset %= 12)
    {
        ESP_LOGI(TAG, "Offset = %d", offset);

        for (uint8_t y = 1, y0 = offset; y < LCD_V_RES; y += y0, y0 = y - y0)
        {
            ESP_LOGI(TAG, "Draw line at %d Y coordinate", y);
            for (uint8_t x = 64 - y; x < 64 + y; x++)
            {
                set_pixel(x, y);
            }
        }

        esp_lcd_panel_draw_bitmap(panel, 0, 0, LCD_H_RES, LCD_V_RES, buffer);
        vTaskDelay(pdMS_TO_TICKS(50));
        memset(buffer, 0, sizeof buffer);
    }
}

