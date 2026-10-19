#include "ft_nmap.h"

static void write_u16(uint8_t *destination, uint16_t value)
{
    value = htons(value);
    memcpy(destination, &value, sizeof(value));
}

static void write_u32(uint8_t *destination, uint32_t value)
{
    value = htonl(value);
    memcpy(destination, &value, sizeof(value));
}

void build_syn_header(uint8_t header[TCP_HEADER_SIZE], uint16_t source_port, uint16_t destination_port, uint32_t sequence)
{
    memset(header, 0, TCP_HEADER_SIZE);

    write_u16(header, source_port);
    write_u16(header + 2, destination_port);
    write_u32(header + 4, sequence);

    header[12] = (TCP_HEADER_SIZE / 4) << 4;
    header[13] = TCP_FLAG_SYN;

    write_u16(header + 14, 1024);
}

void finalize_tcp_checksum(uint8_t header[TCP_HEADER_SIZE],
                          struct in_addr source, struct in_addr destination)
{
    uint8_t input[12 + TCP_HEADER_SIZE] = {0};

    memcpy(input, &source.s_addr, 4);
    memcpy(input + 4, &destination.s_addr, 4);
    input[9] = IPPROTO_TCP;
    write_u16(input + 10, TCP_HEADER_SIZE);

    write_u16(header + 16, 0);
    memcpy(input + 12, header, TCP_HEADER_SIZE);
    write_u16(header + 16, internet_checksum(input, sizeof(input)));
}
