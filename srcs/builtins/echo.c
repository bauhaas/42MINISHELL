/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bahaas <bahaas@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/18 14:15:43 by bahaas            #+#    #+#             */
/*   Updated: 2021/04/12 17:27:01 by bahaas           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int		ft_echo(t_ms *ms, t_cmd *cmd)
{
	int	no_newline;
	int	i;
	int	j;

	no_newline = 0;
	j = 0;
	i = 1;
	while (cmd->content[i] && cmd->content[i][j] == '-')
	{
		j++;
		while (cmd->content[i][j] == 'n')
			j++;
		no_newline = 1;
		if (cmd->content[i][j] == '-')
			break ;
		if (!cmd->content[i][j])
		{
			i++;
			j = 0;
		}
	}
	while (cmd->content[i])
	{
		if (cmd->content[i][0] == '\0')
			printf(" ");
		printf("%s", cmd->content[i]);
		if (cmd->content[i][0] != 0 && cmd->content[i + 1])
			printf(" ");
		i++;
	}
	if (!no_newline)
		printf("\n");
	//ms->last_ret = 0;
	return (0);
}
