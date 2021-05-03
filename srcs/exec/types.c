/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   types.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bahaas <bahaas@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/03 15:31:08 by bahaas            #+#    #+#             */
/*   Updated: 2021/05/03 15:37:57 by bahaas           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int		is_redir(t_cmd *cmd)
{
	if (cmd)
	{
		if (cmd->type_link == 6 || cmd->type_link == 7 || cmd->type_link == 8)
			return (1);
	}
	return (0);
}

int		is_pipe(t_cmd *cmd)
{
	if (cmd->type_link == 4)
		return (1);
	return (0);
}

int		is_type(t_cmd *cmd, int type)
{
	if (cmd && cmd->type_link == type)
		return (1);
	else
		return (0);
}
