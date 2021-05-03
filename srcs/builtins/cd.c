/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bahaas <bahaas@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/21 00:44:41 by bahaas            #+#    #+#             */
/*   Updated: 2021/04/16 13:42:13 by clorin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"
#include <errno.h>

static int	error_no_file(char *str)
{
	ft_putstr_fd("cd: ", 2);
	ft_putstr_fd(str, 2);
	ft_putstr_fd(": No such file or directory\n", 2);
	return (1);
}

static void	error_getcwd(t_ms *ms, char *new_loc)
{
	char	*tmp;

	ft_putstr_fd("Minishell: cd: error determining the current directory:", 2);
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

static void	update_var(t_ms *ms)
{
	t_var	*new;

	new = ft_get_t_var(&ms->env, "PWD");
	if (new)
	{
		ft_strdel(&new->value);
		new->value = ft_strdup(ms->pwd);
	}
	new = ft_get_t_var(&ms->env, "OLDPWD");
	if (new)
	{
		ft_strdel(&new->value);
		new->value = ft_strdup(ms->old_pwd);
	}
}

static int	set_cd(char *new_loc, t_ms *ms)
{
	char	*tmp;

	if (chdir(new_loc) == -1)
		return (error_no_file(new_loc));
	tmp = ft_strnew(2048);
	tmp = getcwd(tmp, sizeof(char) * 2048);
	if (tmp)
	{
		ft_strdel(&ms->old_pwd);
		ms->old_pwd = ft_strdup(ms->pwd);
		ft_strdel(&ms->pwd);
		ms->pwd = ft_strdup(tmp);
		ft_strdel(&tmp);
	}
	else
		error_getcwd(ms, new_loc);
	update_var(ms);
	return (0);
}

int			ft_cd(t_ms *ms, t_cmd *cmd)
{
	if (cmd->content[1] && cmd->content[2])
	{
		ft_putstr_fd("cd: Too many arguments\n", 2);
		return (1);
	}
	if (!cmd->content[1] || ft_strcmp(cmd->content[1], "~") == 0)
	{
		if (!ft_getenv(&ms->env, "HOME", 0))
		{
			ft_putstr_fd("minishell: cd: HOME not set\n", 2);
			return (1);
		}
		if (ft_getenv(&ms->env, "HOME", 1) &&
			ft_strcmp(ft_getenv(&ms->env, "HOME", 1), "\0") == 0)
			return (0);
		return (set_cd(ft_getenv(&ms->env, "HOME", 1), ms));
	}
	if (ft_strcmp(cmd->content[1], "-") == 0)
	{
		ft_pwd(ms, cmd);
		return (set_cd(ms->old_pwd, ms));
	}
	else
		return (set_cd(cmd->content[1], ms));
}
