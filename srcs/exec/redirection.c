/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bahaas <bahaas@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/26 11:35:35 by bahaas            #+#    #+#             */
/*   Updated: 2021/05/03 23:21:03 by bahaas           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	redir(t_ms *ms, t_cmd *cmd, int type)
{
	ft_close(ms->fdout);
	if (type == DRIGHT)
		ms->fdout = open(cmd->content[0], O_CREAT | O_WRONLY
				| O_APPEND, S_IRWXU);
	else
		ms->fdout = open(cmd->content[0], O_CREAT | O_WRONLY
				| O_TRUNC, S_IRWXU);
	if (ms->fdout == -1)
	{
		ft_putstr_fd("minishell: ", STDERR);
		ft_putstr_fd(cmd->content[0], STDERR);
		ft_putendl_fd(": No such file or directory", STDERR);
		ms->ret = 1;
	//	ms->no_exec = 1;
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
	//	ms->no_exec = 1;
		return ;
	}
	dup2(ms->fdin, STDIN);
}

void	select_redirection(t_ms *ms, t_cmd *redir_cmd, t_cmd *file_to_redirect)
{
	if (is_type(redir_cmd, DRIGHT))
		redir(ms, file_to_redirect, DRIGHT);
	else if (is_type(redir_cmd, RIGHT))
		redir(ms, file_to_redirect, RIGHT);
	else if (is_type(redir_cmd, LEFT))
		input(ms, file_to_redirect);
}

void	set_redirection_var(t_cmd *cmd, t_cmd **redir, t_cmd **file)
{
	*redir = NULL;
	*file = NULL;
	if (cmd && (cmd->next && is_redir(cmd->next)))
	{
		*redir = cmd->next;
		if ((*redir)->next)
			*file = (*redir)->next;
	}
}

void	set_redirection(t_ms *ms, t_cmd *cmd)
{
	t_cmd *redir_cmd;
	t_cmd *file_to_redirect;

	set_redirection_var(cmd, &redir_cmd, &file_to_redirect);
	while ((redir_cmd && is_redir(redir_cmd)))
	{
		select_redirection(ms, redir_cmd, file_to_redirect);
		if (ms->ret == 1)
			break ;
		if (redir_cmd)
		{
			if (redir_cmd->next && redir_cmd->next->next)
				redir_cmd = redir_cmd->next->next;
			else
				break ;
			if (file_to_redirect->next && file_to_redirect->next->next)
				file_to_redirect = file_to_redirect->next->next;
			else
				break ;
		}
	}
}
