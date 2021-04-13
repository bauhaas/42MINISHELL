/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bahaas <bahaas@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/01 01:25:44 by bahaas            #+#    #+#             */
/*   Updated: 2021/04/13 13:54:28 by bahaas           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"


void	print_tokens(t_tokens *tokens)
{
	t_tokens *tmp;

	tmp = tokens;
	printf("type_content : 1 = CMD_ARGS, 3 = REDIR, 4 = PIPES, 5 = END_CMD\n");
	printf("\nTOKENS\n");
	while (tmp)
	{
		printf("|%s| |type_content : %d|\n", tmp->content, tmp->type_content);
		tmp = tmp->next;
	}
}

void	print_cmd(t_cmd *cmd)
{
	t_cmd *tmp;
	int i;
	int cmd_id;

	cmd_id = 0;
	tmp = cmd;
	printf("\ntype_pipe : 1 = CMD_ARGS, 4 = PIPES, 5 = END_CMD, 6/7/8 = L/R/DR\n");
	printf("\nCOMMAND\n");
	while (tmp)
	{
		i = 0;
		char *type_link;
		while (tmp->content[i])
		{
			if (tmp->type_link == 0)
				type_link = "0";
			else if (tmp->type_link == 1)
				type_link = "CMD_ARGS";
			else if (tmp->type_link == 4)
				type_link = "PIPES";
			else if (tmp->type_link == 5)
				type_link = "END_CMD";
			else if (tmp->type_link == 6)
				type_link = "L";
			else if (tmp->type_link == 7)
				type_link = "R";
			else if (tmp->type_link == 8)
				type_link = "DR";
			printf("cmd(%d)->content[%d] : %s\n", cmd_id, i, tmp->content[i]);
			i++;
		}
		printf("cmd(%d)->type_link : %s(%d)\n", cmd_id, type_link, tmp->type_link);
		printf("\n");
		cmd_id++;
		tmp = tmp->next;
	}
	printf("\n");
}

void	print_action(t_cmd *cmd)
{
	int j = 0;
	printf("cmd:");
	while (cmd->content[j])
	{
		printf(" %s", cmd->content[j]);
		j++;
	}
	if (cmd->prev)
	{
		printf("\nprev cmd type_link : %d\n", cmd->prev->type_link);
		printf("prev cmd->content[0]: %s\n", cmd->prev->content[0]);
	}
	if (cmd->next)
	{
		printf("\nnext cmd type_link : %d\n", cmd->next->type_link);
		printf("next cmd->content[0]: %s\n", cmd->next->content[0]);
	}
}

void	print_action_exec_condition(t_cmd *cmd, int pipe, t_ms *ms)
{
	printf("test cmd : %s\n", cmd->content[0]);
	if (is_type(cmd->prev, END_CMD))
		printf("test prev END\n");
	if (is_type(cmd->prev, PIPES))
		printf("test prev PIPE\n");
	if (!cmd->prev)
		printf("test prev\n");
	if (pipe)
		printf("test pipe\n");
}
