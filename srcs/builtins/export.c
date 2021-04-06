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

void			ft_list_sort(t_list **begin_list)
{
	t_list		*actuel_node;
	t_list 		*next_node;
	t_var		*env;
	t_var		*env_next;

	actuel_node = *begin_list;
	while (actuel_node->next)
	{
		env = (t_var*)actuel_node->content;
		env_next = (t_var*)actuel_node->next->content;
		if (strcmp(env->name, env_next->name) > 0)
		{
			ft_lstswap(actuel_node,actuel_node->next);
			actuel_node = *begin_list;
		}
		else
			actuel_node = actuel_node->next;
	}
}

void			*list_sort(t_list **dest, t_list *list)
{
		t_list	*tmp;

		tmp = list;
		while (tmp)
		{
			ft_lstadd_front(dest, ft_memdup(tmp, sizeof(tmp)));
			tmp = tmp->next;
		}
		ft_list_sort(dest);
		return (dest);
}

void			free_(void *env)
{
	t_var *e;

	e = (t_var*)env;
	ft_strdel(&e->name);
	ft_strdel(&e->value);
}

int	ft_export(t_ms *ms, t_cmd *cmd)
{
	t_list 		*tmp;
	t_list		*copy;

	tmp = ms->env;
	copy = NULL;
	if (!cmd->content[1])
	{
		list_sort(&copy, tmp);
		while(copy)
		{
			print_env((t_var *)copy->content, EXPORT);
			copy = copy->next;
		}
		ft_lstclear(&copy, &free_);
	}
	else
	{
		printf("TODO\n");
	}
	return (0);
}
