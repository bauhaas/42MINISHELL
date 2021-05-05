/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bahaas <bahaas@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/01 01:25:44 by bahaas            #+#    #+#             */
/*   Updated: 2021/05/05 14:40:14 by bahaas           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	print_tokens(t_tokens *tokens)
{
	t_tokens *tmp;

	if(DEBUG >= 3)
	{
		tmp = tokens;
		printf("type_content : 1 = CMD_ARGS, 3 = REDIR, 4 = PIPES, 5 = END_CMD\n");
		printf("\nTOKENS\n");
		while (tmp)
		{
			printf("|%s| |type_content : %d| |is_env : %d|\n", tmp->content, tmp->type_content, tmp->is_env);
			tmp = tmp->next;
		}
	}
}

void	print_cmd(t_cmd *cmd)
{
	if(DEBUG)
	{
		t_cmd	*tmp;
		int		i;
		int		cmd_id;

		cmd_id = 0;
		tmp = cmd;
		printf("\ntype_pipe : 1 = CMD_ARGS, 4 = PIPES, 5 = END_CMD, 6/7/8 = L/R/DR\n");
		printf("\nCOMMAND\n");
		while (tmp)
		{
			i = 0;
			char *type_link;
			printf("cmd(%d)->address : %p\n", cmd_id, &tmp);
			printf("cmd(%d)->address next : %p\n", cmd_id, &tmp->next);
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
				if(tmp->prev)
				printf("cmd(%d)->content->prev[%d] : %s\n", cmd_id, i, tmp->prev->content[i]);
				if(tmp->next)
				printf("cmd(%d)->content->next[%d] : %s\n", cmd_id, i, tmp->next->content[i]);
				i++;
			}
			if(cmd_id == 4)
				exit(0);
			printf("cmd(%d)->type_link : %s(%d)\n", cmd_id, type_link, tmp->type_link);
			printf("cmd(%d)->is_env : %d\n", cmd_id, tmp->is_env);
			printf("\n");
			cmd_id++;
			tmp = tmp->next;
		}
		printf("\n");
	}
}
