/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bahaas <bahaas@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/18 14:15:43 by bahaas            #+#    #+#             */
/*   Updated: 2021/04/07 16:19:51 by bahaas           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int		ft_echo(t_ms *ms, t_cmd *cmd)
{
	int no_newline;
	int i;
	int j;

	no_newline = 0;
	j = 0;
	i = 1;
	while(cmd->content[i] && cmd->content[i][j] == '-')
	{
		j++;
		while(cmd->content[i][j] == 'n')
			j++;
		no_newline = 1;
		if(cmd->content[i][j] == '-')
			break;
		if(!cmd->content[i][j])
		{
			i++;
			j = 0;
		}
	}
	while(cmd->content[i])
	{
		write(1, cmd->content[i], ft_strlen(cmd->content[i]));
		if(cmd->content[i][0] != 0 && cmd->content[i + 1])
			write(1, " ", 1);
		i++;
	}
	if(!no_newline)
		write(1, "\n", 1);
	return (0);
}
