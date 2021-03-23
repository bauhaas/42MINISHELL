/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bahaas <bahaas@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/21 00:44:41 by bahaas            #+#    #+#             */
/*   Updated: 2021/03/23 14:42:01 by bahaas           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

char *ft_getenv(t_list **head_ref, char *elem)
{
	t_var *var;
	t_list *tmp = *head_ref;
	while(tmp)
	{
		var = (t_var *)tmp->content;
		if(!ft_strcmp(var->name, elem))
			return (var->value);
		tmp = tmp->next;
	}
	return (NULL);
}

void	set_location(t_list **head_ref, t_ms *ms, char *path, char *old_path)
{
	t_var *var;
	t_list *tmp = *head_ref;

	while(tmp)
	{
		var = (t_var *)tmp->content;
		if(!ft_strcmp(var->name, "PWD"))
			var->value = path;
		if(!ft_strcmp(var->name, "OLDPWD"))
			var->value = old_path;
		tmp = tmp->next;
	}
	ms->old_pwd = old_path;
	ms->pwd = path;
}

char *set_path(t_ms *ms, char *path)
{
	char *home;

	if (!ft_strcmp(path, "~") && ft_getenv(&ms->env, "HOME"))
	{
		home = ft_getenv(&ms->env, "HOME");
		if(!home)
		{
			printf("cd: No home directory\n");
			return (NULL);
		}
		return(ft_getenv(&ms->env, "HOME"));
	}
	else if (!ft_strcmp(path, "-"))
		return(ms->old_pwd);
	return (path);
}

int	ft_cd(t_ms *ms, t_cmd *cmd)
{
	char *old_pwd;
	char *new_pwd;
	char *path;
	int i;	

	i = 0;
	old_pwd = malloc(2048);
	new_pwd = malloc(2048);
	old_pwd = getcwd(old_pwd, 2048);
	while(cmd->content[i])
		i++;
	if(i > 2)
	{
		printf("cd: Too many arguments\n");
		return (0);
	}
	else if(i == 1)
	{
		path = ft_getenv(&ms->env, "HOME");
		if(!path)
		{
			printf("cd: No home directory\n");
			return (0);
		}
		chdir(path);
		new_pwd = getcwd(new_pwd, 2048);
		set_location(&ms->env, ms, path, old_pwd);
		return (0);
	}
	path = set_path(ms, cmd->content[1]);
	if(!path)
		return (0);
	if(chdir(path) == -1)
	{
		printf("cd: No such file or direcotry: %s\n", path);
		return(0);
	}
	new_pwd = getcwd(new_pwd, 2048);
	set_location(&ms->env, ms, new_pwd, old_pwd);
	return (0);
}
