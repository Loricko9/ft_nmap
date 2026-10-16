#include "ft_nmap.h"

uint16_t internet_checksum(const uint8_t *data, size_t length)
{
	uint32_t sum = 0;

    while (length >= 2)
    {
        uint32_t word = ((uint32_t)data[0] << 8) | data[1];

        sum += word;
        sum = (sum & 0xFFFFu) + (sum >> 16);

        data += 2;
        length -= 2;
    }

    if (length == 1)
        sum += (uint32_t)data[0] << 8;

    while (sum > 0xFFFFu)
        sum = (sum & 0xFFFFu) + (sum >> 16);

    return (uint16_t)~sum;
}
