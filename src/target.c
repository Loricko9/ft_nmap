#include "ft_nmap.h"

bool prepare_ipv4_target(const char *text, struct in_addr *address)
{
    return (inet_pton(AF_INET, text, address) == 1);
}
