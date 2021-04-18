/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bahaas <bahaas@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/20 16:00:10 by bahaas            #+#    #+#             */
/*   Updated: 2021/04/19 00:21:40 by bahaas           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int launch_exec(t_ms *ms, t_cmd *cmd)
{
	int pid;
	int child_pid;
	int status;

	search_prog(ms, cmd);
	if (cmd->ret_value == 0)
	{
		ms->last_ret = 0;
		pid = fork();
		if (pid == 0)
			execve(cmd->content[0], cmd->content, ms->arr_env);
		else
		{
			ms->pid = pid;
			child_pid = waitpid(0, &status, 0);
			if (WIFEXITED(status))
				ms->last_ret = WEXITSTATUS(status);
			if (WIFSIGNALED(status))
				ms->last_ret = WTERMSIG(status) + 128;
			kill(pid, SIGTERM);
			ms->pid = 0;
		}
	}
	else
		error_file(ms, cmd);
	return (ms->last_ret);
}

void		set_pipe(int *pipe_value, t_ms *ms, t_cmd *cmd)
{
	pid_t	pid;
	int		fd[2];

	*pipe_value = 0;
	if(cmd && is_type(cmd, PIPES))
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

int empty_cmd_content(t_ms *ms, t_cmd *cmd)
{
	printf("\nLa commande  «  » n'a pas été trouvée, mais peut être installée avec :\n\n");
	printf("sudo apt install libpam-mount\n");
	printf("sudo apt install openssh-server\n");
	printf("sudo apt install openvswitch-common\n");
	printf("sudo apt install openvswitch-switch\n");
	printf("sudo apt install php-common\n");
	printf("sudo apt install bpfcc-tools\n");
	printf("sudo apt install burp\n");
	printf("sudo apt install cryptomount\n");
	printf("sudo apt install dolphin-emu\n");
	printf("sudo apt install mailutils-mh\n");
	printf("sudo apt install mmh\n");
	printf("sudo apt install nmh\n");
	printf("\n");
	return(127);
}

void	launch_cmd(t_ms *ms, t_cmd *cmd)
{
	if (ms->recursive == 0 && ft_strcmp(cmd->content[0], "\0"))
		return;
	if (cmd && !ft_strcmp(cmd->content[0], "exit") && !has_pipe(cmd))
		ms->last_ret = ft_exit(ms, cmd);
	else if (cmd && get_bltn(ms, cmd->content[0]))
		ms->last_ret = launch_bltn(ms, cmd);
	else
	{
		if(cmd && !ft_strcmp(cmd->content[0], "\0"))
			ms->last_ret = empty_cmd_content(ms, cmd);
		else
			ms->last_ret = launch_exec(ms, cmd);
	}
	close_fd(ms->pipin);
	close_fd(ms->pipout);
	ms->pipin = -1;
	ms->pipout = -1;
	ms->recursive = 0;
}

void set_redirection(t_ms *ms, t_cmd *cmd)
{
	t_cmd *tmp = cmd;
	if(tmp->type_link == 6)
	{
		tmp = cmd->next;
		valid_file(tmp);
		if(tmp->ret_value == 0)
		{
		close_fd(ms->fdin);
		ms->fdin = open(tmp->content[0], O_RDONLY, 0644);
		dup2(ms->fdin, STDIN);
		}
		else
		{
			error_file(ms, tmp);
			ms->last_ret = 1;
		}
		*cmd = *cmd->next;
	}
	if(tmp->type_link == 7)
	{
		tmp = cmd->next;
		close_fd(ms->fdout);
		ms->fdout = open(tmp->content[0], O_CREAT | O_WRONLY | O_APPEND, 0644);
		dup2(ms->fdout, STDOUT);
		*cmd = *cmd->next;
	}
	if(tmp->type_link == 8)
	{
		tmp = cmd->next;
		close_fd(ms->fdout);
		ms->fdout = open(tmp->content[0], O_CREAT | O_WRONLY | O_TRUNC, 0644);
		dup2(ms->fdout, STDOUT);
		*cmd = *cmd->next;
	}
	if(cmd->prev && cmd->prev->type_link != 5)
	{
		if(cmd->prev->type_link == 6)
		{
			valid_file(cmd);
			if(cmd->ret_value == 0)
			{
			close_fd(ms->fdin);
			ms->fdin = open(cmd->content[0], O_RDONLY, 0644);
			dup2(ms->fdin, STDIN);
			}
			else
				error_file(ms, cmd);
		}
		else if (cmd->prev->type_link == 7)
		{
			close_fd(ms->fdout);
			ms->fdout = open(cmd->content[0], O_CREAT | O_WRONLY | O_APPEND, 0644);
			dup2(ms->fdout, STDOUT);
		}
		else if (cmd->prev->type_link == 8)
		{
			close_fd(ms->fdout);
			ms->fdout = open(cmd->content[0], O_CREAT | O_WRONLY | O_TRUNC, 0644);
			dup2(ms->fdout, STDOUT);
		}
	}
}

int	choose_action(t_ms *ms, t_cmd *cmd)
{
	int pipe;

	set_redirection(ms, cmd);
	set_pipe(&pipe, ms, cmd->prev);
	if (cmd->next && !is_type(cmd->next, END_CMD) && pipe != 1)
		choose_action(ms, cmd->next->next);
	if ((is_type(cmd->prev, END_CMD) || is_type(cmd->prev, PIPES) || !cmd->prev) && pipe != 1)
		launch_cmd(ms, cmd);
	return (0);
}
