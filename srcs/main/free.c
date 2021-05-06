/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bahaas <bahaas@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/06 03:43:42 by bahaas            #+#    #+#             */
/*   Updated: 2021/05/06 03:48:18 by bahaas           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void			free_list(void *content)
{
	char	*e;

	e = (char *)content;
	ft_strdel(&e);
}

void			free_cmd(t_cmd *cmd)
{
	t_cmd		*ptr_list;
	t_cmd		*ptr_next;
	int			i;

	ptr_list = cmd;
	while (ptr_list)
	{
		i = 0;
		ptr_next = ptr_list->next;
		while (ptr_list->content[i])
		{
			ft_strdel(&ptr_list->content[i]);
			i++;
		}
		free(ptr_list->content);
		free(ptr_list);
		ptr_list = ptr_next;
	}
	cmd = NULL;
}

void			free_arrstr(char **arr_env)
{
	int			i;

	i = -1;
	while (arr_env[++i])
	{
		free(arr_env[i]);
		arr_env[i] = NULL;
	}
	free(arr_env);
	arr_env = NULL;
}

void			free_split(char ***split)
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
