/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bahaas <bahaas@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/26 11:35:35 by bahaas            #+#    #+#             */
/*   Updated: 2021/05/03 11:54:46 by bahaas           ###   ########.fr       */
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
		if(cmd->next->ret_value == 4)
			return (0);
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


void set_redirection(t_ms *ms, t_cmd *cmd)
{
	if(DEBUG)
		printf("cmd enter in set_redir : %s\n", cmd->content[0]);
	t_cmd *first_cmd;
	t_cmd *redir_cmd;
	t_cmd *file_to_redirect;

	first_cmd = cmd;
	if(cmd && (cmd->next && is_redir(cmd->next)))
	{
		redir_cmd = cmd->next;
		if(redir_cmd->next)
			file_to_redirect = redir_cmd->next;
	}
	if(DEBUG)
	{
		if(redir_cmd)
			printf("cmd_redir : %s\n", redir_cmd->content[0]);
		if(file_to_redirect)
			printf("file_to_redirect : %s\n", file_to_redirect->content[0]);
	}
	while((redir_cmd && is_redir(redir_cmd)))
	{
		if (is_type(redir_cmd, DRIGHT))
			redir(ms, file_to_redirect, DRIGHT);
		else if (is_type(redir_cmd, RIGHT))
			redir(ms, file_to_redirect, RIGHT);
		else if (is_type(redir_cmd, LEFT))
			input(ms, file_to_redirect);
		if(ms->ret == 1)
			break;
		if(redir_cmd)
		{
			if(redir_cmd->next && redir_cmd->next->next)
				redir_cmd = redir_cmd->next->next;
			else
				break;
			if(file_to_redirect->next && file_to_redirect->next->next)
				file_to_redirect = file_to_redirect->next->next;
			else
				break;
		}
	}
}

