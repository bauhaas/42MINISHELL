/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bahaas <bahaas@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/20 16:00:10 by bahaas            #+#    #+#             */
/*   Updated: 2021/04/21 22:55:09 by bahaas           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void		select_pipe(int *pipe_value, t_ms *ms, t_cmd *cmd)
{
	pid_t	pid;
	int		fd[2];

	*pipe_value = 0;
	if (cmd && is_type(cmd, PIPES))
	{
		pipe(fd);
		pid = fork();
		if (pid == 0)
		{
			close_fd(fd[1]);
			dup2(fd[0], STDIN);
			ms->pipin = fd[0];
			*pipe_value = 2;
			ms->flag = 1;
		}
		else
		{
			close_fd(fd[0]);
			dup2(fd[1], STDOUT);
			ms->pipout = fd[1];
			*pipe_value = 1;
		}
	}
}

void		select_redirection(t_ms *ms, t_cmd *cmd)
{
	t_cmd *tmp;

	tmp = cmd;
	if (tmp->type_link == 6)
	{
		tmp = cmd->next;
		valid_file(tmp);
		if (tmp->ret_value == 0)
			launch_redirection(ms, tmp, tmp->prev->type_link);
		else
		{
			error_file(ms, tmp);
			ms->last_ret = 1;
		}
		*cmd = *cmd->next;
	}
	if (tmp->type_link == 7 || tmp->type_link == 8)
	{
		tmp = cmd->next;
		launch_redirection(ms, tmp, tmp->prev->type_link);
		*cmd = *cmd->next;
	}
	if (cmd->prev && cmd->prev->type_link != 5)
	{
		if (cmd->prev->type_link == 6)
		{
			valid_file(cmd);
			if (cmd->ret_value == 0)
			{
				close_fd(ms->fdin);
				ms->fdin = open(cmd->content[0], O_RDONLY, 0644);
				dup2(ms->fdin, STDIN);
			}
			else
				error_file(ms, cmd);
		}
		else if (cmd->prev->type_link == 7 || cmd->prev->type_link == 8)
			launch_redirection(ms, cmd, cmd->prev->type_link);
	}
}

int			select_action(t_ms *ms, t_cmd *cmd)
{
	int pipe;

	select_redirection(ms, cmd);
	select_pipe(&pipe, ms, cmd->prev);
	if (cmd->next && !is_type(cmd->next, END_CMD) && pipe != 1)
		select_action(ms, cmd->next->next);
	if ((is_type(cmd->prev, END_CMD) || is_type(cmd->prev, PIPES) || !cmd->prev)
			&& pipe != 1)
		launch_cmd(ms, cmd);
	return (0);
}
