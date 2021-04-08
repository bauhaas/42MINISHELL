/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bahaas <bahaas@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/21 00:44:41 by bahaas            #+#    #+#             */
/*   Updated: 2021/04/08 22:58:09 by bahaas           ###   ########.fr       */
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


char 		*ft_getenv(t_list **head_ref, char *elem, int i)
{
	t_var 	*var;
	t_list 	*tmp;

	tmp = *head_ref;
	while(tmp)
	{
		var = (t_var *)tmp->content;
		if(!ft_strcmp(var->name, elem))
		{
			if(i)
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
		tmp =tmp->next;
	}
	return (NULL);
}

int		is_type(t_cmd *cmd, int type)
{
	if (cmd && cmd->type_link == type)
		return (1);
	else
		return (0);
}

int		has_pipe(t_cmd *cmd)
{
	while (cmd && is_type(cmd, END_CMD) == 0)
	{
		if (is_type(cmd, PIPES))
			return (1);
		cmd = cmd->next;
	}
	return (0);
}

