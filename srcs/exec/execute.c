/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bahaas <bahaas@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/20 16:00:10 by bahaas            #+#    #+#             */
/*   Updated: 2021/04/28 15:06:28 by bahaas           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int			is_redir(t_cmd *cmd)
{
	if(cmd->type_link == 6 || cmd->type_link == 7 || cmd->type_link == 8)
		return (1);
	return (0);
}

void	ft_close(int fd)
{
	if (fd > 0)
		close(fd);
}

void		select_pipe(int *pipe_value, t_ms *ms, t_cmd *cmd)
{
	printf("ENTER IN SELECT PIPE ");
	printf("with cmd : %s\n", cmd->content[0]);
	pid_t	pid;
	int		fd[2];

	*pipe_value = 0;
	
	if (cmd && is_type(cmd, PIPES))
	{
		pipe(fd);
		pid = fork();
		printf("pid in pipes : %d\n", getpid());
		if (pid == 0)
		{
			close_fd(fd[1]);
			dup2(fd[0], STDIN);
			ms->pipin = fd[0];
			*pipe_value = 2;
			ms->flag = 1;
			ms->no_exec = 0;
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

int			is_pipe(t_cmd *cmd)
{
	if(cmd->type_link == 4)
		return (1);
	return (0);
}

int			has_valid_redir_or_pipe(t_cmd *cmd)
{
	if(cmd->next && (is_redir(cmd->next) || is_pipe(cmd->next)))
	{
	//	printf("test has valid redir or pipe\n");
		if(cmd->next->ret_value == 4)
		{
	//		printf("LOL HERE IN CONDITION FOR EXECUTE\n");
			return (0);
		}
	}
	return (1);
}

void	redir(t_ms *ms, t_cmd *cmd, int type)
{
	ft_close(ms->fdout);
	if (type == DRIGHT)
		ms->fdout = open(cmd->content[0], O_CREAT | O_WRONLY | O_TRUNC, S_IRWXU);
	else
		ms->fdout = open(cmd->content[0], O_CREAT | O_WRONLY | O_APPEND, S_IRWXU);
	if (ms->fdout == -1)
	{
		ft_putstr_fd("minishell: ", STDERR);
		ft_putstr_fd(cmd->content[0], STDERR);
		ft_putendl_fd(": No such file or directory", STDERR);
		ms->ret = 1;
		ms->no_exec = 1;
		return ;
	}
	dup2(ms->fdout, STDOUT);
}

void	input(t_ms *ms, t_cmd *cmd)
{
	ft_close(ms->fdin);
	ms->fdin = open(cmd->content[0], O_RDONLY, S_IRWXU);
	if (ms->fdin == -1)
	{
		ft_putstr_fd("minishell: ", STDERR);
		ft_putstr_fd(cmd->content[0], STDERR);
		ft_putendl_fd(": No such file or directory", STDERR);
		ms->ret = 1;
		ms->no_exec = 1;
		return ;
	}
	dup2(ms->fdin, STDIN);
}

void		select_action(t_ms *ms, t_cmd *cmd)
{
	/*
	int pipe;
	t_cmd *tmp;
	ms->lol = 0;

	printf("ENTER SELECT ACTION\n");
	if(cmd)
	printf("with cmd : %s\n", cmd->content[0]);
	tmp = cmd;
	if(select_redirection(ms, &cmd) == 0) // if an error in redirec stop execution
	{
		if(cmd && cmd->prev)
			cmd->prev->ret_value = 4;
		return(0);
	}
	if(is_type(cmd, PIPES) || (cmd->prev && is_type(cmd->prev, PIPES)))
		select_pipe(&pipe, ms, cmd->prev);
*/
	//printf("cmd after select pipe : %s\n", cmd->content[0]);
	//printf("pipe : %d\n", pipe);
	/*	
	if (cmd->next && !is_type(cmd->next, END_CMD) && pipe != 1 && cmd->prev && is_redir(cmd->prev))
	{
		printf("TEST LOL 1\n");
		ms->lol = 1;
		select_action(ms, cmd->next);
	}
	*/
