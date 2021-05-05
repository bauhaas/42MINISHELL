/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd_error.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bahaas <bahaas@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/05 13:26:20 by bahaas            #+#    #+#             */
/*   Updated: 2021/05/05 13:29:29 by bahaas           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int		error_no_file(char *str)
{
	ft_putstr_fd("minishell: cd: ", 2);
	ft_putstr_fd(str, 2);
	ft_putstr_fd(": No such file or directory\n", 2);
	return (1);
}

void	error_getcwd(t_ms *ms, char *new_loc)
{
	char	*tmp;

	ft_putstr_fd("minishell: cd: error determining the current directory:", 2);
	ft_putstr_fd(" getcwd: cannot access parent directories: ", 2);
	ft_putstr_fd("No file or folder of this type\n", 2);
	ft_strdel(&ms->old_pwd);
	ms->old_pwd = ft_strdup(ms->pwd);
	tmp = ft_strnew(ft_strlen(ms->pwd) + ft_strlen(new_loc) + 1);
	if (tmp)
	{
		ft_strcpy(tmp, ms->pwd);
		ft_strcat(tmp, "/");
		ft_strcat(tmp, new_loc);
		ft_strdel(&ms->pwd);
		ms->pwd = ft_strdup(tmp);
		ft_strdel(&tmp);
	}
}
