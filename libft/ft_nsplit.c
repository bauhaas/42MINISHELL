/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_nsplit.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clorin <clorin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/09 11:42:38 by clorin            #+#    #+#             */
/*   Updated: 2021/04/09 11:42:45 by clorin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int		nb_word(char const *str, char c, int nb)
{
	size_t		i;
	int			cmpt;

	i = 0;
	cmpt = 0;
	while (str[i] && nb >= 0)
	{
		while (str[i] == c)
			i++;
		if (str[i])
		{
			cmpt++;
			nb--;
		}
		while (str[i] && str[i] != c)
			i++;
	}
	return (cmpt);
}

static void		free_split(char **dest, int y)
{
	while (y >= 0)
	{
		free(dest[y]);
		y--;
	}
	free(dest);
}

static int		next(char const *s, int i, int n, char c)
{
	if (n == 0)
		i = ft_strlen(s);
	else
	{
		while (s[i] && s[i] != c)
			i++;
	}
	return (i);
}

static char		**ft_fill_words(char **dest, char const *s, char c, int n)
{
	int			i;
	int			y;
	int			j;

	i = 0;
	y = 0;
	while (s[i] && n >= 0)
	{
		while (s[i] == c && n--)
			i++;
		j = i;
		i = next(s, i, n, c);
		if (i > j)
		{
			dest[y++] = ft_strndup(s + j, i - j);
			if (!dest[y - 1])
			{
				free_split(dest, y - 2);
				return (NULL);
			}
		}
	}
	dest[y] = NULL;
	return (dest);
}

char			**ft_nsplit(char const *s, char c, int n)
{
	char		**dest;

	if (!s || n == 0)
		return (NULL);
	dest = (char **)malloc(sizeof(char *) * (nb_word(s, c, n) + 1));
	if (!dest)
		return (NULL);
	return (ft_fill_words(dest, s, c, n));
}
