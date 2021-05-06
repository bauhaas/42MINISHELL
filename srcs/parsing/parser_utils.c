/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clorin <clorin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/19 14:13:16 by clorin            #+#    #+#             */
/*   Updated: 2021/05/06 03:39:17 by bahaas           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int			is_spec(char c)
{
	return (c == '\\' || c == ' ' || c == '\t' || c == '>' || c == '|'
			|| c == '<' || c == '"' || c == '\'');
}

int			escaped(char *str, int pos)
{
	if (pos > 0 && str[pos - 1] == '\\')
		return (TRUE);
	return (FALSE);
}

int			back_slash(t_ms *ms, char *str, char **word, int i)
{
	if (valid_quotes(str, i) == 0)
	{
		i++;
		*word = ft_add_char(*word, str[i]);
		if (str[i])
			i++;
		ms->escaped_tokens = 1;
	}
	else if (valid_quotes(str, i) == DQUOTE)
	{
		i++;
		if (str[i] != '$' && str[i] != '\\' && str[i] != '"')
			*word = ft_add_char(*word, '\\');
		*word = ft_add_char(*word, str[i++]);
	}
	else
		*word = ft_add_char(*word, str[i++]);
	return (i);
}

int			special(t_ms *ms, char *str, char **word, int i)
{
	if (valid_quotes(str, i) == 0)
	{
		create_token(ms, word);
		if (str[i] == '>')
		{
			if (str[i + 1] == '>')
			{
				*word = ft_strdup(">>");
				i++;
			}
			else
				*word = ft_strdup(">");
		}
		else if (str[i] != '\t' && str[i] != ' ')
			*word = ft_add_char(*word, str[i]);
		create_token(ms, word);
	}
	else
		*word = ft_add_char(*word, str[i]);
	i++;
	return (i);
}

int			quote(char *str, char **word, int i, int q)
{
	if (valid_quotes(str, i) == q)
		*word = ft_add_char(*word, str[i]);
	else
		*word = ft_add_char(*word, '\0');
	i++;
	return (i);
}
