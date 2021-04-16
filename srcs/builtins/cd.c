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

/*
** Swap the content of 2 list element
*/

void		ft_lstswap(t_list *prev, t_list *next)
{
	t_var	*tmp;

	tmp = (t_var *)prev->content;
	prev->content = next->content;
	next->content = tmp;
}

static int	set_cd(char *new_loc, t_ms *ms)
{
	char	*tmp;

	tmp = ft_strnew(2048);
	tmp = getcwd(tmp, sizeof(char) * 2048);
	if (chdir(new_loc) == -1)
	{
		ft_putstr_fd("cd: ", 2);
		ft_putstr_fd(new_loc, 2);
		ft_putstr_fd(": No such file or directory\n", 2);
		ft_strdel(&tmp);
		return (1);
	}
	ft_strdel(&ms->old_pwd);
	ms->old_pwd = ft_strdup(tmp);
	ft_strdel(&ms->pwd);
	ft_strclr(tmp);
	tmp = getcwd(tmp, sizeof(char) * 2048);
	ms->pwd = ft_strdup(tmp);
	ft_strdel(&tmp);
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
		if (!ft_getenv(&ms->env, "HOME", 1))
		{
			ft_putstr_fd("minishell: cd: HOME not set\n", 2);
			return (1);
		}
		return (set_cd(ft_getenv(&ms->env, "HOME", 1), ms));
	}
	if (ft_strcmp(cmd->content[1], "-") == 0)
		return (set_cd(ms->old_pwd, ms));
	else
		return (set_cd(cmd->content[1], ms));
}
