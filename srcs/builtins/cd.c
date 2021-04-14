/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bahaas <bahaas@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/21 00:44:41 by bahaas            #+#    #+#             */
/*   Updated: 2021/04/12 17:41:39 by bahaas           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

/*
** Swap the content of 2 list element
*/

void	ft_lstswap(t_list *prev, t_list *next)
{
	t_var *tmp;

	tmp = (t_var *)prev->content;
	prev->content = next->content;
	next->content = tmp;
}

/*
** using cd, recreate the PWD or/and OLDPWD env var if they have been unset
** before. We had them one position before the end of our list.
*/

void	reset_in_lst(t_ms *ms, char *name)
{
	t_list	*new;
	t_list	*last;
	t_var	*var;

	var = malloc(sizeof(t_var));
	var->name = ft_strdup(name);
	if (!ft_strcmp(name, "PWD"))
		var->value = ms->pwd;
	else
		var->value = ms->old_pwd;
	last = ft_lstlast(ms->env);
	new = ft_lstnew(var);
	ft_lstadd_back(&ms->env, new);
	ft_lstswap(last, new);
}

/*
** Update the pwd in t_location and in the env list if needed.
*/

void	set_location(t_ms *ms, char *path, char *old_path)
{
	int		found_oldpwd_env;
	int		found_pwd_env;
	t_var	*var;
	t_list	*tmp;

	tmp = ms->env;
	found_pwd_env = 0;
	found_oldpwd_env = 0;
	while (tmp)
	{
		var = (t_var *)tmp->content;
		if (!ft_strcmp(var->name, "PWD"))
		{
			var->value = path;
			found_pwd_env = 1;
		}
		if (!ft_strcmp(var->name, "OLDPWD"))
		{
			var->value = old_path;
			found_oldpwd_env = 1;
		}
		tmp = tmp->next;
	}
	ms->old_pwd = old_path;
	ms->pwd = path;
	if (found_pwd_env == 0)
		reset_in_lst(ms, "PWD");
	if (found_oldpwd_env == 0)
		reset_in_lst(ms, "OLDPWD");
}

static char	*cd_home(t_ms *ms, int tilde)
{
	char	*home;

	home = ft_getenv(&ms->env, "HOME", 1);
	if (!home)
	{
		//if (tilde)
			ft_putstr_fd("cd: No home directory\n", 2);
		return (NULL);
	}
	return (home);
}

static char	*select_location(t_ms *ms, char *path)
{
	if (!ft_strcmp(path, "~"))
		return (cd_home(ms, 1));
	else if (!ft_strcmp(path, "-"))
		return (ms->old_pwd);
	return (path);
}

int		ft_cd(t_ms *ms, t_cmd *cmd)
{
	char	*old_pwd;
	char	*new_pwd;
	int		i;

	i = 0;
	old_pwd = malloc(2048);
	old_pwd = getcwd(old_pwd, sizeof(char) * 2048);
	new_pwd = malloc(2048);
	while (cmd->content[i])
		i++;
	if (i > 2)
	{
		ft_putstr_fd("cd: Too many arguments\n", 2);
		return (1);
	}
	else if (i == 1)
	{
		new_pwd = cd_home(ms, 0);
		if (!new_pwd)
			return (1);
		chdir(new_pwd);
		new_pwd = getcwd(new_pwd, sizeof(char) * 2048);
		set_location(ms, new_pwd, old_pwd);
		return (0);
	}
	new_pwd = select_location(ms, cmd->content[1]);
	if (!new_pwd)
		return (1);
	if (chdir(new_pwd) == -1)
	{
		ft_putstr_fd("cd: ", 2);
		ft_putstr_fd(new_pwd, 2);
		ft_putstr_fd(": No such file or directory\n", 2);
		return (1);
	}
	new_pwd = getcwd(new_pwd, sizeof(char) * 2048);
	set_location(ms, new_pwd, old_pwd);
	return (0);
}
