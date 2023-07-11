#include <stdlib.h>
#include "stdint.h"

uint16_t calculate_crc(unsigned char *data_ptr, unsigned short size);
uint8_t check_crc(unsigned char *data_ptr, unsigned short size);

