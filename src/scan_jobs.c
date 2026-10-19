#include "ft_nmap.h"

void *preview_scan_jobs(void *arg) {
    t_thread_data *data = (t_thread_data *)arg;

    int fd = open_tcp_sender(data->source);
    if (fd == -1)
        return false;

    while (1) {
        int port = atomic_fetch_add(&current_port, 1);
        if (port >= MAX_PORTS)
            break;

        if (!is_port_set(data->port_bitmap, port))
            continue;
        for (int scan = 0; scan < SCAN_COUNT; scan++) {
            if (!data->types[scan])
                continue;
            printf("%s:%u %s\n",
                data->target,
                (unsigned int)port,
                scan_types[scan]);

            if (scan == SCAN_SYN) {
                uint8_t tcp[TCP_HEADER_SIZE];

                build_syn_header(tcp, 40000, port, 1);
                finalize_tcp_checksum(tcp, data->source, data->destination);
                for (size_t k = 0; k < sizeof(tcp); k++) {
                    printf(" %02x", (unsigned int)tcp[k]);
                }
                printf("\n");
            }
        }
    }
    close(fd);
    return NULL;
}
