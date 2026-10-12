/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lle-saul <lle-saul@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/21 10:24:56 by lle-saul          #+#    #+#             */
/*   Updated: 2026/09/21 10:24:56 by lle-saul         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_nmap.h"

static void	parse_help(char *arg, t_info *info)
{
	(void)info;
	(void)arg;
	printf("Help Screen\n");
	printf("ft_nmap [OPTIONS]\n");
	printf("--help\tPrint this help screen\n");
	printf("--ports\tports to scan (eg: 1-10 or 1,2,3 or 1,5-15)\n");
	printf("--ip\tip addresses to scan in dot format\n");
	printf("--file\tFile name containing IP addresses to scan,\n");
	printf("--speedup\t[250 max] number of parallel threads to use\n");
	printf("--scan\tSYN/NULL/FIN/XMAS/ACK/UDP\n");
	exit(0);
}

bool	set_port(uint8_t *bitmap, int port)
{
    if (port >= 1 && port < MAX_PORTS) {
        bitmap[port / 8] |= (1 << (port % 8));
		return (true);
    }
	return (false);
}

bool	is_port_set(const uint8_t *bitmap, int port)
{
    if (port >= 1 && port < MAX_PORTS) {
        return (bitmap[port / 8] & (1 << (port % 8))) != 0;
    }
    return (false);
}

static void	port_error(char *str, char **ports_list, char **ports)
{
	printf("ft_nmap: Invalid port number: %s\n", str);
	free_tab(ports_list);
	free_tab(ports);
	exit(1);
}

static void	parse_ports(char *arg, t_info *info)
{
	for (int i = 0; arg[i]; i++) {
		if (!((arg[i] >= '0' && arg[i] <= '9') || arg[i] == ',' || arg[i] == '-')) {
			printf("ft_nmap: Invalid port format: %s\n", arg);
			exit(1);
		}
		if (arg[i] == '-' || arg[i] == ',') {
			if (i == 0 || arg[i + 1] == '\0' || !isdigit(arg[i - 1]) || !isdigit(arg[i + 1])) {
				printf("ft_nmap: Invalid port range format: %s\n", arg);
				exit(1);
			}
		}
	}
	
	char	**ports = ft_split(arg, ",");
	for (int i = 0; ports[i]; i++) {
		char	**ports_list = ft_split(ports[i], "-");
		switch (tab_len(ports_list)) {
			case 1:
				if (set_port(info->port_bitmap, strtol(ports_list[0], NULL, 10)) == false)
					port_error(ports[i], ports_list, ports);
				break;
			case 2:
				for (int start = strtol(ports_list[0], NULL, 10), end = strtol(ports_list[1], NULL, 10); start <= end; start++) {
					if (set_port(info->port_bitmap, start) == false)
						port_error(ports[i], ports_list, ports);
				}
				break;
			default:
				port_error(ports[i], ports_list, ports);
		}
		free_tab(ports_list);
	}
	free_tab(ports);

}

static void	parse_ip(char *arg, t_info *info)
{
	info->ip = arg;
}

static void	parse_file(char *arg, t_info *info)
{
	info->file = arg;
}

static void	parse_speedup(char *arg, t_info *info)
{
	info->nb_threads = strtol(arg, NULL, 10);
	if (info->nb_threads < 1 || info->nb_threads > 250) {
		printf("ft_nmap: Invalid number of threads: %zu\n", info->nb_threads);
		exit(1);
	}
}

static void	parse_scan(char *arg, t_info *info)
{
	char *scan_types[] = {"SYN", "NULL", "FIN", "XMAS", "ACK", "UDP", NULL};
	char **types = ft_split(arg, "/, ");

	bool found;
	for (int i = 0; types[i]; i++) {
		found = false;
		for (int j = 0; scan_types[j]; j++) {
			if (strcmp(types[i], scan_types[j]) == 0) {
				info->types[j] = true;
				found = true;
				break ;
			}
		}
		if (!found) {
			printf("ft_nmap: Invalid scan type: %s\n", types[i]);
			free_tab(types);
			exit(1);
		}
	}
	free_tab(types);
}

void	check_flags(int ac, char **av, t_info *info)
{
	char *flag[] = {"--help", "--ports", "--ip", "--file", "--speedup", "--scan", NULL};
	FuncParser parser[] = {&parse_help, &parse_ports, &parse_ip, &parse_file, &parse_speedup, &parse_scan};
	
	bool found_flag;
	for (int i = 1; i < ac; i++)
	{
		found_flag = false;
		for (int j = 0; j < 6; j++) {
			if (strcmp(av[i], flag[j]) == 0) {
				parser[j](av[i + 1], info);
				found_flag = true;
				i++;
				break;
			}
		}
		if (av[i][0] == '-' && !found_flag) {
			printf("ft_nmap: Unknown flag: %s\n", av[i]);
			exit(1);
		}
		else if (!found_flag) {
			printf("ft_nmap: Unknown argument: %s\n", av[i]);
			exit(1);
		}
	}
}
