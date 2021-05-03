/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fork.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bahaas <bahaas@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/03 14:55:03 by bahaas            #+#    #+#             */
/*   Updated: 2021/05/03 14:58:07 by bahaas           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void			child_execution(t_ms *ms, t_cmd **cmd, int fdd, int *fd)
{
	dup2(fdd, STDIN);
	if ((*cmd)->next && (*cmd)->next->type_link == 4)
		dup2(fd[1], STDOUT);
	if ((*cmd)->next && is_redir((*cmd)->next))
		set_redirection(ms, (*cmd));
	if (ms->ret)
		exit(ms->ret);
	close(fd[0]);
	select_execution(ms, (*cmd), 1);
	if ((*cmd)->ret_value)
		exit(ms->last_ret);
	execve((*cmd)->content[0], (*cmd)->content, ms->arr_env);
}

void			parent_execution(int *fdd, int *fd)
{
	close(fd[1]);
	*fdd = fd[0];
}

void			fork_error(void)
{
	ft_putstr_fd("Minishell: fork: Out of memory\n", 2);
	exit(12);
}
