/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bahaas <bahaas@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/18 14:15:43 by bahaas            #+#    #+#             */
/*   Updated: 2021/03/23 14:39:19 by bahaas           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int		ft_echo(t_ms *ms, t_cmd *cmd)
{
	int no_newline;
	int i;

	no_newline = 0;
	i = 1;
	while(cmd->content[i] && !ft_strcmp(cmd->content[i], "-n")) //loop here with cmp cuz "echo -n -n -n" return nothing
	{
		no_newline = 1;
		i++;
	}
	while(cmd->content[i])
	{
		write(1, cmd->content[i], ft_strlen(cmd->content[i]));
		if(cmd->content[i + 1])
			write(1, " ", 1);
		i++;
	}
	if(!no_newline)
		write(1, "\n", 1);
	return (0);
}
