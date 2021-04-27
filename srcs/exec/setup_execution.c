/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   setup_execution.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bahaas <bahaas@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/13 14:06:30 by bahaas            #+#    #+#             */
/*   Updated: 2021/04/27 15:56:23 by bahaas           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	close_fd(int fd)
{
	if (fd > 0)
		close(fd);
}

void	reset_fd(t_ms *ms)
{
	dup2(ms->in, STDIN);
	dup2(ms->out, STDOUT);
	close_fd(ms->fdin);
	close_fd(ms->fdout);
	close_fd(ms->pipin);
	close_fd(ms->pipout);
	ms->fdin = -1;
	ms->fdout = -1;
	ms->pipin = -1;
	ms->pipout = -1;
}

t_cmd	*next_cmd_to_execute(t_cmd *cmd)
{
	cmd = cmd->next;
	while (cmd && cmd->type_link != CMD_ARGS)
	{
		cmd = cmd->next;
		if (cmd && cmd->type_link == CMD_ARGS && cmd->prev->type_link != 5)
			cmd = cmd->next;
		if (cmd && (cmd->type_link == 6 ||
					cmd->type_link == 7 ||
					cmd->type_link == 8) && cmd->prev->type_link == 5)
			break ;
	}
	return (cmd);
}

/*
** 1. Execute cmd block per block (séparateur = ; )
** 2. After execution, we reset our fd to stdin & stdout for next command
** 3. On se déplace au prochain bloc de commande (premiere commande après un ;)
*/

void	setup_execution(t_ms *ms, t_cmd *cmd)
{
	int status;
	int has_redir_first = 0;

	ms->no_exec = 0;
	ms->ret = 0;
	while(cmd && is_redir(cmd))
	{
	//	printf("cmd in redir loop : %s\n", cmd->content[0]);
		if (is_type(cmd, DRIGHT))
			redir(ms, cmd->next, DRIGHT);
		else if (is_type(cmd, RIGHT))
			redir(ms, cmd->next, RIGHT);
		else if (is_type(cmd, LEFT))
			input(ms, cmd->next);
		if(cmd)
		cmd = cmd->next->next;
		has_redir_first = 1;
		
		if(ms->ret)
		{
			ms->last_ret = 1;
			break;
		}
	}
	if(has_redir_first && is_type(cmd, PIPES) && cmd->next)
		cmd = cmd->next;
	if(ms->ret)
		reset_fd(ms);
	ms->no_exec = 0;
	while (ms->exit == 1 && cmd && !ms->ret)
	{
	//	printf("go in loop to exec with cmd : %s\n", cmd->content[0]);
		ms->flag = 0;
		ms->recursive = 1;
		select_action(ms, cmd);
		reset_fd(ms);
		waitpid(-1, &status, 0);
		if (ms->flag == 1)
			exit(0);
		ms->no_exec = 0;
		cmd = next_cmd_to_execute(cmd);
	}
	reset_fd(ms);
}

