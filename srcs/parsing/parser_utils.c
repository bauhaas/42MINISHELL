/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clorin <clorin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/19 14:13:16 by clorin            #+#    #+#             */
/*   Updated: 2021/04/19 14:14:03 by clorin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int			back_slash(char *str, char **word, int i)
{
	if (valid_quotes(str, i) == 0)
	{
		i++;
		*word = ft_add_char(*word, str[i]);
		if (str[i])
			i++;
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

void		new_token(t_tokens **tokens, char **word)
{
	t_tokens		*new;

	if (*word)
	{
		new = create_token(tokens);
		if (new == NULL)
			return ;
		new->content = ft_strdup(*word);
		new->type_content = set_token_type(*word, NULL);
		ft_strdel(word);
	}
}

int			dbl_quote(char *str, char **word, int i)
{
	if (valid_quotes(str, i) == QUOTE)
		*word = ft_add_char(*word, str[i]);
	else
		*word = ft_add_char(*word, '\0');
	i++;
	return (i);
}

int			simple_quote(char *str, char **word, int i)
{
	if (valid_quotes(str, i) == DQUOTE)
		*word = ft_add_char(*word, str[i]);
	else
		*word = ft_add_char(*word, '\0');
	i++;
	return (i);
}

int			special(char *str, char **word, int i, t_tokens **tokens)
{
	if (valid_quotes(str, i) == 0)
	{
		new_token(tokens, word);
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
		new_token(tokens, word);
	}
	else
		*word = ft_add_char(*word, str[i]);
	i++;
	return (i);
}
