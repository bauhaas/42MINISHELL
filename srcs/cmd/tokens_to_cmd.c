/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokens_to_cmd.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bahaas <bahaas@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/03 12:13:07 by bahaas            #+#    #+#             */
/*   Updated: 2021/05/06 03:05:44 by bahaas           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/*
** Find the number of strings to malloc in our cmd->content via token type.
*/

static int	token_number_in_cmd(t_tokens **tokens)
{
	t_tokens	*count;
	int			i;

	count = *tokens;
	i = 0;
	while (count)
	{
		if (count->type_content == CMD_ARGS)
		{
			count = count->next;
			i++;
		}
		else
		{
			i++;
			break ;
		}
	}
	return (i);
}

static void	create_cmd(t_ms *ms, t_tokens **tokens)
{
	t_cmd		*new;
	t_cmd		*tmp;
	int			i;

	tmp = ms->cmd;
	new = ft_memalloc(sizeof(t_cmd));
	i = token_number_in_cmd(tokens);
	new->content = malloc(sizeof(char *) * (i + 1));
	if ((*tokens)->type_content != CMD_ARGS)
		fill_sep_cmd(&new, tokens);
	else
		fill_arg_cmd(&new, tokens);
	if (ms->cmd == NULL)
		ms->cmd = new;
	else
	{
		while (tmp != NULL && tmp->next != NULL)
			tmp = tmp->next;
		tmp->next = new;
		new->prev = tmp;
	}
}

void		tokens_to_cmd(t_ms *ms, t_tokens **tokens)
{
	if ((*tokens)->type_content == PIPES && ((*tokens)->prev
				&& ((*tokens)->prev->type_content == PIPES)))
	{
		*tokens = (*tokens)->next;
		ms->total_consecutive_pipes++;
		return ;
	}
	else
		create_cmd(ms, tokens);
}
