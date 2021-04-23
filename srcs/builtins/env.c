/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bahaas <bahaas@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/21 00:42:31 by bahaas            #+#    #+#             */
/*   Updated: 2021/04/21 15:53:48 by bahaas           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void		free_env(void *env)
{
	t_var	*e;

	e = (t_var*)env;
	ft_strdel(&e->name);
	ft_strdel(&e->value);
	free(e);
}

void		print_env(t_var *env, int mod)
{
	if (mod == ENV)
	{
		if (!env->value)
			return ;
		if (env->value[0] == '\0')
			printf("%s=\n", env->name);
		else
			printf("%s=%s\n", env->name, env->value);
	}
	else
	{
		printf("declare -x %s", env->name);
		if (env->value == NULL)
			printf("\n");
		else if (env->value[0] == '\0')
			printf("=\'\'\n");
		else
			printf("=\'%s\'\n", env->value);
	}
}

int			ft_env(t_ms *ms, t_cmd *cmd)
{
	t_list	*tmp;

	(void)ms;
	(void)cmd;
	tmp = ms->env;
	if (!cmd->content[1])
	{
		while (tmp)
		{
			print_env((t_var *)tmp->content, ENV);
			tmp = tmp->next;
		}
	}
	return (0);
}
