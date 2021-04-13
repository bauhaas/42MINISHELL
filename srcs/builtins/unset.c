/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bahaas <bahaas@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/21 00:41:56 by bahaas            #+#    #+#             */
/*   Updated: 2021/04/12 17:18:46 by bahaas           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	delete_one(t_list **head_ref, char *name)
{
	t_list	*tmp;
	t_list	*prev;
	t_var	*var;

	tmp = *head_ref;
	prev = NULL;
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

int		ft_unset(t_ms *ms, t_cmd *cmd)
{
	char	*env_name;
	int		i;
	int		status;

	i = 0;
	status = 0;
	while (cmd->content[++i])
	{
		env_name = ft_getenv(&ms->env, cmd->content[i], 0);
		if (env_name)
		{
			delete_one(&ms->env, env_name);
			status |= 0;
		}
		else
		{
			ft_putstr_fd("unset :", 2);
			ft_putstr_fd(cmd->content[i], 2);
			ft_putstr_fd(": invalid parameter name\n", 2);
			status |= 1;
		}
	}
	return (status);
}
