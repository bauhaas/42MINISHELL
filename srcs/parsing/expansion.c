/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clorin <clorin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/24 13:13:22 by clorin            #+#    #+#             */
/*   Updated: 2021/04/12 15:55:15 by bahaas           ###   ########.fr       */
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
int		valid_quotes(const char *s, int len)
{
	int	i;
	int	open;
	char *str;

	i = 0;
	open = 0;
	str = ft_strdup(s);
	while (str[i] && i <= len)
	{
		if (i > 0 && str[i - 1] == '\\' && str[i] != '\'')
		{
			if(str[i] == '\\')
				str[i] = '_';
		}
		else if (open == 0 && str[i] == '\"')
			open = 34;
		else if (open == 0 && str[i] == '\'' && str[i - 1] != '\\')
			open = 39;
		else if (open == 34 && str[i] == '\"')
			open = 0;
		else if (open == 39 && str[i] == '\'')
			open = 0;
		i++;
	}
	ft_strdel(&str);
	return (open);
}

int				valid_name(char *name)
{
	int			i;

	i = 0;
	if (!name || !*name)
		return (FALSE);
	if (name[i] == '$')
		i++;
	if (!ft_isalpha(name[i]) && name[i] != '_')
		return (FALSE);
	i++;
	while (name[i])
	{
		if (!ft_isalnum(name[i]) && name[i] != '_')
			return (FALSE);
		i++;
	}
	return (TRUE);
}

/*
**	this function returns the name's var after $ or NULL if nothing
*/
static char		*get_name_var(char *str)
{
	int		i;
	int		j;
	char	*dest;

	i = 0;
	if (!str || !*str)
		return (NULL);
	if (str[i++] == '$')
	{
		j = i;
		if (ft_isalpha(str[i]) || str[i] == '_')
		{
			i++;
			while (str[i] && (ft_isalnum(str[i]) || str[i] == '_'))
				i++;
			dest = ft_strndup(str + j, i - j);
			return (dest);
		}
	}
	return (NULL);
}

/*
**	this function retrieves the value of the environment variable and returns its length
*/
static int			len_substitut(char *str, t_ms *mini)
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
			len_total += ft_intlen(mini->last_ret);
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

// char		*substitute2(char *str, t_ms *mini)
// {
// 	int		i;
// 	int		len_total_subs;
// 	int		j;
// 	char	*dest;
// 	char	*name;
	
// 	printf("-- substitute string <%s> --\n", str);
// 	//printf("longueur au depart = %d\n", (int)ft_strlen(str));
// 	i = 0;
// 	// premier passe pour la longueur total
// 	//printf("first pass >>>\t");
// 	len_total_subs = len_substitut(str, mini);
// 	if (len_total_subs == 0)
// 		return (NULL);
// 	printf("Longueur substitute = %d\n", (int)len_total_subs);
// 	dest = ft_strnew(len_total_subs);
// 	// deuxieme pass pour subsitute var
// 	//printf("Second pass >>>\n");
// 	i = 0;
// 	j = 0;
// 	while (str[i])
// 	{
// 		name = get_name_var(str + i);
// 		printf("name trouve = %s\n", name);
// 		if (str[i] == '$' && str[i + 1] == '?')
// 		{
// 			ft_strcat(dest, "TODO_$?");
// 			j += ft_strlen("TODO_$?");
// 			i += 2;
// 		}
// 		else if (str[i] != '$' || (str[i] == '$' && (str[i + 1] == '$' || !str[i + 1])))
// 			dest[j++] = str[i++];
// 		else if (name && ft_getenv(&mini->env, name, TRUE))
// 		{
// 			//printf("ft_strcat(%s,%s)=>", dest, ft_getenv(&mini->env, name, TRUE));
// 			ft_strcat(dest, ft_getenv(&mini->env, name, TRUE));
// 			//len_temp = ft_strlen(ft_getenv(&mini->env, name, TRUE));
// 			j += ft_strlen(ft_getenv(&mini->env, name, TRUE));
// 			i += ft_strlen(name) + 1;
// 			//printf("%s\n", dest);
// 		}
// 		else if (name && !ft_getenv(&mini->env, name, TRUE))
// 		{
// 			i += ft_strlen(name) + 1;
// 		}
// 		else
// 			printf("else\n");
// 		ft_strdel(&name);
// 	}
// 	//printf("nouvelle chaine = %s\n", dest);
// 	return (dest);
// }

