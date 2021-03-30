/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bahaas <bahaas@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/21 00:44:02 by bahaas            #+#    #+#             */
/*   Updated: 2021/03/29 12:02:05 by bahaas           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void			afree_(void *env)
{
	t_var *e;

	e = (t_var*)env;
	ft_strdel(&e->name);
	ft_strdel(&e->value);
	free(e);
}

int ft_exit(t_ms *ms, t_cmd *cmd)
{
	
	int i = 0;
	while(ms->bltn->bltn_name[i])
	{
		free(ms->bltn->bltn_name[i]);
		i++;
	}
	free(ms->bltn);
	free(ms->pwd);
	free(ms->old_pwd);
	ft_lstclear(&ms->env, &afree_);
	exit(0);
	return (0);
}
