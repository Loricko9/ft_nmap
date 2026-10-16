#include "ft_nmap.h"

bool preview_target(const char *target, const t_info *info)
{
    struct in_addr source;
    char src_text[INET_ADDRSTRLEN];
    char target_text[INET_ADDRSTRLEN];
    struct sockaddr_in destination = {0};

    if (!prepare_ipv4_target(target, &destination.sin_addr))
    {
        fprintf(stderr, "ft_nmap: Cannot prepare IPv4 target: %s\n",
            target);
        return false;
    }

    printf("Prepared IPv4 target: %s\n", target);
    destination.sin_family = AF_INET;

    if (inet_ntop(AF_INET, &destination.sin_addr,
            target_text, sizeof(target_text)) == NULL)
    {
        perror("ft_nmap: inet_ntop");
        return false;
    }

    if (!find_source_ipv4(destination.sin_addr, &source))
        return false;

    if (inet_ntop(AF_INET, &source,
            src_text, sizeof(src_text)) == NULL)
    {
        perror("ft_nmap: source inet_ntop");
        return false;
    }

    printf("Source IPv4: %s\n", src_text);

    int fd = open_tcp_sender(source);
    if (fd == -1)
        return false;

    printf("Raw TCP socket bound to %s\n", src_text);
    printf("Raw TCP socket ready for %s (fd=%d)\n", target_text, fd);

    size_t count = preview_scan_jobs(target, info);
    printf("Jobs: %zu\n", count);

    close(fd);
    return true;
}
