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
**	" = 34	' = 39
*/
#include "../includes/minishell.h"

// int				is_quote(const int c)
// {
// 	if (c == 39)
// 		return (39);
// 	if (c == 34)
// 		return (34);
// 	return (FALSE);
// }

/*
**	This function returns TRUE if the character c at position POS is preceded by /
*/
// int				is_echaped(const char *str, const int c, int pos)
// {
// 	if (pos > 0 && pos < ft_strlen(str))
// 	{
// 		return (((int)str[pos - 1] == 92) && (int)str[pos] == c);
// 	}
// 	return (FALSE);
// }

/*
**	this function return the first quote cast in int or 0 if nothing
**	the *pos is the position of this quote.
*/
// int			first_quote(const char *str, int *pos)
// {
// 	if (!str || !str[0])
// 		return (0);
// 	while (*pos < ft_strlen(str))
// 	{
// 		if (is_quote((int)str[*pos]))
// 			return ((int)str[*pos]);
// 		(*pos)++;
// 	}
// 	*pos = 0;
// 	return (0);
// }

// int			next_quote(const char *str, int c, int i)
// {
// 	if (i >= ft_strlen(str))
// 		return (ft_strlen(str));
// 	while (str[i])
// 	{
// 		if ((int)str[i] == c && !is_echaped(str, c, i))
// 			return i;
// 		i++;
// 	}
// 	return (ft_strlen(str));
// }

/*
**	This function returns ascii code of the quote if pos is in
**	a string surrounded by quotes.
**	False otherwise.
*/
// int			is_in_quotes(const char *str, int pos)
// {
// 	int		begin;
// 	int		end;
// 	int		quote;

// 	begin = 0;
// 	quote = TRUE;
// 	if (!str || *str == '\0' || pos == ft_strlen(str) - 1)
// 		return (FALSE);
// 	while (str[begin] && quote)
// 	{
// 		quote = first_quote(str, &begin);
// 		if (quote && !is_echaped(str, quote, begin))
// 		{
// 			begin++;
// 			end = next_quote(str, quote, begin) - 1;
// 			//printf("[%d,%d]\n", begin, end);
// 			if (pos >= begin && pos <= end)
// 				return (quote);
// 			end  += 2;
// 		}
// 		begin = end;
// 	}
// 	return (FALSE);
// }

/*
** This function checks the the string str
** if open return the ascii code of the quote
** otherwise 0 if closed
*/
// int		valid_quotes(const char *str, int len)
// {
// 	int	i;
// 	int	open;

// 	i = 0;
// 	open = 0;
// 	while (str[i] && i <= len)
// 	{
// 		if (i > 0 && str[i - 1] == '\\')
// 			;
// 		else if (open == 0 && str[i] == '\"')
// 			open = 34;
// 		else if (open == 0 && str[i] == '\'')
// 			open = 39;
// 		else if (open == 34 && str[i] == '\"')
// 			open = 0;
// 		else if (open == 39 && str[i] == '\'')
// 			open = 0;
// 		//printf("pour i = %d => open = %d\n", i,open);
// 		i++;
// 	}
// 	return (open);
// }

/*
**	fonction qui recherche le nom d'une var après $
*/
char		*get_name_var(char *str)
{
	int		i;
	int		j;
	char	*dest;

	i = 0;
	if (!str || !*str)
		return (NULL);
	while (str[i] && (str[i]!= '$' || is_escaped('$', str, i)))
		i++;
	if (str[i++] == '$')
	{
		j = i;
		if (ft_isalpha(str[i++]))
		{
			while (str[i] && (ft_isalnum(str[i]) || str[i] == '_'))
				i++;
			dest = ft_strndup(str + j, i - j);
			return (dest);
		}
	}
	return (NULL);
}

int			len_substitut(char *str, t_ms *mini)
{
	int		len_total;
	int		i;
	char	*name;

	len_total = 0;
	i = 0;
	while (str[i])
	{
		name = get_name_var(str + i);
		if (str[i] == '$' && str[i + 1] == '?')
		{
			len_total += ft_strlen("TODO_$?");
			i++;
		}
		else if (name)
		{
			len_total += ft_strlen(ft_getenv(&mini->env, name, TRUE));
			i += ft_strlen(name);
		}
		else
			len_total++;	
		ft_strdel(&name);
		i++;
	}
	return (len_total);
}

char		*substitute(char *str, t_ms *mini)
{
	int		i;
	int		len_total_subs;
	int		j;
	char	*dest;
	char	*name;
	
	//printf("-- substitute string <%s> --\n", str);
	//printf("longueur au depart = %d\n", (int)ft_strlen(str));
	i = 0;
	// premier passe pour la longueur total
	//printf("first pass >>>\t");
	len_total_subs = len_substitut(str, mini);
	if (len_total_subs == 0)
		return (NULL);
	//printf("Longueur substitute = %d\n", (int)len_total_subs);
	dest = ft_strnew(len_total_subs);
	// deuxieme pass pour subsitute var
	//printf("Second pass >>>\n");
	i = 0;
	j = 0;
	while (str[i])
	{
		name = get_name_var(str + i);
		if (str[i] == '$' && str[i + 1] == '?')
		{
			ft_strcat(dest, "TODO_$?");
			j += ft_strlen("TODO_$?");
			i += 2;
		}
		else if (name && ft_getenv(&mini->env, name, TRUE))
		{
			//printf("ft_strcat(%s,%s)=>", dest, ft_getenv(&mini->env, name, TRUE));
			ft_strcat(dest, ft_getenv(&mini->env, name, TRUE));
			//len_temp = ft_strlen(ft_getenv(&mini->env, name, TRUE));
			j += ft_strlen(ft_getenv(&mini->env, name, TRUE));
			i += ft_strlen(name) + 1;
			//printf("%s\n", dest);
		}
		else if (name && !ft_getenv(&mini->env, name, TRUE))
		{
			i += ft_strlen(name) + 1;
		}
		else
		{
			dest[j++] = str[i++];
			//printf("dest = %s\n", dest);
		}
		ft_strdel(&name);
	}
	//printf("nouvelle chaine = %s\n", dest);
	return (dest);
}

