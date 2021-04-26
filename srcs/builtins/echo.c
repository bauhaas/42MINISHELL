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

static int		new_line(t_cmd *cmd, int *i)
{
	int			j;
	int			def_newline;
	int			no_newline;

	j = 0;
	def_newline = 0;
	no_newline = 0;
	while (cmd->content[*i] && cmd->content[*i][j++] == '-')
	{
		if (!ft_strcmp(cmd->content[*i], "-"))
			return (0);
		while (cmd->content[*i][j] == 'n' && (no_newline = 1))
			j++;
		if (cmd->content[*i][j] && cmd->content[*i][j] != 'n')
			return (def_newline);
		if (!cmd->content[*i][j])
		{
			(*i)++;
			j = 0;
			if (no_newline)
				def_newline = 1;
		}
	}
	return (def_newline);
}

int				ft_echo(t_ms *ms, t_cmd *cmd)
{
	int			def_newline;
	int			i;

	def_newline = 0;
	i = 1;
	def_newline = new_line(cmd, &i);
	while (cmd->content[i])
	{
		if (cmd->content[i][0] == '\0' && !cmd->is_env)
			write(1, " ", 1);
		if (ft_strcmp(cmd->content[i], "~") == 0)
			ft_putstr_fd(ft_getenv(&ms->env, "HOME", 1), STDOUT);
		else
			ft_putstr_fd(cmd->content[i], STDOUT);
		if (cmd->content[i][0] != 0 && cmd->content[i + 1])
			write(1, " ", 1);
		i++;
	}
	if (!def_newline)
		write(1, "\n", 1);
	return (0);
}
