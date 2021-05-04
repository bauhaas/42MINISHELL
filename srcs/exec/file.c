/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   file.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bahaas <bahaas@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/06 15:30:38 by bahaas            #+#    #+#             */
/*   Updated: 2021/05/03 14:15:07 by bahaas           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/*
**	update cmd-> ret value.
**	if not exists => 1
**	if not executable => 2
**	if directory => 3
*/

void				valid_file(t_cmd *cmd, int i)
{
	struct stat		buffer;
	int				exist;

	exist = (stat(cmd->content[0], &buffer));
	if (exist != 0)
		cmd->ret_value = 1;
	else if (i == 0 && (buffer.st_mode & S_IXUSR) == 0)
		cmd->ret_value = 2;
	else if (S_ISDIR(buffer.st_mode) != 0)
		cmd->ret_value = 3;
	else
		cmd->ret_value = 0;
}

/*
**	This function returns TRUE if *file exists
*/

static int			file_exist(const char *file)
{
	struct stat		buffer;

	return (stat(file, &buffer) == 0);
}

/*
**	show errors when looking for executable
**	and update minishell-> exit
*/

void				error_file(t_ms *ms, t_cmd *cmd)
{
	ft_putstr_fd("minishell: ", STDERR);
	ft_putstr_fd(cmd->content[0], STDERR);
	if (cmd->ret_value == 1)
	{
		ft_putstr_fd(": No such file or directory\n", STDERR);
		ms->last_ret = 127;
	}
	else if (cmd->ret_value == 2)
	{
		ft_putstr_fd(": Permission denied\n", STDERR);
		ms->last_ret = 126;
	}
	else if (cmd->ret_value == 3)
	{
		ft_putstr_fd(": is a directory\n", STDERR);
		ms->last_ret = 126;
	}
	else if (cmd->ret_value == 4)
	{
		ft_putstr_fd(": Command not found\n", STDERR);
		ms->last_ret = 127;
	}
}

/*
**	search in the PATH and update cmd->content[0]
*/

static void			find_absolute_path(t_ms *ms, t_cmd *cmd, int i)
{
	char			**path_to_check;
	char			*path_env;
	char			*program;

	path_env = ft_getenv(&ms->env, "PATH", 1);
	path_to_check = ft_split(path_env, ':');
	if (!path_to_check)
	{
		cmd->ret_value = 1;
		return ;
	}
	while (path_to_check[i])
	{
		program = ft_strdup(path_to_check[i]);
		program = ft_strjoin(path_to_check[i], "/");
		program = ft_strjoin(program, cmd->content[0]);
		if (file_exist(program))
		{
			ft_strdel(&cmd->content[0]);
			cmd->content[0] = program;
			return ;
		}
		i++;
	}
	cmd->ret_value = 4;
}

/*
**	search and validate the order
*/

int					search_prog(t_ms *ms, t_cmd *cmd)
{
	if (cmd->content[0][0] != '.' && cmd->content[0][0] != '/')
		find_absolute_path(ms, cmd, 0);
	else
		valid_file(cmd, 0);
	if (cmd->ret_value)
	{
		if (!ft_strcmp(cmd->content[0], ".."))
			cmd->ret_value = 4;
		error_file(ms, cmd);
		return (cmd->ret_value);
	}
	return (TRUE);
}
