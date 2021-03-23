/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bahaas <bahaas@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/21 00:42:31 by bahaas            #+#    #+#             */
/*   Updated: 2021/03/23 15:09:36 by bahaas           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	ft_env(t_ms *ms, t_cmd *cmd)
{
	t_var *var;
	t_list *tmp = ms->env;
	if(!cmd->content[1])
	{
		while(tmp)
		{
			var = (t_var *)tmp->content;
			printf("%s", var->name);
			printf("=");
			printf("%s\n", var->value);
			tmp = tmp->next;
		}
	}
	return (0);
}
