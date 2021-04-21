/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clorin <clorin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/21 13:22:34 by clorin            #+#    #+#             */
/*   Updated: 2021/04/21 15:54:17 by bahaas           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void		ft_list_sort(t_list **begin_list)
{
	t_list	*actuel_node;
	t_var	*env;
	t_var	*env_next;

	actuel_node = *begin_list;
	while (actuel_node->next)
	{
		env = (t_var*)actuel_node->content;
		env_next = (t_var*)actuel_node->next->content;
		if (strcmp(env->name, env_next->name) > 0)
		{
			ft_lstswap(actuel_node, actuel_node->next);
			actuel_node = *begin_list;
		}
		else
			actuel_node = actuel_node->next;
	}
}

void		*list_sort(t_list **dest, t_list *list)
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
