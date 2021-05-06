/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bahaas <bahaas@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/21 00:44:41 by bahaas            #+#    #+#             */
/*   Updated: 2021/05/05 13:28:35 by bahaas           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

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
	tmp = NULL;
	tmp = getcwd(tmp, sizeof(char) * 2048);
	if (tmp)
	{
		ft_strdel(&ms->old_pwd);
		ms->old_pwd = ft_strdup(ms->pwd);
		ft_strdel(&ms->pwd);
		ms->pwd = ft_strdup(tmp);
	}
	else
		error_getcwd(ms, new_loc);
	update_var(ms);
	ft_strdel(&tmp);
	return (0);
}

static int	cd_old_pwd(t_ms *ms, t_cmd *cmd)
{
	int		ret;

	if (ft_is_empty(ms->old_pwd))
	{
		ft_putstr_fd("minishell: cd: OLDPWD not set\n", 2);
		return (1);
	}
	ret = set_cd(ms->old_pwd, ms);
	if (ret == 0)
		ft_pwd(ms, cmd);
	return (ret);
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
		return (cd_old_pwd(ms, cmd));
	else
		return (set_cd(cmd->content[1], ms));
}
