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

                if (job.type == SCAN_SYN) {

                const uint8_t sample[] = {0x07, 0x08, 0xA, 0x01};

                printf("Checksum sample: %04x\n",
                    (unsigned int)internet_checksum(sample, sizeof(sample)));

                uint8_t tcp[TCP_HEADER_SIZE];

                build_syn_header(tcp, 40000, job.port, 1);
                for (size_t k = 0; k < sizeof(tcp); k++) {
                    printf(" %02x", (unsigned int)tcp[k]);
                }
                printf("\n");
            }
            count++;
        }
    }
    return count;
}
