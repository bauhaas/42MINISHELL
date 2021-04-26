/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bahaas <bahaas@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/18 14:15:43 by bahaas            #+#    #+#             */
/*   Updated: 2021/04/21 15:52:58 by bahaas           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int		ft_echo(t_ms *ms, t_cmd *cmd)
{
	(void)cmd;
	(void)ms;
	int	no_newline;
	int def_newline;
	int	i;
	int	j;

	no_newline = 0;
	def_newline = 0;
	j = 0;
	i = 1;
	while (cmd->content[i] && cmd->content[i][j] == '-')
	{
		if (!ft_strcmp(cmd->content[i], "-"))
		{
			def_newline = 0;
			break ;
		}
		j++;
		while (cmd->content[i][j] == 'n')
		{
			j++;
			no_newline = 1;
		}
		if (cmd->content[i][j] && cmd->content[i][j] != 'n')
		{
			no_newline = 0;
			break ;
		}
		if (!cmd->content[i][j])
		{
			i++;
			j = 0;
			if (no_newline)
				def_newline = 1;
		}
	}
	while (cmd->content[i])
	{
		if (cmd->content[i][0] == '\0' && !cmd->is_env )
			write(1, " ", 1);
		ft_putstr_fd(cmd->content[i], 1);
		if (cmd->content[i][0] != 0 && cmd->content[i + 1])
			write(1, " ", 1);
		i++;
	}
	if (!def_newline)
		write(1, "\n", 1);
	return (0);
}
