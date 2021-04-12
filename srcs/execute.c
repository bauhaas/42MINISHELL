/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bahaas <bahaas@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/20 16:00:10 by bahaas            #+#    #+#             */
/*   Updated: 2021/04/12 17:01:02 by bahaas           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int launch_exec(t_ms *ms, t_cmd *cmd)
{
	int pid;
	int status;

	pid = 0;
	search_prog(ms, cmd);
	if (cmd->ret_value == 0)
	{
		pid = fork();
		if (pid == -1)
			printf("error  msg to display\n");
		else if (pid == 0)
		{
			execve(cmd->content[0], cmd->content, ms->arr_env);
			//	exit(0);
		}
		else
		{
			waitpid(pid, &status, 0);
			kill(pid, SIGTERM);
		}
	}
	else
		error_file(ms, cmd);
	return (0);
}

void	ft_close(int fd)
{
	if (fd > 0)
		close(fd);
}

int		minipipe(t_ms *ms)
{
//	printf("ENTER IN MINIPIPE\n");
	//printf("go to minipipe\n");
	pid_t	pid;
	int		pipefd[2];

	pipe(pipefd);
	pid = fork();
	if (pid == 0)
	{
		ft_close(pipefd[1]);
		dup2(pipefd[0], STDIN);
		ms->pipin = pipefd[0];
		ms->pid = -1;
		return (2);
	}
	else
	{
		ft_close(pipefd[0]);
		dup2(pipefd[1], STDOUT);
		ms->pipout = pipefd[1];
		ms->pid = pid;
		return (1);
	}
}

void	launch_cmd(t_ms *ms, t_cmd *cmd)
{
//	printf("ENTER IN LAUNCH_CMD\n");
	//printf("go to exec_cmd\n");
	if (ms->charge == 0)
		return ;
	if (cmd && !ft_strcmp(cmd->content[0], "exit") && !has_pipe(cmd))
		ft_exit(ms, cmd);
	else if (cmd && get_bltn(ms, cmd->content[0]))
	{
		//printf("\nWe execute the builtin : %s\n", cmd->content[0]);
		ms->ret = launch_bltn(ms, cmd);
	}
	else if (cmd)
	{
		//printf("\nWe fork and execute the cmd : %s\n", cmd->content[0]);
		ms->ret = launch_exec(ms, cmd);
	}
	ft_close(ms->pipin);
	ft_close(ms->pipout);
	ms->pipin = -1;
	ms->pipout = -1;
	ms->charge = 0;
//	printf("EXIT LAUNCH_CMD\n\n");
}

int	choose_action(t_ms *ms, t_cmd *cmd)
{
//	printf("\nENTER IN CHOOSE_ACTION\n");
	int pipe = 0;
	//PRINT PREV & NEXT INFO
	print_action(cmd);
	
	//TO DO REDIRECTION
	
	if (cmd->prev && is_type(cmd->prev, PIPES))
		pipe = minipipe(ms);
	if (cmd->next && !is_type(cmd->next, END_CMD) && pipe != 1)
		choose_action(ms, cmd->next->next);
	// PRINT INFO FOR THE NEXT CONDITION
	print_action_exec_condition(cmd, pipe, ms);
	if ((is_type(cmd->prev, END_CMD) || is_type(cmd->prev, PIPES) || !cmd->prev) && pipe != 1)
		launch_cmd(ms, cmd);
	return (0);
}

void	reset_std(t_ms *ms)
{
	dup2(ms->in, STDIN);
	dup2(ms->out, STDOUT);
}

void	close_fds(t_ms *ms)
{
	ft_close(ms->fdin);
	ft_close(ms->fdout);
	ft_close(ms->pipin);
	ft_close(ms->pipout);
}

void	reset_fds(t_ms *ms)
{
	ms->fdin = -1;
	ms->fdout = -1;
	ms->pipin = -1;
	ms->pipout = -1;
	ms->pid = -1;
}

t_cmd	*next_cmd_to_execute(t_cmd *cmd, int skip)
{
	
//	printf("ENTER IN NEXT RUN\n");
//	printf("Current cmd : %s\n", cmd->content[0]);
	
	if (cmd && skip)
		cmd = cmd->next;
//	if (cmd)
//		printf("Current cmd after one inc: %s\n", cmd->content[0]);
	while (cmd && cmd->type_link != CMD_ARGS)
	{
		cmd = cmd->next;
		if (cmd && cmd->type_link == CMD_ARGS && cmd->prev == NULL)
			;
		else if (cmd && cmd->type_link == CMD_ARGS && cmd->prev->type_link != 5)
			cmd = cmd->next;
	}
	//PRINT INFO 
	/*
	if (cmd)
		printf("Next cmd : %s\n", cmd->content[0]);
	else
		printf("Next cmd : NONE\n");
	*/
	return (cmd);
}

void	setup_execution(t_ms *ms, t_cmd *cmd)
{
	int		status;

	//printf("ENTER IN SETUP\n");
	while (ms->exit == 1 && cmd)
	{
		ms->charge = 1;
		choose_action(ms, cmd);
		reset_std(ms);
		close_fds(ms);
		reset_fds(ms);
		waitpid(-1, &status, 0);
		status = WEXITSTATUS(status);
		cmd = next_cmd_to_execute(cmd, SKIP);
	}
	//printf("END OF SET_UP_EXECUTE\n");
}
