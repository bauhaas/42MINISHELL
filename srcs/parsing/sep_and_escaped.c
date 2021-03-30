/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sep_and_escaped.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bahaas <bahaas@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/29 17:32:41 by bahaas            #+#    #+#             */
/*   Updated: 2021/03/30 01:47:17 by bahaas           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/*
** Parse all the sep set and if we found that a sep is part of our line, we
** set a new token equals to the sep. Then inc our index to the sep len.
*/

int	is_sep_tok(t_ms *ms, t_tokens **tokens, char *line, size_t *i)
{
	size_t				j;
	t_tokens			*new;

	j = 0;
	while (ms->sep_set[j] != NULL)
	{
		if (ft_strnequ(ms->sep_set[j], &line[*i], ft_strlen(ms->sep_set[j])))
		{
			new = create_token(tokens);
			if (new == NULL)
				return (1);
			*i += ft_strlen(ms->sep_set[j]);
			new->content = ft_strdup(ms->sep_set[j]);
		}
		j++;
	}
	return (0);
}

/*
** WIP
*/

int	is_escaped_tok(t_ms *ms, t_tokens **tokens, char *line, size_t *i)
{
	t_tokens	*new;

	if (line[*i] == BSLASH && ft_strlen(&line[*i]) >= 2)
	{
		if (line[*i + 1] == '\n')
		{
			*i += 2;
			return (0);
		}
		new = create_token(tokens);
		if (new == NULL)
			return (1);
		new->content = ft_strnew(1);
		if (new->content == NULL)
			return (1);
		new->content[0] = line[*i + 1];
		*i += 2;
	}
	else if (line[*i] == BSLASH)
		*i += 1;
	return (0);
}
