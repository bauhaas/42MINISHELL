/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bahaas <bahaas@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/03 11:47:55 by bahaas            #+#    #+#             */
/*   Updated: 2021/05/03 11:55:54 by bahaas           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static t_cmd	*next_cmd_to_execute(t_ms *ms, t_cmd *cmd)
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

static void		waiting_loop(t_ms *ms, t_cmd *cmd)
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
	int			has_redir_first;

	ms->ret = 0;
	ms->forked = 0;
	has_redir_first = 0;
	fd = 0;
	while (cmd && is_redir(cmd))
	{
		if (is_type(cmd, DRIGHT))
			redir(ms, cmd->next, DRIGHT);
		else if (is_type(cmd, RIGHT))
			redir(ms, cmd->next, RIGHT);
		else if (is_type(cmd, LEFT))
			input(ms, cmd->next);
		if (cmd)
			cmd = cmd->next->next;
		has_redir_first = 1;
		if (ms->ret)
		{
			ms->last_ret = 1;
			break ;
		}
	}
	if (has_redir_first && is_type(cmd, PIPES) && cmd->next)
		cmd = cmd->next;
	reset_fd(ms);
	while (cmd && !ms->ret)
	{
		if (cmd && !ft_strcmp(cmd->content[0], "exit"))
			;
		else
		{
			ms->forked++;
			pipe(fd);
			pid = fork();
			if (pid == -1 || ms->forked == 100)
			{
				ft_putstr_fd("Minishell: fork: Out of memory\n", 2);
				exit(12);
			}
			else if (pid == 0)
			{
				dup2(fdd, STDIN);
				if (cmd->next && cmd->next->type_link == 4)
					dup2(fd[1], STDOUT);
				if (cmd->next && is_redir(cmd->next))
					set_redirection(ms, cmd);
				if (ms->ret)
					exit(ms->ret);
				close(fd[0]);
				select_execution(ms, cmd, 1);
				if (cmd->ret_value)
					exit(ms->last_ret);
				execve(cmd->content[0], cmd->content, ms->arr_env);
			}
			else
			{
				close(fd[1]);
				fdd = fd[0];
			}
		}
		cmd = next_cmd_to_execute(ms, cmd);
	}
	waiting_loop(ms, cmd);
}
