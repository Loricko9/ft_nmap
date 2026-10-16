/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_nmap.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kquetat- <kquetat-@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/21 09:52:13 by lle-saul          #+#    #+#             */
/*   Updated: 2026/10/14 16:52:13 by kquetat-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_NMAP_H
#define FT_NMAP_H

#include <arpa/inet.h>
#include <ctype.h>
#include <netinet/in.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <unistd.h>
# include <stdbool.h>
# include <stdint.h>
# include <ctype.h>
# include <pthread.h>
# include <time.h>
# include <stdatomic.h>

#define MAX_PORTS 65536
#define BITMAP_SIZE (MAX_PORTS / 8)

#define TCP_HEADER_SIZE 20
#define TCP_FLAG_SYN 0x02

void build_syn_header(uint8_t header[TCP_HEADER_SIZE], uint16_t source_port,
                      uint16_t destination_port, uint32_t sequence);

typedef struct s_info
{
    char *ip;
    char *file;
    uint8_t port_bitmap[BITMAP_SIZE];
    size_t nb_threads;
    bool types[6];
} t_info;

typedef void (*FuncParser)(char *, t_info *);

/* --- scanning section --- */

typedef enum e_scan_type
{
    SCAN_SYN = 0,
    SCAN_NULL,
    SCAN_FIN,
    SCAN_XMAS,
    SCAN_ACK,
    SCAN_UDP,
    SCAN_COUNT
} t_scan_type;

typedef struct s_scan_job
{
    const char *target;
    uint16_t port;
    t_scan_type type;
} t_scan_job;

/* --- end of scanning section --- */

/*parser.c*/
void check_flags(int ac, char **av, t_info *info);
bool is_port_set(const uint8_t *bitmap, int port);
bool set_port(uint8_t *bitmap, int port);

/*parser_file.c*/
char **read_host_file(char *filename);

/*ft_slpit.c*/
char **ft_split(char *str, char *sep);

/*utils.c*/
void ft_init_info(t_info *info);
void free_tab(char **tab);
int tab_len(char **tab);

/*scan_jobs.c*/
size_t preview_scan_jobs(const char *target, const t_info *info);

/*target.c*/
bool prepare_ipv4_target(const char *text, struct in_addr *address);
bool find_source_ipv4(struct in_addr target, struct in_addr *source);

/* sender.c */
int open_tcp_sender(struct in_addr source);

/* scan.c */
bool preview_target(const char *target, const t_info *info);

/*checksum.c*/
uint16_t internet_checksum(const uint8_t *data, size_t length);

#endif
