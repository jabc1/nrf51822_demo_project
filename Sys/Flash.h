#ifndef FLASH_H
#define FLASH_H

#include "app_util_platform.h"
void flash_init(void);
void flash_page_erase(uint32_t page_number);
void flash_page_write(uint32_t page_number, const uint8_t * src);
void flash_write(uint32_t bytes_address, const uint8_t *src, uint32_t num_byte);
void flash_read(uint32_t bytes_address, uint8_t *dest, uint32_t num_byte);

void flash_test(void);

#endif

