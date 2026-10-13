/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_file.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lle-saul <lle-saul@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/12 12:59:59 by lle-saul          #+#    #+#             */
/*   Updated: 2026/10/12 12:59:59 by lle-saul         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_nmap.h"

char	**read_host_file(char *filename)
{
	FILE *file = fopen(filename, "r");
	if (!file) {
		printf("ft_nmap: Could not open file: %s\n", filename);
		exit(1);
	}

	char *line = NULL;
	size_t len = 0;
	ssize_t read;

	char **ip_list = NULL;
	size_t ip_count = 0;

	while ((read = getline(&line, &len, file)) != -1) {
		line[strcspn(line, "\r\n")] = 0;
		char **split_line = ft_split(line, " ");
		if (!split_line)
			return (NULL);
		if (ip_list)
			ip_list = realloc(ip_list, sizeof(char *) * (ip_count + tab_len(split_line) + 1));
		else
			ip_list = malloc(sizeof(char *) * (tab_len(split_line) + 1));
		for (int i = 0; split_line[i]; i++) {
			ip_list[ip_count] = strdup(split_line[i]);
			if (!ip_list[ip_count]) {
				printf("ft_nmap: Memory allocation error\n");
				free_tab(split_line);
				free_tab(ip_list);
				exit(1);
			}
			ip_count++;
		}
		ip_list[ip_count] = NULL;
		free_tab(split_line);
	}

	free(line);
	fclose(file);
	return (ip_list);
}