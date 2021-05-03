/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bahaas <bahaas@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/20 16:00:10 by bahaas            #+#    #+#             */
/*   Updated: 2021/05/03 11:26:59 by bahaas           ###   ########.fr       */
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
