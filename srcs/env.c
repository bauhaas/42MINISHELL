/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bahaas <bahaas@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/21 00:42:31 by bahaas            #+#    #+#             */
/*   Updated: 2021/03/21 23:16:36 by bahaas           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

/*
void display_env(void *content)
{
	printf("test\n");
}*/

int	ft_env(t_ms *ms, t_cmd *cmd)
{
	t_var *var;
	//ft_lstiter(ms->env, display_env(ms->env->content));
	if(!cmd->content[1])
	{
		while(ms->env)
		{
			var = (t_var *)ms->env->content;
			printf("%s", var->name);
			printf("=");
			printf("%s\n", var->value);
			ms->env = ms->env->next;
		}
	}
	return (0);
}