/*
**	concate the value of *name in *dest
**	and returns the length of *name
*/

static int			cat_value(t_ms *mini, char *str, char *dest)
{
	char			*name;
	int				len;

	name = get_name_var(str);
	len = ft_strlen(name);
	if (*str != '$' || (*str == '$' && (*(str + 1) == '$' || !*(str + 1))))
	{
		ft_strncat(dest, str, 1);
		ft_strdel(&name);
		return (1);
	}
	if (*str == '$' && *(str + 1) == '?')
	{
		ft_strcat(dest, ft_itoa(mini->last_ret));
		ft_strdel(&name);
		return (2);
	}
	if (name && ft_getenv(&mini->env, name, TRUE))
		ft_strcat(dest, ft_getenv(&mini->env, name, TRUE));
	ft_strdel(&name);
	return (len + 1);
}

char		*substitute(char *str, t_ms *mini)
{
	int		i;
	int		len_total_subs;
	char	*dest;
	
	i = 0;
	len_total_subs = len_substitut(str, mini);
	//printf("test len_total_subs : %d\n", len_total_subs);
	if (len_total_subs == 0)
		return (NULL);
	dest = ft_strnew(len_total_subs);
	if (!dest)
		return (NULL);
	while (str[i])
		i += cat_value(mini, str + i, dest);
	//printf("test dest : %s\n", dest);
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

static char			*value(t_ms *mini, char *str, int *i)
{
	char			*name;
	int				len;
	char			*dest;

	dest = NULL;
	name = get_name_var(str);
	// printf("name = %s\n", name);
	len = ft_strlen(name);
	if (*str == '$' && *(str + 1) == '?')
	{
		dest = ft_strnew(ft_strlen(ft_itoa(mini->last_ret)));
		ft_strcat(dest, ft_itoa(mini->last_ret));
		ft_strdel(&name);
		*i += 2;
		return (dest);
	}
	else if (name && ft_getenv(&mini->env, name, TRUE))
	{
		dest = ft_strnew(ft_strlen(ft_getenv(&mini->env, name, TRUE)));
		ft_strcat(dest, ft_getenv(&mini->env, name, TRUE));
		ft_strdel(&name);
		*i += len + 1;
		return (dest);
	}
	else if (name && !ft_getenv(&mini->env, name, TRUE))
	{
		dest = ft_strnew(1);
		*i += len + 1;
		return (dest);
	}
	else if (*str != '$' || (*str == '$' && (*(str + 1) == '$' || !*(str + 1))))
	{
		dest = ft_strnew(1);
		ft_strncat(dest, str, 1);
		ft_strdel(&name);
		*i += 1;
		return (dest);
	}
	else
	{
		dest = ft_strnew(1);
		dest[0] = '$';
		ft_strdel(&name);
		*i += 1;
		return (dest);
	}	

}

static char	*ft_add_str(char *s1, const char *s2)
{
	char *dest;
	int	i;
	int j;

	i = 0;
	j = 0;
	dest = ft_strnew(ft_strlen(s1) + ft_strlen(s2));
	while (s1 && s1[i])
		dest[j++] = s1[i++];
	i = 0;
	while (s2[i])
		dest[j++] = s2[i++];
	ft_strdel(&s1);
	return (dest);
}
static char	*ft_add_char(char *str, char c)
{
	int		len;
	char	*new;

	len = ft_strlen(str);
	new = ft_strnew(len + 1);
	ft_memcpy(new, str, len);
	new[len] = c;
	ft_strdel(&str);
	return (new);
}

static int	is_spec_car(char c)
{
	return (c == '\\' || c == '$' || c == '"' || c == '\'' || c == ' ' || c== '\t'
		|| c == ';' || c == '|' || c == '>' || c == '<');
}

void	parse(char *str, t_ms *mini, t_tokens **tokens)
{
	int		i;
	char	*word;
	t_list	*list_word;
	t_tokens *new;

	if (!str)
		return ;
	if (valid_quotes(str, ft_strlen(str)) > 0)
	{
		printf("minishell: syntax error with open quotes\n");
		return ;
	}
	i = 0;

	word = NULL;
	list_word = NULL;
	//printf("debut\n");
	while (str[i])
	{
		if(!is_spec_car(str[i]))
		{
			word = ft_add_char(word, str[i]);
			i++;
		}
		else
		{
			if (str[i] == '\\')
			{
				if (valid_quotes(str, i) == 0)
				{
					i++;
					word = ft_add_char(word, str[i]);
					i++;
				}
				else if (valid_quotes(str, i) == DQUOTE)
				{
					i++;
					if(str[i] != '$' && str[i] != '\\' && str[i] != '"')
						word = ft_add_char(word, '\\');
					word = ft_add_char(word, str[i]);
					i++;
				}
				else
				{
					word = ft_add_char(word, str[i]);
					i++;
				}
			}
			else if (str[i] == ' ' || str[i] == '\t' || str[i] == ';'
				|| str[i] == '>' || str[i] =='|' || str[i] == '<')
			{
				if (valid_quotes(str, i ) == 0)
				{
					if (word)
					{
						new = create_token(tokens);
						if(new == NULL)
							return ;
						new->content = ft_strdup(word);
						new->type_content = CMD;
						//ft_lstadd_back(&list_word,ft_lstnew(ft_strdup(word)));
						//printf("word = %s\n", word);
					}
					ft_strdel(&word);
					if (str[i] == '>')
					{
						if (str[i + 1] == '>')
						{
							word = ft_strdup(">>");
							i++;
						}
						else
							word = ft_strdup(">");
					}
					else if (str[i] != '\t' && str[i] != ' ')
					{
						word = ft_add_char(word, str[i]);
					}
					if (word)
					{
						new = create_token(tokens);
						//if(new == NULL)
							//return;
						new->content = ft_strdup(word);
						new->type_content = set_token_type_aaa(word, new);
						//ft_lstadd_back(&list_word,ft_lstnew(ft_strdup(word)));
					}
					//printf("sep = '%s'\n", word);
					ft_strdel(&word);
				}
				else
					word = ft_add_char(word, str[i]);
				i++;
				
			}
			else if (str[i] == '"')
			{
				if (valid_quotes(str, i) == QUOTE)
				{
					word = ft_add_char(word, str[i]);
					i++;
				}
				else
				{
					word = ft_add_char(word, '\0');
					i++;
				}	
			}
			else if (str[i] == '\'')
			{
				if (valid_quotes(str, i) == DQUOTE)
				{
					word = ft_add_char(word, str[i]);
					i++;
				}
				else
				{
					word = ft_add_char(word, '\0');
					i++;
				}
			}
			else if (str[i] == '$' && valid_quotes(str, i) != QUOTE)
				word = ft_add_str(word, value(mini, str + i, &i));
			else
			{
				word = ft_add_char(word, str[i]);
				i++;
			}
		}
	}
	if(word)
	{
		new = create_token(tokens);
		if(new == NULL)
			return ;
		new->content = ft_strdup(word);
		new->type_content = CMD;
		//ft_lstadd_back(&list_word,ft_lstnew(ft_strdup(word)));
		//printf("word = %s\n", word);
	}

	ft_strdel(&word);
	printf("fin\n");
}
