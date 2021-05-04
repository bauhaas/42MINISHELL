/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bahaas <bahaas@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/03 15:28:15 by bahaas            #+#    #+#             */
/*   Updated: 2021/05/03 23:20:03 by bahaas           ###   ########.fr       */
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
//	ft_close(ms->pipin);
//	ft_close(ms->pipout);
	ms->fdin = -1;
	ms->fdout = -1;
//	ms->pipin = -1;
//	ms->pipout = -1;
}
