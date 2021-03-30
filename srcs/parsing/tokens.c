/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokens.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bahaas <bahaas@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/20 14:29:47 by bahaas            #+#    #+#             */
/*   Updated: 2021/03/30 14:54:17 by bahaas           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/*
** Parse our list of tokens. Set a new one and add it at the end of our list
*/

t_tokens	*create_token(t_tokens **tokens)
{
	t_tokens	*tmp;
	t_tokens	*new;

	new = ft_memalloc(sizeof(t_tokens));
	tmp = *tokens;
	while (tmp != NULL && tmp->next != NULL)
		tmp = tmp->next;
	if (tmp == NULL)
		*tokens = new;
	else
		tmp->next = new;
	return (new);
}

void	init_sep(t_ms *ms)
{
	ms->sep_set[0] = ft_strdup(";");
	ms->sep_set[1] = ft_strdup(">");
	ms->sep_set[2] = ft_strdup(">>");
	ms->sep_set[3] = ft_strdup("<");
	ms->sep_set[4] = ft_strdup("|");
	ms->sep_set[5] = NULL;
}

/*
** We trim all the space at the beginning of the line
** Then, we'll iterate on each type of token.
*/

int	get_tokens(t_ms *ms, t_tokens **tokens, char *line)
{
	size_t	i;

	i = 0;
	init_sep(ms);
	while (line[i])
	{
		while (is_space(line[i]))
			i++;
		//printf("test line before str_tok : %s\n", &line[i]);
		if (is_str_tok(ms, tokens, line, &i))
			return (1);
		//printf("test line beofre sep_tok: %s\n", &line[i]);
		if (is_sep_tok(ms, tokens, line, &i))
			return (1);
		//printf("test line before quote_tok: %s\n", &line[i]);
		if (is_quote_tok(ms, tokens, line, &i))
			return (1);
		//printf("test line before dquote_tok: %s\n", &line[i]);
		if (is_dquote_tok(ms, tokens, line, &i))
			return (1);
		//printf("test line before escaped_tok: %s\n", &line[i]);
		if (is_escaped_tok(ms, tokens, line, &i))
			return (1);
	}
	return (0);
}
