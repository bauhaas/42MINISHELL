/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fill_command.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bahaas <bahaas@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/03 13:56:58 by bahaas            #+#    #+#             */
/*   Updated: 2021/05/03 14:01:26 by bahaas           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void		set_redirection_type(t_cmd *cmd, t_tokens **tokens)
{
	if ((*tokens)->type_content == PIPES)
		cmd->type_link = 4;
	else if ((*tokens)->type_content == REDIR)
	{
		cmd->type_link = 6;
		if (!strcmp((*tokens)->content, ">"))
			cmd->type_link = 7;
		else if (!strcmp((*tokens)->content, ">>"))
			cmd->type_link = 8;
	}
}

void		fill_sep_cmd(t_ms *ms, t_cmd **new, t_cmd **cmd, t_tokens **tokens)
{
	int i;

	i = 0;
	set_redirection_type(*new, tokens);
	(*new)->content[i] = ft_strdup((*tokens)->content);
	i++;
	*tokens = (*tokens)->next;
	(*new)->content[i] = NULL;
}

void		fill_arg_cmd(t_ms *ms, t_cmd **new, t_cmd **cmd, t_tokens **tokens)
{
	int i;

	i = 0;
	while ((*tokens))
	{
		if ((*tokens)->type_content == CMD_ARGS)
		{
			if ((*tokens)->is_env)
				(*new)->is_env = 1;
			(*new)->type_link = CMD_ARGS;
			(*new)->content[i] = ft_strdup((*tokens)->content);
			i++;
			*tokens = (*tokens)->next;
			if (((*tokens) &&
					(*tokens)->prev && (*tokens)->prev->type_content == REDIR))
				break ;
		}
		else
			break ;
	}
	(*new)->content[i] = NULL;
}
