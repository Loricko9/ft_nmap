/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_nmap.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kquetat- <kquetat-@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/21 09:52:27 by lle-saul          #+#    #+#             */
/*   Updated: 2026/10/13 19:19:52 by kquetat-         ###   ########.fr       */
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
		// printf("Scanning IP: %s\n", ip_lst[i]);

		struct in_addr address;

		if (!prepare_ipv4_target(ip_lst[i], &address)) {
			fprintf(stderr, "ft_nmap: Cannot prepare IPv4 target: %s\n",
				ip_lst[i]);
			free_tab(ip_lst);
			return EXIT_FAILURE;
		}

		printf("Prepared IPv4 target: %s\n", ip_lst[i]);
		struct sockaddr_in destination = {0};

		destination.sin_family = AF_INET;
		destination.sin_addr = address;

		char printable[INET_ADDRSTRLEN];
		if (inet_ntop(AF_INET, &destination.sin_addr, printable, sizeof(printable)) == NULL) {
			perror("ft_nmap: inet_ntop");
			free_tab(ip_lst);
			return EXIT_FAILURE;
		}

		int send_fd = socket(AF_INET, SOCK_RAW, IPPROTO_TCP);
		if (send_fd == -1) {
			perror("ft_nmap: socket");
			free_tab(ip_lst);
			return EXIT_FAILURE;
		}
		printf("Raw TCP socket ready for %s (fd=%d)\n", printable, send_fd);

		size_t count = preview_scan_jobs(ip_lst[i], &info);
		printf("Jobs: %zu\n", count);
		close(send_fd);
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
