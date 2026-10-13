#include "ft_nmap.h"

int open_tcp_sender(struct in_addr source)
{
    struct sockaddr_in local = {0};
    int fd = socket(AF_INET, SOCK_RAW, IPPROTO_TCP);

    if (fd == -1)
    {
        perror("ft_nmap: socket");
        return -1;
    }

    local.sin_family = AF_INET;
    local.sin_addr = source;

    if (bind(fd, (const struct sockaddr *)&local, sizeof(local)) == -1)
    {
        perror("ft_nmap: bind");
        close(fd);
        return -1;
    }

    return fd;
}
