/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bahaas <bahaas@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/21 00:41:56 by bahaas            #+#    #+#             */
/*   Updated: 2021/05/06 03:06:03 by bahaas           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static void	delete_one(t_list **head_ref, char *name)
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

static void	unset_error(char *bad_identier)
{
	ft_putstr_fd("minishell: unset: '", 2);
	ft_putstr_fd(bad_identier, 2);
	ft_putstr_fd("': not a valid identifier\n", 2);
}

int			ft_unset(t_ms *ms, t_cmd *cmd)
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
		else if (cmd->content[i][0] == '\0' || cmd->content[i][0] == '=' ||
			!valid_name(cmd->content[i]))
		{
			unset_error(cmd->content[i]);
			status |= 1;
		}
		else
			status |= 0;
	}
	return (status);
}
