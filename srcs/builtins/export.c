/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bahaas <bahaas@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/20 20:09:25 by bahaas            #+#    #+#             */
/*   Updated: 2021/03/21 00:40:30 by bahaas           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	ft_export(t_ms *ms, t_cmd *cmd)
{
	t_list *tmp = ms->env;

	if (!cmd->content[1])
	{
		while(tmp)
		{
			print_env((t_var *)tmp->content, EXPORT);
			tmp = tmp->next;
		}
	}
	else
	{
		printf("TODO !\n");
	}
	return (0);
}
