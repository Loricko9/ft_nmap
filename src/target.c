#include "ft_nmap.h"

bool prepare_ipv4_target(const char *text, struct in_addr *address)
{
    struct addrinfo hints;
    struct addrinfo *res = NULL;

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;
    int status = getaddrinfo(text, NULL, &hints, &res);
    if (status == 0 && res) {
        struct sockaddr_in *ipv4 = (struct sockaddr_in *)res->ai_addr;
        *address = ipv4->sin_addr;
        freeaddrinfo(res);
        return true;
    }
    return (inet_pton(AF_INET, text, address) == 1);
}

bool find_source_ipv4(struct in_addr target, struct in_addr *source) {
    struct sockaddr_in remote = {0};
    struct sockaddr_in local = {0};
    socklen_t local_len = sizeof(local);

    int route_fd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

    if (route_fd == -1) {
        perror("ft_nmap: route socket");
        return false;
    }

    remote.sin_family = AF_INET;
    remote.sin_addr = target;
    remote.sin_port= htons(9);

    if (connect(route_fd, (struct sockaddr *)&remote, sizeof(remote)) == -1) {
        perror("ft_nmap: route connect");
        close(route_fd);
        return false;
    }

    if (getsockname(route_fd, (struct sockaddr *)&local, &local_len) == -1) {
        perror("ft_nmap: getsockname");
        close(route_fd);
        return false;
    }

    *source = local.sin_addr;
    close(route_fd);
    return true;
}