/*
	if (cmd->next && !is_type(cmd->next, END_CMD) && pipe != 1 && ms->lol == 0)
	{
		printf("TEST LOL 0\n");
		//printf("cmd next next : %s\n", cmd->next->next->content[0]);
		select_action(ms, cmd->next->next);
	}
	while(tmp) //if error on a cmd stop the execution
	{
		if(tmp->next && tmp->next->ret_value != 0)
			return (0);
		tmp = tmp->next;
	}
*/
	/*
	printf("BEFORE EXEC : %s\n", cmd->content[0]);
	if(cmd->prev)
		printf("BEFORE EXEC PRV : %s\n", cmd->prev->content[0]);
	if (((is_type(cmd->prev, PIPES) || !cmd->prev) && pipe != 1 && has_valid_redir_or_pipe(cmd)))
	{
		printf("GO TO EXECUTE\n");
	//	printf("cmd : cmd->content[0] : %s\n", cmd->content[0]);
		launch_cmd(ms, cmd, pipe);
	}
	*/
/*
	if (((is_type(cmd->prev, PIPES) || !cmd->prev || (cmd->prev && cmd->prev->type_link == CMD_ARGS && cmd->prev->prev && is_redir(cmd->prev->prev))) && pipe != 1 && has_valid_redir_or_pipe(cmd)))
	{
		printf("GO TO EXECUTE\n");
		printf("with cmd : %s\n", cmd->content[0]);
		//printf("with lol : %d\n", ms->lol);

		launch_cmd(ms, cmd, pipe);
	}
	printf("EXIT SELECT ACTION\n\n");
	return (0);
*/
	printf("ENTER SELECT ACTION ");
//	if(cmd)
	printf("with cmd : %s\n", cmd->content[0]);
	t_cmd	*prev;
	t_cmd	*next;
	int		pipe;

	prev = cmd->prev;
	next = cmd->next;
	/*
	if(prev)
		printf("prev : %s\n", prev->content[0]);
	printf("curr : %s\n", cmd->content[0]);
	if(next)
		printf("next : %s\n", next->content[0]);
	*/
	pipe = 0;
	if(!ms->ret)
	{
	if (is_type(prev, DRIGHT))
		redir(ms, cmd, DRIGHT);
	else if (is_type(prev, RIGHT))
		redir(ms, cmd, RIGHT);
	else if (is_type(prev, LEFT))
		input(ms, cmd);
	}
	if (is_type(prev, PIPES))
		select_pipe(&pipe, ms, cmd->prev);
	if (next && pipe != 1)
	{
	//	printf("RECURSIVE CALL\n");
		select_action(ms, next->next);
	}
/*	
	printf("test cmd ret before execute : %d\n", cmd->ret_value);
	printf("cmd before execute : %s	\n", cmd->content[0]);
	if(cmd->next)
	{
	printf("cmd next before execute : %s\n", cmd->next->content[0]);
	printf("test cmd next ret before execute : %d\n", cmd->next->ret_value);
	}
*/	
	if ((is_type(prev, PIPES) || !prev || (cmd->prev && cmd->prev->type_link == CMD_ARGS && cmd->prev->prev && is_redir(cmd->prev->prev))) && pipe != 1 && ms->no_exec == 0)
	{
	//	printf("test\n");
		printf("GO TO EXECUTE ");
		printf("with cmd : %s\n", cmd->content[0]);
		//if(is_type(next, PIPES) && next->next && next->next->ret_value != 0)
		//	return ;
		//else
		launch_cmd(ms, cmd, pipe);
		printf("EXIT EXECUTE\n");
	}
	
//	printf("test cmd ret after execute : %d\n", cmd->ret_value);
//	printf("test last ret after execute : %d\n", ms->last_ret);
	printf("ms->last_ret before exit select_action : %d\n", ms->last_ret);
	if(cmd->next && cmd->next->ret_value != 0)
		cmd->ret_value = cmd->next->ret_value; 
	if(ms->ret)
		ms->last_ret = ms->ret;
	printf("EXIT SELECT ACTION\n\n");
}
