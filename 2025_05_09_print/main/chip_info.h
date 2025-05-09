#ifndef CHIP_INFO_H
#define CHIP_INFO_H

#include "esp_chip_info.h"

char* get_chip_name(const esp_chip_info_t* chip_info);
void print_chip_info(void);

#endif
