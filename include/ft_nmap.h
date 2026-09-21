/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_nmap.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lle-saul <lle-saul@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/21 09:52:13 by lle-saul          #+#    #+#             */
/*   Updated: 2026/09/21 09:52:13 by lle-saul         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_NMAP_H
# define FT_NMAP_H

# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <unistd.h>
# include <stdbool.h>

typedef struct s_info
{
	char	*ip;
	char	*file;
	ssize_t	port_start;
	ssize_t	port_end;
	size_t	nb_threads;
	bool	types[6];
}	t_info;

typedef void (*FuncParser)(char *, t_info *);

/*parser.c*/
void	check_flags(int ac, char **av, t_info *info);

/*ft_slpit.c*/
char **ft_split(char *str, char *sep);

/*utils.c*/
void	ft_init_info(t_info *info);
void	free_tab(char **tab);

#endif

