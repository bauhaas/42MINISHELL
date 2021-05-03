/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bahaas <bahaas@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/03 11:47:55 by bahaas            #+#    #+#             */
/*   Updated: 2021/05/03 14:59:12 by bahaas           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static t_cmd	*next_cmd_to_execute(t_cmd *cmd)
{
	cmd = cmd->next;
	while (cmd)
	{
		cmd = cmd->next;
		if (cmd && ((is_redir(cmd)) || (is_type(cmd, PIPES)) ||
					(cmd->prev && is_redir(cmd->prev))))
			cmd = cmd->next;
		else
			break ;
	}
	return (cmd);
}

static int		last_pid_id(int nb_fork, int last_status[100][2])
{
	int			i;
	int			last_pid;
	int			last_i;

	i = 0;
	last_pid = 0;
	last_i = 0;
	while (i < nb_fork)
	{
		if (last_pid < last_status[i][0])
		{
			last_pid = last_status[i][0];
			last_i = i;
		}
		i++;
	}
	return (last_status[last_i][1]);
}

static void		waiting_loop(t_ms *ms)
{
	int			status;
	int			pid_fils;
	int			last_status[100][2];
	int			i;

	if (ms->forked)
	{
		pid_fils = 0;
		i = 0;
		while (pid_fils >= 0)
		{
			pid_fils = waitpid(-1, &status, 0);
			last_status[i][0] = pid_fils;
			if (WIFEXITED(status))
				ms->last_ret = WEXITSTATUS(status);
			if (WIFSIGNALED(status))
				ms->last_ret = WTERMSIG(status) + 128;
			last_status[i++][1] = ms->last_ret;
		}
		ms->last_ret = last_pid_id(ms->forked, last_status);
		ms->forked = 0;
	}
}

void			select_execution(t_ms *ms, t_cmd *cmd, int exit_in_pipeline)
{
	if (cmd && get_bltn(ms, cmd->content[0]))
	{
		ms->last_ret = launch_bltn(ms, cmd);
		if (exit_in_pipeline)
			exit(ms->last_ret);
	}
	else
		search_prog(ms, cmd);
}

void			pipeline(t_cmd *cmd, t_ms *ms)
{
	int			fd[2];
	pid_t		pid;
	int			fdd;

	ms->ret = 0;
	ms->forked = 0;
	first_cmd_is_redir(ms, &cmd);
	while (cmd && !ms->ret)
	{
		if (cmd && ft_strcmp(cmd->content[0], "exit"))
		{
			ms->forked++;
			pipe(fd);
			pid = fork();
			if (pid == -1 || ms->forked == 100)
				fork_error();
			else if (pid == 0)
				child_execution(ms, &cmd, fdd, fd);
			else
				parent_execution(&fdd, fd);
		}
		cmd = next_cmd_to_execute(cmd);
	}
	waiting_loop(ms);
}
