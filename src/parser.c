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

void	parse_help(char *arg, t_info *info)
{
	(void)info;
	(void)arg;
	printf("Help Screen\n");
	printf("ft_nmap [OPTIONS]\n");
	printf("--help Print this help screen\n");
	printf("--ports ports to scan (eg: 1-10 or 1,2,3 or 1,5-15)\n");
	printf("--ip ip addresses to scan in dot format\n");
	printf("--file File name containing IP addresses to scan,\n");
	printf("--speedup [250 max] number of parallel threads to use\n");
	printf("--scan SYN/NULL/FIN/XMAS/ACK/UDP\n");
	exit(0);
}

void	parse_ports(char *arg, t_info *info)
{
	(void)arg;
	(void)info;
}

void	parse_ip(char *arg, t_info *info)
{
	info->ip = arg;
}

void	parse_file(char *arg, t_info *info)
{
	info->file = arg;
}

void	parse_speedup(char *arg, t_info *info)
{
	info->nb_threads = strtol(arg, NULL, 10);
	if (info->nb_threads < 1 || info->nb_threads > 250) {
		printf("ft_nmap: Invalid number of threads: %zu\n", info->nb_threads);
		exit(1);
	}
}

void	parse_scan(char *arg, t_info *info)
{
	char *scan_types[] = {"SYN", "NULL", "FIN", "XMAS", "ACK", "UDP"};
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
	char *flag[] = {"--help", "--ports", "--ip", "--file", "--speedup", "--scan"};
	FuncParser parser[] = {&parse_help, &parse_ports, &parse_ip, &parse_file, &parse_speedup, &parse_scan};
	
	bool found_flag;
	for (int i = 1; i < ac; i++)
	{
		found_flag = false;
		for(int j = 0; j < 6; j++) {
			if (strcmp(av[i], flag[j]) == 0) {
				parser[j](*av, info);
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
