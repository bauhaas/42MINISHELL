/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   file_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bahaas <bahaas@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/06 13:05:56 by bahaas            #+#    #+#             */
/*   Updated: 2021/05/06 13:06:34 by bahaas           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

char			*construc_path(char *path, char *cmd)
{
	char			*program;

	program = NULL;
	program = ft_strnew(ft_strlen(path) + 1 + ft_strlen(cmd));
	if (program)
	{
		ft_strcat(program, path);
		ft_strcat(program, "/");
		ft_strcat(program, cmd);
	}
	return (program);
}
