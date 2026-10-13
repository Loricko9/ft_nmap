/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_nmap.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kquetat- <kquetat-@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/21 09:52:13 by lle-saul          #+#    #+#             */
/*   Updated: 2026/10/13 21:55:51 by kquetat-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_NMAP_H
# define FT_NMAP_H

# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <unistd.h>
# include <stdbool.h>
# include <stdint.h>
# include <ctype.h>
# include <arpa/inet.h>
# include <sys/socket.h>
# include <netinet/in.h>

#define MAX_PORTS 65536
#define BITMAP_SIZE (MAX_PORTS / 8)

typedef struct s_info
{
	char	*ip;
	char	*file;
	uint8_t	port_bitmap[BITMAP_SIZE];
	size_t	nb_threads;
	bool	types[6];
}	t_info;

typedef void (*FuncParser)(char *, t_info *);

/* --- scanning section --- */

typedef enum e_scan_type {
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
    const char  *target;
    uint16_t    port;
    t_scan_type type;
}   t_scan_job;

/* --- end of scanning section --- */

/*parser.c*/
void	check_flags(int ac, char **av, t_info *info);
bool 	is_port_set(const uint8_t *bitmap, int port);
bool	set_port(uint8_t *bitmap, int port);

/*parser_file.c*/
char	**read_host_file(char *filename);

/*ft_slpit.c*/
char **ft_split(char *str, char *sep);

/*utils.c*/
void	ft_init_info(t_info *info);
void	free_tab(char **tab);
int		tab_len(char **tab);

/*scan_jobs.c*/
size_t preview_scan_jobs(const char *target, const t_info *info);

/*target.c*/
bool prepare_ipv4_target(const char *text, struct in_addr *address);
bool find_source_ipv4(struct in_addr target, struct in_addr *source);

#endif
