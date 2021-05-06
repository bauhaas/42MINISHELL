/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clorin <clorin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/17 16:05:15 by clorin            #+#    #+#             */
/*   Updated: 2021/05/06 04:54:12 by bahaas           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static char	**expansion(char *str, char **word, t_ms *ms, int *i)
{
	int				j;
	int				k;
	char			**split_word;

	j = *i;
	k = 0;
	split_word = NULL;
	*word = ft_add_str(*word, value(ms, str + *i, i));
	if (!(!ft_strchr(*word, ' ') || valid_quotes(str, j)
				|| (j > 0 && str[j - 1] == '=')) && ms->echo == FALSE)
	{
		split_word = ft_split(*word, ' ');
		return (split_word);
	}
	else if (*word[0] == '\0')
	{
		split_word = malloc(sizeof(char *) * 2);
		split_word[0] = ft_strnew(1);
		split_word[1] = NULL;
		ms->free_all = 1;
		return (split_word);
	}
	return (NULL);
}

static int	parse2(t_ms *ms, char *str, int i, char **word)
{
	if (str[i] == '\\')
		i = back_slash(ms, str, word, i);
	else if (str[i] == ' ' || str[i] == '\t' || str[i] == '>' ||
			str[i] == '|' || str[i] == '<')
		i = special(ms, str, word, i);
	else if (str[i] == '"')
		i = quote(str, word, i, QUOTE);
	else if (str[i] == '\'')
		i = quote(str, word, i, DQUOTE);
	return (i);
}

static int	init_is_env(t_ms *ms)
{
	ms->is_env = 0;
	return (TRUE);
}

void		env_token(t_ms *ms, char **word, char ***split_word, char ***head)
{
	*head = *split_word;
	while (**split_word)
		create_token(ms, (*split_word)++);
	ft_strdel(word);
}

void		parse(char *str, t_ms *ms, int i)
{
	char			**split_word;
	char			**split_head;
	char			*word;

	word = NULL;
	while (str[i] && init_is_env(ms))
	{
		ms->free_all = 0;
		if (!is_spec_car(str[i]))
			word = ft_add_char(word, str[i++]);
		else if (is_spec(str[i]))
			i = parse2(ms, str, i, &word);
		else if (str[i] == '$' && valid_quotes(str, i) != QUOTE)
		{
			if ((split_word = expansion(str, &word, ms, &i)))
				env_token(ms, &word, &split_word, &split_head);
			if (ms->free_all)
				free_split(&split_head);
		}
		else
			word = ft_add_char(word, str[i++]);
	}
	create_token(ms, &word);
}
