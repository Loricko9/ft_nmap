/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.C                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lle-saul <lle-saul@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/21 13:30:28 by lle-saul          #+#    #+#             */
/*   Updated: 2026/09/21 13:30:28 by lle-saul         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_nmap.h"

static int is_sep(char c, char *sep)
{
    while (*sep) {
        if (c == *sep)
            return (1);
        sep++;
    }
    return (0);
}

static int count_words(char *str, char *sep)
{
    int count = 0;
    int in_word = 0;

    while (*str) {
        if (!is_sep(*str, sep) && !in_word) {
            in_word = 1;
            count++;
        }
		else if (is_sep(*str, sep)) {
            in_word = 0;
        }
        str++;
    }
    return (count);
}

static char *copy_word(char *str, int len)
{
    char *word = malloc(sizeof(char) * (len + 1));
    if (!word)
        return (NULL);
    for (int i = 0; i < len; i++)
        word[i] = str[i];
    word[len] = '\0';
    return (word);
}

static void free_tab_count(char **tab, int count)
{
    for (int i = 0; i < count; i++)
        free(tab[i]);
    free(tab);
}

char **ft_split(char *str, char *sep)
{
    if (!str || !sep)
        return (NULL);

    int words = count_words(str, sep);
    char **tab = malloc(sizeof(char *) * (words + 1));
    if (!tab)
        return (NULL);

    int i = 0;
    while (*str) {
        if (!is_sep(*str, sep)) {
            char *start = str;
            int len = 0;
            while (*str && !is_sep(*str, sep)) {
                len++;
                str++;
            }
            tab[i] = copy_word(start, len);
            if (!tab[i]) {
                free_tab_count(tab, i);
                return (NULL);
            }
            i++;
        }
		else {
            str++;
        }
    }
    tab[i] = NULL;
    return (tab);
}