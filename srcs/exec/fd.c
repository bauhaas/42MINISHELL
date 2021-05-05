/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bahaas <bahaas@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/03 15:28:15 by bahaas            #+#    #+#             */
/*   Updated: 2021/05/05 13:07:08 by bahaas           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	ft_close(int fd)
{
	if (fd > 0)
		close(fd);
}

void	reset_fd(t_ms *ms)
{
	dup2(ms->in, STDIN);
	dup2(ms->out, STDOUT);
	ft_close(ms->fdin);
	ft_close(ms->fdout);
	ms->fdin = -1;
	ms->fdout = -1;
}
