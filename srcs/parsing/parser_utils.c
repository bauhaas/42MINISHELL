/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clorin <clorin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/19 14:13:16 by clorin            #+#    #+#             */
/*   Updated: 2021/04/27 09:14:46 by clorin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int			escaped(char *str, int pos)
{
	if (pos > 0 && str[pos - 1] == '\\')
		return (TRUE);
	return (FALSE);
}

void		new_token(t_ms *ms, t_tokens **tokens, char **word)
{
	t_tokens		*new;

	if (*word)
	{
		new = create_token(tokens);
		if (new == NULL)
			return ;
		new->content = ft_strdup(*word);
		if (ms->escaped_tokens != 1)
			new->type_content = set_token_type(*word);
		else
			new->type_content = 1;
		ms->escaped_tokens = 0;
		if (ms->is_env)
			new->is_env = 1;
		if (ft_strcmp(*word, "echo") == 0)
			ms->echo = TRUE;
		ft_strdel(word);
	}
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
		new_token(ms, &ms->tokens, word);
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
		new_token(ms, &ms->tokens, word);
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
