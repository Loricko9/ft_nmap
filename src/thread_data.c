/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   thread_data.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lle-saul <lle-saul@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/14 10:08:31 by lle-saul          #+#    #+#             */
/*   Updated: 2026/10/14 11:21:02 by lle-saul         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_nmap.h"

bool	convert_hostname(struct sockaddr_in *ip_addr, char *address)
{
	struct addrinfo	hint;
	struct addrinfo	*res;
	int				ret;

	memset(&hint, 0, sizeof(hint));
	hint.ai_family = AF_INET;
	hint.ai_socktype = SOCK_RAW;
	ret = getaddrinfo(address, NULL, &hint, &res);
	if (ret != 0)
	{
		printf("ft_ping: unknown host\n");
		return (true);
	}
	memcpy(ip_addr, res->ai_addr, sizeof(struct sockaddr_in));
	freeaddrinfo(res);
	return (false);
}

bool	check_ip(struct sockaddr_in *ip_addr, char *address)
{
	memset(ip_addr, 0, sizeof(*ip_addr));
	ip_addr->sin_family = AF_INET;
	if (inet_pton(AF_INET, address, &ip_addr->sin_addr) <= 0)
		if (convert_hostname(ip_addr, address))
			return (true);
	return (false);
}

t_thread_data	init_thread_data(t_info *info, char *host)
{
	t_thread_data	thread_data;

	memset(&thread_data, 0, sizeof(t_thread_data));
	thread_data.host = host;
	memcpy(thread_data.port_bitmap, info->port_bitmap, BITMAP_SIZE);
	memcpy(thread_data.types, info->types, sizeof(info->types));
	
	
	return (thread_data);
}
