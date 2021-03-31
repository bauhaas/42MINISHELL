/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bahaas <bahaas@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/01 01:25:44 by bahaas            #+#    #+#             */
/*   Updated: 2021/04/01 01:40:15 by bahaas           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"


void	print_tokens(t_tokens *tokens)
{
	t_tokens *tmp;

	tmp = tokens;
	printf("type_content : 0 = CMD, 1 = ARGS, 2 = PIPES, 3 = REDIR, 4 = END_CMD \n");
	printf("type_quotes : 0 = NO_QUOTES, 34 = DQUOTE, QUOTE = 39\n");
	printf("\nTOKENS\n");
	while(tmp)
	{
		printf("|%s|  |type_quote : %d| |type_content : %d|\n", tmp->content, tmp->type_quote, tmp->type_content);
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
	printf("\nCOMMAND\n");
	while(tmp)
	{
		i = 0;
		while(tmp->content[i])
		{
			printf("cmd(%d)->content[%d] : %s\n", cmd_id, i, tmp->content[i]);
			i++;
		}
		cmd_id++;
		tmp = tmp->next;
	}
}

