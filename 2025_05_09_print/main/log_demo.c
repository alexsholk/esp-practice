#include "log_demo.h"
#include <stdio.h>
#include "esp_log.h"
#include "esp_log_level.h"

#define MESSAGE "%s - message with log level %s"

static const char* TAG = "Log demo";

char* log_level(const esp_log_level_t level)
{
    switch (level)
    {
    case ESP_LOG_NONE: return "ESP_LOG_NONE";
    case ESP_LOG_ERROR: return "ESP_LOG_ERROR";
    case ESP_LOG_WARN: return "ESP_LOG_WARN";
    case ESP_LOG_INFO: return "ESP_LOG_INFO";
    case ESP_LOG_DEBUG: return "ESP_LOG_DEBUG";
    case ESP_LOG_VERBOSE: return "ESP_LOG_VERBOSE";
    case ESP_LOG_MAX: return "ESP_LOG_MAX";
    default: return "Unknown";
    }
}

void log_demo(void)
{
    printf("Current log level: %s\n", log_level(esp_log_level_get(TAG)));
    printf("Set log level to %s\n", log_level(ESP_LOG_MAX));
    esp_log_level_set(TAG, ESP_LOG_MAX);
    printf("Current log level: %s\n", log_level(esp_log_level_get(TAG)));

    ESP_LOGE(TAG, MESSAGE, "ESP_LOGE", "ESP_LOG_ERROR");
    ESP_LOGW(TAG, MESSAGE, "ESP_LOGW", "ESP_LOG_WARN");
    ESP_LOGI(TAG, MESSAGE, "ESP_LOGI", "ESP_LOG_INFO");
    ESP_LOGD(TAG, MESSAGE, "ESP_LOGD", "ESP_LOG_DEBUG");
    ESP_LOGV(TAG, MESSAGE, "ESP_LOGV", "ESP_LOG_VERBOSE");
}
