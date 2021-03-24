/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bahaas <bahaas@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/21 00:41:56 by bahaas            #+#    #+#             */
/*   Updated: 2021/03/24 09:58:31 by bahaas           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void delete_one(t_list** head_ref, char *name)
{
	t_list* tmp = *head_ref;
	t_list* prev = NULL;
	t_var *var;

	var = (t_var *)tmp->content;
	if (tmp != NULL && !ft_strcmp(var->name, name))
	{
		*head_ref = tmp->next;
		free(tmp);
		return ;
	}
	else
	{
		while (tmp != NULL && ft_strcmp(var->name, name))
		{
			prev = tmp;
			tmp = tmp->next;
			var = (t_var *)tmp->content;
		}
		if (tmp == NULL)
			return ;
		prev->next = tmp->next;
		free(tmp);
	}
}

int	ft_unset(t_ms *ms, t_cmd *cmd)
{
	char	*env_name;
	int		i;

	i = 0;
	while(cmd->content[++i])
	{
		env_name = ft_getenv(&ms->env, cmd->content[i], 0);
		if(env_name)
			delete_one(&ms->env, env_name);
	}
	return (0);
}