/*
**	this function return True if in str[pos] it's a quote
**		- not echaped
**		- in a string with diff quote
*/

// int			quote_in_quotes(const char *str, int pos)
// {
// 	int		in_quote;

// 	if (pos >=0 && pos < ft_strlen(str))
// 	{
// 		if (is_echaped(str, str[pos], pos))
// 			return (TRUE);
// 		in_quote = valid_quotes(str, pos);
// 		if (in_quote == 0)
// 			return (FALSE);
// 		return (is_quote(str[pos]) != in_quote);
// 	}
// 	return (FALSE);
// }

// int			space_in_quotes(const char *str, int pos)
// {
// 	if (pos >=0 && pos < ft_strlen(str))
// 	{
// 		return (str[pos] == ' ' && is_in_quotes(str, pos));
// 	}
// 	return (FALSE);
// }

// static char		*ft_strndup_split_(const char *s, int begin, int end, t_ms *mini)
// {
// 	char		*str;
// 	char		*dest;
// 	size_t		i;
// 	int			j;
// 	int			quote;

// 	i = begin;
// 	j = 0;
// 	str = ft_strnew(end - begin);
// 	if (!str)
// 		return (NULL);
// 	while (s[i] && i < end)
// 	{
// 		if (is_quote(s[i]) && !quote_in_quotes(s, i))
// 			i++;
// 		else if (valid_quotes(s, i) == 39)	//in simple quote
// 		{
// 			// on garde tout
// 				str[j++] = s[i++];
// 		}
// 		else if (valid_quotes(s, i) == 34)	// in double quote
// 		{
// 			//on verifie \ $ 
// 			if ((is_echaped(s, '$', i + 1))
// 				|| (is_echaped(s, '\\', i + 1))
// 				|| (is_echaped(s, '\"', i + 1))) 
// 			{
// 				i++;	// on affiche pas \$
// 			}
// 			else
// 			{
// 					str[j++] = s[i++];
// 			} 
// 		}
// 		else
// 				str[j++] = s[i++];;
// 	}
// 	dest = substitute(str, mini);
// 	ft_strdel(&str);
// 	return (dest);
// }

// static void		free_split_(char **dest, int y)
// {
// 	while (y >= 0)
// 	{
// 		free(dest[y]);
// 		y--;
// 	}
// 	free(dest);
// }

// static char		**ft_fill_words_(char **dest, char const *s, t_ms *mini)
// {
// 	int			i;
// 	int			y;
// 	int			j;

// 	i = 0;
// 	y = 0;
// 	while (s[i])
// 	{
// 		while (s[i] && (s[i] == ' ' || (is_quote(s[i]) && !quote_in_quotes(s,i))))
// 			i++;
// 		j = i;
// 		while (s[i] && (s[i]!= ' ' || is_in_quotes(s, i)))
// 			i++;
// 		if (i > j)
// 		{
// 			dest[y++] = ft_strndup_split_(s, j, i, mini);
// 			if (!dest[y - 1])
// 			{
// 				free_split_(dest, y - 2);
// 				return (NULL);
// 			}
// 		}
// 	}
// 	dest[y] = NULL;
// 	return (dest);
// }

// static int		nb_word_(char const *s)
// {
// 	size_t		i;
// 	int			cmpt;

// 	i = 0;
// 	cmpt = 0;
// 	while (s[i])
// 	{
// 		while (s[i] && (s[i] == ' ' || (is_quote(s[i]) && !quote_in_quotes(s,i))))
// 			i++;			
// 		if (s[i])
// 			cmpt++;
// 		while (s[i] && (s[i]!= ' ' || is_in_quotes(s, i)))
// 			i++;
// 	}
// 	//printf("nb = %d\n", cmpt);
// 	return (cmpt);
// }

// char		**parse(const char *str, t_ms *mini)
// {
// 	int		i;
// 	char	**dest;
// 	int		nb_word;

// 	i = 0;
// 	if (!str)
// 		return (NULL);
// 	if (valid_quotes(str, ft_strlen(str)) > 0)
// 	{
// 		printf("minishell: syntax error with open quotes\n");
// 		return (NULL);
// 	}
// 	nb_word = nb_word_(str);
// 	dest = (char **)malloc(sizeof(char *) * (nb_word + 1));
// 	if (!dest)
// 		return(NULL);
// 	return (ft_fill_words_(dest, str, mini));
// }