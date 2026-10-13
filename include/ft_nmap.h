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
# include <stdint.h>
# include <ctype.h>
# include <pthread.h>
# include <time.h>
# include <stdatomic.h>

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

#endif

