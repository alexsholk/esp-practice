#ifndef CHIP_INFO_H
#define CHIP_INFO_H

#include "esp_chip_info.h"

char* get_chip_name(esp_chip_model_t);
void print_chip_info(const esp_chip_info_t*);

#endif
