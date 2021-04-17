/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clorin <clorin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/24 13:13:22 by clorin            #+#    #+#             */
/*   Updated: 2021/04/12 17:16:03 by bahaas           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

/*
** This function checks the the string str
** if open return the ascii code of the quote
** otherwise 0 if closed
*/

int		valid_quotes(const char *s, int len)
{
	char	*str;
	int		open;
	int		i;

	str = ft_strdup(s);
	open = 0;
	i = -1;
	while (str[++i] && i <= len)
	{
		if (i > 0 && str[i - 1] == '\\' && str[i] != '\'')
		{
			if (str[i] == '\\')
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
	}
	ft_strdel(&str);
	return (open);
}

int		valid_name(char *name)
{
	int	i;

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
	char	*dest;
	int		i;
	int		j;

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
** It retrieves the value of the environment variable and returns its length
*/

static int			len_substitut(char *str, t_ms *mini)
{
	int		len_total;
	char	*name;
	int		i;

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

/*
**	Concate the value of *name in *dest
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
	int		len_total_subs;
	char	*dest;
	int		i;

	i = 0;
	len_total_subs = len_substitut(str, mini);
	if (len_total_subs == 0)
		return (NULL);
	dest = ft_strnew(len_total_subs);
	if (!dest)
		return (NULL);
	while (str[i])
		i += cat_value(mini, str + i, dest);
	return (dest);
}

static char			*value(t_ms *mini, char *str, int *i)
{
	char			*dest;
	char			*name;
	int				len;

	dest = NULL;
	name = get_name_var(str);
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

static int	is_spec_car(char c)
{
	return (c == '\\' || c == '$' || c == '"' || c == '\'' || c == ' ' ||
			c == '\t' || c == ';' || c == '|' || c == '>' || c == '<');
}

void	parse(char *str, t_ms *mini, t_tokens **tokens)
{
	t_tokens	*new;
	char		*word;
	int			i;

	if (!str)
		return ;
	if (valid_quotes(str, ft_strlen(str)) > 0)
	{
		printf("minishell: syntax error with open quotes\n");
		return ;
	}
	i = 0;
	word = NULL;
	while (str[i])
	{
		if (!is_spec_car(str[i]))
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
					if (str[i])
						i++;
				}
				else if (valid_quotes(str, i) == DQUOTE)
				{
					i++;
					if (str[i] != '$' && str[i] != '\\' && str[i] != '"')
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
				|| str[i] == '>' || str[i] == '|' || str[i] == '<')
			{
				if (valid_quotes(str, i) == 0)
				{
					if (word)
					{
						new = create_token(tokens);
						if (new == NULL)
							return ;
						new->content = ft_strdup(word);
						new->type_content = CMD_ARGS;
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
						word = ft_add_char(word, str[i]);
					if (word)
					{
						new = create_token(tokens);
						if (new == NULL)
							return ;
						new->content = ft_strdup(word);
						new->type_content = set_token_type(word, new);
					}
					ft_strdel(&word);
				}
				else
					word = ft_add_char(word, str[i]);
				i++;
			}
			else if (str[i] == '"')
			{
				if (valid_quotes(str, i) == QUOTE)
					word = ft_add_char(word, str[i]);
				else
					word = ft_add_char(word, '\0');
				i++;
			}
			else if (str[i] == '\'')
			{
				if (valid_quotes(str, i) == DQUOTE)
					word = ft_add_char(word, str[i]);
				else
					word = ft_add_char(word, '\0');
				i++;
			}
			else if (str[i] == '$' && valid_quotes(str, i) != QUOTE)
			{
				int j;
				j = i;
				word = ft_add_str(word, value(mini, str + i, &i));
				if (!(!ft_strchr(word, ' ') || valid_quotes(str, j) || (j > 0 && str[j - 1] == '=')))
				{
					// printf("espace dans word = `%s` et str[%d] = %c\nOn split : ", word, j-1, str[j - 1]);
					char **split_word;
					split_word = ft_split(word, ' ');
					int k = 0;
					while(split_word[k])
					{
						new = create_token(tokens);
						if (new == NULL)
							return ;
						new->content = ft_strdup(split_word[k]);
						new->type_content = CMD_ARGS;
						k++;
					}
					free_split(&split_word);
					ft_strdel(&word);
				}
			}
			else
			{
				word = ft_add_char(word, str[i]);
				i++;
			}
		}
	}
	if (word)
	{
		new = create_token(tokens);
		if (new == NULL)
			return ;
		new->content = ft_strdup(word);
		new->type_content = CMD_ARGS;
	}
	ft_strdel(&word);
}
