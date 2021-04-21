/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clorin <clorin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/17 16:05:15 by clorin            #+#    #+#             */
/*   Updated: 2021/04/17 16:05:40 by clorin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static char			**expansion(char *str, char **word, t_ms *mini, int *i)
{
	int				j;
	int				k;
	char			**split_word;

	j = *i;
	k = 0;
	split_word = NULL;
	*word = ft_add_str(*word, value(mini, str + *i, i));
	if (!(!ft_strchr(*word, ' ') || valid_quotes(str, j)
		|| (j > 0 && str[j - 1] == '=')))
	{
		split_word = ft_split(*word, ' ');
		return (split_word);
	}
	return (NULL);
}

static int			parse2(char *str, int i, char **word, t_tokens **tokkens)
{
	if (str[i] == '\\')
		i = back_slash(str, word, i);
	else if (str[i] == ' ' || str[i] == '\t' || str[i] == ';'
	|| str[i] == '>' || str[i] == '|' || str[i] == '<')
		i = special(str, word, i, tokkens);
	else if (str[i] == '"')
		i = quote(str, word, i, QUOTE);
	else if (str[i] == '\'')
		i = quote(str, word, i, DQUOTE);
	return (i);
}

static	int			is_spec(char c)
{
	return (c == '\\' || c == ' ' || c == '\t' ||
			c == ';' || c == '>' || c == '|' || c == '<'
			|| c == '"' || c == '\'');
}

void				parse(char *str, t_ms *mini, t_tokens **tokens)
{
	char			**split_word;
	char			*word;
	int				i;

	i = 0;
	word = NULL;
	while (str[i])
	{
		if (!is_spec_car(str[i]))
			word = ft_add_char(word, str[i++]);
		else if (is_spec(str[i]))
			i = parse2(str, i, &word, tokens);
		else if (str[i] == '$' && valid_quotes(str, i) != QUOTE)
		{
			if ((split_word = expansion(str, &word, mini, &i)))
			{
				while (*split_word)
					new_token(tokens, (split_word)++);
				ft_strdel(&word);
			}
		}
		else
			word = ft_add_char(word, str[i++]);
	}
	new_token(tokens, &word);
}
