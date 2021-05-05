/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bahaas <bahaas@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/21 00:44:41 by bahaas            #+#    #+#             */
/*   Updated: 2021/05/05 14:29:55 by bahaas           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void		free_split(char ***split)
{
	int i;

	i = -1;
	while ((*split)[++i])
	{
		free((*split)[i]);
		(*split)[i] = NULL;
	}
	free(*split);
	*split = NULL;
}

char		*ft_getenv(t_list **head_ref, char *elem, int i)
{
	t_var	*var;
	t_list	*tmp;

	tmp = *head_ref;
	while (tmp)
	{
		var = (t_var *)tmp->content;
		if (!ft_strcmp(var->name, elem))
		{
			if (i)
				return (var->value);
			return (var->name);
		}
		tmp = tmp->next;
	}
	return (NULL);
}

t_var		*ft_get_t_var(t_list **head_ref, char *elem)
{
	t_list	*tmp;
	t_var	*dest;

	tmp = *head_ref;
	while (tmp)
	{
		dest = (t_var*)tmp->content;
		if (!ft_strcmp(dest->name, elem))
			return (dest);
		tmp = tmp->next;
	}
	return (NULL);
}
