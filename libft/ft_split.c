/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clorin <clorin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/22 15:24:16 by clorin            #+#    #+#             */
/*   Updated: 2020/09/22 15:50:15 by clorin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static char		*ft_strndup_split(const char *s, size_t n)
{
	char		*str;
	size_t		i;

	i = 0;
	str = (char *)malloc(sizeof(char) * n + 1);
	if (!str)
		return (NULL);
	ft_bzero(str, n + 1);
	while (s[i] && i < n)
	{
		str[i] = s[i];
		i++;
	}
	return (str);
}

static int		nb_word(char const *str, char c)
{
	size_t		i;
	int			cmpt;

	i = 0;
	cmpt = 0;
	while (str[i])
	{
		while (str[i] == c)
			i++;
		if (str[i])
			cmpt++;
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

static char		**ft_fill_words(char **dest, char const *s, char c)
{
	int			i;
	int			y;
	int			j;

	i = 0;
	y = 0;
	while (s[i])
	{
		while (s[i] == c)
			i++;
		j = i;
		while (s[i] && s[i] != c)
			i++;
		if (i > j)
		{
			dest[y++] = ft_strndup_split(s + j, i - j);
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

char			**ft_split(char const *s, char c)
{
	char		**dest;

	if (!s)
		return (NULL);
	dest = (char **)malloc(sizeof(char *) * (nb_word(s, c) + 1));
	if (!dest)
		return (NULL);
	return (ft_fill_words(dest, s, c));
}
