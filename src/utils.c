/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lle-saul <lle-saul@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/21 10:23:24 by lle-saul          #+#    #+#             */
/*   Updated: 2026/09/21 10:23:24 by lle-saul         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_nmap.h"

void	ft_init_info(t_info *info)
{
	info->ip = NULL;
	info->file = NULL;
	info->port_start = -1;
	info->port_end = -1;
	info->nb_threads = 1;
	for (int i = 0; i < 6; i++)
		info->types[i] = false;
}

void free_tab(char **tab)
{
    if (!tab)
		return;
	for (int i = 0; tab[i]; i++)
        free(tab[i]);
    free(tab);
}
