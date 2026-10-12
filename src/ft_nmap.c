/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_nmap.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lle-saul <lle-saul@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/21 09:52:27 by lle-saul          #+#    #+#             */
/*   Updated: 2026/09/21 09:52:27 by lle-saul         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_nmap.h"

int	main(int ac, char **av)
{
	t_info	info;

	if (ac < 2)
		return (printf("Usage: ft_nmap [--help] [--ports [NUMBER/RANGED]] --ip IP_ADDRESS [--speedup [NUMBER]] [--scan [TYPE]]\n"), 1);
	ft_init_info(&info);
	check_flags(ac, av, &info);
	if (!info.ip && !info.file)
		return (printf("ft_nmap: No IP address or file specified\n"), 1);
	if (info.ip && info.file)
		return (printf("ft_nmap: Cannot specify both IP address and file\n"), 1);
	
	char	**ip_lst = NULL;
	if (info.ip) {
		ip_lst = malloc(sizeof(char *) * 2);
		if (!ip_lst)
			return (printf("ft_nmap: Memory allocation error\n"), 1);
		ip_lst[0] = strdup(info.ip);
		ip_lst[1] = NULL;
	}
	else {
		ip_lst = read_host_file(info.file);
		if (!ip_lst)
			return (printf("ft_nmap: Memory allocation error\n"), 1);
	}
	
	for (int i = 0; ip_lst[i]; i++) {
		printf("Scanning IP: %s\n", ip_lst[i]);
	}

	free_tab(ip_lst);

	// printf("IP: %s\n", info.ip);
	// printf("File: %s\n", info.file);
	// printf("Number of threads: %zu\n", info.nb_threads);
	// printf("Scan type: \n");
	// for (int i = 0; i < 6; i++) {
	// 	if (info.types[i]) {
	// 		switch (i) {
	// 			case 0: printf("SYN\n"); break;
	// 			case 1: printf("NULL\n"); break;
	// 			case 2: printf("FIN\n"); break;
	// 			case 3: printf("XMAS\n"); break;
	// 			case 4: printf("ACK\n"); break;
	// 			case 5: printf("UDP\n"); break;
	// 		}
	// 	}
	// }
	// printf("Port bitmap: \n");
	// for (int i = 0; i < MAX_PORTS; i++) {
	// 	if (is_port_set(info.port_bitmap, i))
	// 		printf("%d port is set\n", i);
	// }
	return (0);
}