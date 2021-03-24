/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clorin <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/24 13:13:22 by clorin            #+#    #+#             */
/*   Updated: 2021/03/24 13:13:27 by clorin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int			is_quote(const char c)
{
	return (c == '\'' || c== '\"');
}

int			next_quote(const char *str, char c, int i)
{
	if (i >= ft_strlen(str))
		return (ft_strlen(str));
	while (str[i])
	{
		if (str[i] == c)
			return i;
		i++;
	}
	return (ft_strlen(str));
}

/*
**	This function returns TRUE if pos is in a string surrounded by quotes.
**	False otherwise.
*/
int			is_in_quote(const char *str, int pos)
{
	int		i;
	int		j;
	char	c;
	int		quote;

	i = 0;
	quote = FALSE;
	if (!str || *str == '\0')
		return (FALSE);

	/**
	on boucle pour trouver la premier quote
	on stock c
	quote = true
	on boucle pour trouver la fin de la quote
	 si pos dqns l'interval => True
	 sinon
	 	on continue
	*/
	while (str[i])
	{
		c = '\0';
		while (str[i] && !is_quote(str[i]))
			i++;
		if (!str[i] || pos <= i)
			return (FALSE);
		c = str[i++];
		j = next_quote(str, c, i) - 1;
		//printf("%d in [%d,%d] -> ", pos, i,j);
		if (pos >= i && pos <= j)
		{
			//printf("TRUE\n");
			return (TRUE);
		}
		//printf("FALSE\n");
		i = j + 2;
	}
	return (FALSE);
}
/*
** This function checks the number of " or '.
returns FALSE if the first quote is odd. TRUE if even.
*/
int			valid_quote(const char *str)
{
	int		i;
	int		cmpt;
	char	c;

	i = 0;
	cmpt = 0;
	if (!str || *str == '\0')
		return (TRUE);
	while (str[i] && str[i] != '\"' && str[i] != '\'')
		i++;
	if (!str[i])
		return (TRUE);
	cmpt++;
	c = str[i++];
	while (str[i])
	{
		if (str[i] == c)
			cmpt++;
		i++;
	}
	return ((cmpt % 2) == 0);
}

int			quote_in_quote(const char *str, int pos)
{
	if (pos >= 0 && pos < ft_strlen(str))
	{
		return (is_quote(str[pos]) && is_in_quote(str, pos));
	}
	return (FALSE);
}

int			space_in_quote(const char *str, int pos)
{
	if (pos >=0 && pos < ft_strlen(str))
	{
		return (str[pos] == ' ' && is_in_quote(str, pos));
	}
	return (FALSE);
}

static char		*ft_strndup_split_(const char *s, size_t n)
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

static void		free_split_(char **dest, int y)
{
	while (y >= 0)
	{
		free(dest[y]);
		y--;
	}
	free(dest);
}

static char		**ft_fill_words_(char **dest, char const *s)
{
	int			i;
	int			y;
	int			j;

	i = 0;
	y = 0;
	while (s[i])
	{
		while ((s[i] == ' ' && !space_in_quote(s, i))
			||(is_quote(s[i]) && !quote_in_quote(s, i)))
			i++;
		j = i;
		while (s[i] && (s[i] != ' ' || space_in_quote(s, i))
			&& (!is_quote(s[i]) || quote_in_quote(s, i)))
			i++;
		if (i > j)
		{
			dest[y++] = ft_strndup_split_(s + j, i - j);
			if (!dest[y - 1])
			{
				free_split_(dest, y - 2);
				return (NULL);
			}
		}
	}
	dest[y] = NULL;
	return (dest);
}

static int		nb_word_(char const *str)
{
	size_t		i;
	int			cmpt;

	i = 0;
	cmpt = 0;
	while (str[i])
	{
		while (str[i] == ' ' && !space_in_quote(str, i))
			i++;
		if (str[i])
			cmpt++;
		while (str[i] && (str[i] != ' ' || space_in_quote(str,i)))
			i++;
	}
	return (cmpt);
}

char		**parse(const char *str)
{
	int		i;
	char	**dest;
	int		nb_word;

	i = 0;
	if (!str)
		return (NULL);
	nb_word = nb_word_(str);
	//printf("Il y a %d mots dans la chaine\n", nb_word);
	dest = (char **)malloc(sizeof(char *) * (nb_word + 1));
	if (!dest)
		return(NULL);
	return (ft_fill_words_(dest, str));
	while (str[i])
	{
		if((!ft_isspace(str[i]) && !is_quote(str[i])) || is_in_quote(str, i))
			printf("%c", str[i]);
		i++;
	}
}