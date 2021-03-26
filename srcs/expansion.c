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

/*
**	if c is a quote return his ascii code, 0 otherwise
*/
#include "../includes/minishell.h"

int				is_quote(const int c)
{
	if (c == 39)
		return (39);
	if (c == 34)
		return (34);
	return (FALSE);
}

/*
**	This function returns TRUE if the character c at position POS is preceded by /
*/
int				is_echaped(const char *str, const int c, int pos)
{
	if (pos > 0 && pos < ft_strlen(str))
	{
		return (((int)str[pos - 1] == 92) && (int)str[pos] == c);
	}
	return (FALSE);
}

/*
**	this function return the first quote cast in int or 0 if nothing
**	the *pos is the position of this quote.
*/
int			first_quote(const char *str, int *pos)
{
	if (!str || !str[0])
		return (0);
	while (*pos < ft_strlen(str))
	{
		if (is_quote((int)str[*pos]))
			return ((int)str[*pos]);
		(*pos)++;
	}
	*pos = 0;
	return (0);
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
**	This function returns ascii code of the quote if pos is in
**	a string surrounded by quotes.
**	False otherwise.
*/
int			is_in_quotes(const char *str, int pos)
{
	int		begin;
	int		end;
	int		quote;

	begin = 0;
	quote = TRUE;
	if (!str || *str == '\0' || pos == ft_strlen(str) - 1)
		return (FALSE);
	while (str[begin] && quote)
	{
		quote = first_quote(str, &begin);
		if (quote && !is_echaped(str, quote, begin))
		{
			begin++;
			end = next_quote(str, quote, begin) - 1;
			//printf("[%d,%d]\n", begin, end);
			if (pos >= begin && pos <= end)
				return (quote);
			end  += 2;
		}
		begin = end;
	}
	return (FALSE);
}

/*
** This function checks the the string str
** if open return the ascii code of the quote
** otherwise 0 if closed
*/
int		valid_quotes(const char *str)
{
	int	i;
	int	open;

	i = 0;
	open = 0;
	while (str[i])
	{
		if (i > 0 && str[i - 1] == '\\')
			;
		else if (open == 0 && str[i] == '\"')
			open = 34;
		else if (open == 0 && str[i] == '\'')
			open = 39;
		else if (open == 34 && str[i] == '\"')
			open = 0;
		else if (open == 39 && str[i] == '\'')
			open = 0;
		i++;
	}
	return (open);
}
// int			valid_quotes(const char *str)
// {
// 	int		i;
// 	int		cmpt;

// 	i = 0;
// 	cmpt = 0;
// 	if (!str || *str == '\0')
// 		return (TRUE);
// 	while (str[i])
// 	{
// 		if (is_quote(str[i]) && !is_echaped(str, str[i], i)
// 			&& !quote_in_quotes(str, i))
// 			cmpt++;
// 		i++;
// 	}
// 	return ((cmpt % 2) == 0);
// }


/*
**	this function return True if in str[pos] it's a quote
**		- not echaped
**		- in a string with diff quote
*/
int			quote_in_quotes(const char *str, int pos)
{
	int		quote;
	if (pos >= 0 && pos < ft_strlen(str))
	{
		quote = is_quote(str[pos]);
		if (is_echaped(str,quote, pos))
			return (TRUE);
		int q = is_in_quotes(str, pos);
		//printf("quote = %c in %c\n", quote, q? q:'#');
		if (!q)
			return (FALSE);
		return (quote != q);
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

static char		*ft_strndup_split_(const char *s, int begin, int end)
{
	char		*str;
	size_t		i;
	int			j;
	int			quote;

	i = begin;
	j = 0;
	//printf("\nstrndup(%s,%d, %d)\n",s, begin, end);
	str = ft_strnew(end - begin);
	if (!str)
		return (NULL);
	while (s[i] && i < end)
	{
		quote = is_in_quotes(s, (int)i);
		if (quote)
		{
			//printf("On est dans une quote(%c) en %zu = %c\n", quote,i, s[i]);
			if (s[i] == '\\' && (int)s[i + 1] == quote)
				i++;
			else
				str[j++] = s[i++];
		}
		else if (!is_quote(str[i]))
			str[j++] = s[i++];
		else
			i++;
	}
	//printf("fin\n");
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
			dest[y++] = ft_strndup_split_(s, j, i);
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
	if (valid_quotes(str) > 0)
	{
		printf("minishell: syntax error with open quotes\n");
		return (NULL);
	}
	nb_word = nb_word_(str);
	dest = (char **)malloc(sizeof(char *) * (nb_word + 1));
	if (!dest)
		return(NULL);
	return (ft_fill_words_(dest, str));
}