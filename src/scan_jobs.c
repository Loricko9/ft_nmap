#include "ft_nmap.h"

size_t preview_scan_jobs(const char *target, const t_info *info) {
    static const char *names[SCAN_COUNT] = {
        "SYN", "NULL", "FIN", "XMAS", "ACK", "UDP"
    };
    size_t count = 0;

    for (int port = 1; port < MAX_PORTS; port++) {
        if (!is_port_set(info->port_bitmap, port))
            continue;

        for (int scan = 0; scan < SCAN_COUNT; scan++) {
            if (!info->types[scan])
                continue;

            t_scan_job job = {
                .target = target,
                .port = (uint16_t)port,
                .type = (t_scan_type)scan
            };

            printf("%s:%u %s\n",
                job.target,
                (unsigned int)job.port,
                names[job.type]);
            count++;
        }
    }
    return count;
}
