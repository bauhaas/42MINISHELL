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


/*
\\ - backslash
\a - alert (BEL)
\b - backspace
\c - produce no further output
\e - escape
\f - form feed
\n - new line
\r - carriage return
\t - horizontal tab
\v - vertical tab
\0NNN - byte with octal value NNN (1 to 3 digits)
\xHH - byte with hexadecimal value HH (1 to 2 digits)
*/
#include "../includes/minishell.h"

int			is_quote(const int c)
{
	return (c == 39 || c== 34);
}

/*
**	This function returns TRUE if the character c at position POS is preceded by /
*/
int			is_echaped(const char *str, const int c, int pos)
{
	if (pos > 0 && pos < ft_strlen(str))
	{
		return (((int)str[pos - 1] == 92) && (int)str[pos] == c);
	}
	return (FALSE);
}

int			next_quote(const char *str, int c, int i)
{
	if (i >= ft_strlen(str))
		return (ft_strlen(str));
	while (str[i])
	{
		if ((int)str[i] == c && !is_echaped(str, c, i))
			return i;
		i++;
	}
	return (ft_strlen(str));
}

/*
**	This function returns TRUE if pos is in a string surrounded by quotes.
**	False otherwise.
*/
int			is_in_quotes(const char *str, int pos)
{
	int		i;
	int		j;
	int		c;
	//int		quote;

	i = 0;
	//quote = FALSE;
	//printf("debut ");
	if (!str || *str == '\0')
		return (FALSE);
	//printf("\n");
	while (str[i])
	{
		c = 0;
		//printf("%d =>%c ",i, str[i]);
		while (str[i] && !is_quote(str[i]))
		{
			i++;
			//printf(".");
		}
		//printf("x");
		if (!str[i] || pos <= i)
			return (FALSE);
		//printf("%d in [%d", pos, i);
		c = str[i++];
		j = next_quote(str, c, i);
		//printf(",%d[\n", j);
		if (pos >= i && pos < j)
			return (TRUE);
		i = j + 1;
	}
	return (FALSE);
}



/*
** This function checks the number of " or '.
returns FALSE if the first quote is odd. TRUE if even.
*/

int			valid_quotes(const char *str)
{
	int		i;
	int		j;
	int		cmpt;
	char	c;

	i = 0;
	cmpt = 0;
	if (!str || *str == '\0')
		return (TRUE);
	while (str[i])
	{
		c = '\0';
		while (str[i] && (!is_quote(str[i]) || is_echaped(str, str[i], i)))
			i++;
		if (!str[i])
			return (TRUE);
		c = str[i++];
		j = next_quote(str, c, i);
		if (j == ft_strlen(str))
			return (FALSE);
		i = j + 1;
	}
	return (TRUE);
}

int			quote_in_quotes(const char *str, int pos)
{
	if (pos >= 0 && pos < ft_strlen(str))
	{
		return (is_quote(str[pos]) && is_in_quotes(str, pos));
	}
	return (FALSE);
}

int			space_in_quotes(const char *str, int pos)
{
	if (pos >=0 && pos < ft_strlen(str))
	{
		return (str[pos] == ' ' && is_in_quotes(str, pos));
	}
	return (FALSE);
}

static char		*ft_strndup_split_(const char *s, size_t n)
{
	char		*str;
	size_t		i;
	int			j;

	i = 0;
	j = 0;
	str = (char *)malloc(sizeof(char) * n + 1);
	if (!str)
		return (NULL);
	ft_bzero(str, n + 1);
	while (s[i] && i < n)
		str[j++] = s[i++];
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
		while ((s[i] == ' ' && !space_in_quotes(s, i))
			||(is_quote(s[i]) && !quote_in_quotes(s, i)))
			i++;
		j = i;
		while (s[i] && (s[i] != ' ' || space_in_quotes(s, i))
			&& (!is_quote(s[i]) || quote_in_quotes(s, i)))
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

static int		nb_word_(char const *s)
{
	size_t		i;
	int			cmpt;

	i = 0;
	cmpt = 0;
	while (s[i])
	{
		while ((s[i] == ' ' && !space_in_quotes(s, i))
			||(is_quote(s[i]) && !quote_in_quotes(s, i)))
			i++;
		if (s[i])
			cmpt++;
		while (s[i] && (s[i] != ' ' || space_in_quotes(s, i))
			&& (!is_quote(s[i]) || quote_in_quotes(s, i)))
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
	dest = (char **)malloc(sizeof(char *) * (nb_word + 1));
	if (!dest)
		return(NULL);
	return (ft_fill_words_(dest, str));
}