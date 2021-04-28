/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   launch.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bahaas <bahaas@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/20 14:40:36 by bahaas            #+#    #+#             */
/*   Updated: 2021/04/28 15:03:25 by bahaas           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void		empty_cmd_content(t_ms *ms, t_cmd *cmd)
{
	if (!cmd->is_env)
	{
		printf("\nLa commande  «  » n'a pas été trouvée,");
		printf(" mais peut être installée avec :\n\n");
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
		ms->last_ret = 127;
	}
}

void		launch_exec(t_ms *ms, t_cmd *cmd)
{
	/*
	printf("ENTER IN EXEC\n");
	printf("with cmd : %s\n", cmd->content[0]);
	*/
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
	{
		if(!ft_strcmp(cmd->content[0], ".."))
			cmd->ret_value = 4;
		error_file(ms, cmd);
	}
}

void		launch_cmd(t_ms *ms, t_cmd *cmd, int pipe)
{
	if (ms->recursive == 0 && ft_strcmp(cmd->content[0], "cat") && pipe == 0)
		return ;
	if (ms->recursive == 0 && !ft_strcmp(cmd->content[0], "sleep")
			&& (pipe == 0 || pipe == 2))
		return ;
	if (ms->recursive == 0 && !ft_strcmp(cmd->content[0], "cat")
			&& pipe == 0 && cmd->prev)
		exit(0);
	if(ms->recursive == 0)
	{
		if(!ft_strcmp(cmd->content[0], "cat")&& cmd->content[1])
			return ;
		else if(!ft_strcmp(cmd->content[0], "cat") && !cmd->content[1])
			;
		else
			return ;
	}
	if (cmd && !ft_strcmp(cmd->content[0], "exit") && !has_pipe(cmd))
		ms->last_ret = ft_exit(ms, cmd);
	else if (cmd && get_bltn(ms, cmd->content[0]))
		ms->last_ret = launch_bltn(ms, cmd);
	else
	{
		if (cmd && !ft_strcmp(cmd->content[0], "\0"))
			empty_cmd_content(ms, cmd);
		else
		{
		//	printf("GO IN LAUNCH EXEC\n");
			launch_exec(ms, cmd);
			printf("after execution we are on pid : %d avec cmd qui a eu lieu: %s\n", getpid(), cmd->content[0]);
		}
	}
	close_fd(ms->pipin);
	close_fd(ms->pipout);
	ms->pipin = -1;
	ms->pipout = -1;
	ms->recursive = 0;
}

void		launch_redirection(t_ms *ms, t_cmd *cmd, int redirection_type)
{
	char *file;
/*
	printf("LAUNCH REDIRECTION\n");
	printf("with cmd : %s\n", cmd->content[0]);
	printf("with redirection_type : %d\n", redirection_type);
*/
	file = cmd->content[0];
	if (redirection_type == RIGHT || redirection_type == DRIGHT)
	{
		close_fd(ms->fdout);
		if (redirection_type == RIGHT)
			ms->fdout = open(file, O_CREAT | O_WRONLY | O_APPEND, 0644);
		else
			ms->fdout = open(file, O_CREAT | O_WRONLY | O_TRUNC, 0644);
		dup2(ms->fdout, STDOUT);
	}
	else if (redirection_type == LEFT)
	{
		close_fd(ms->fdin);
		ms->fdin = open(file, O_RDONLY, 0644);
		dup2(ms->fdin, STDIN);
	}
}
