/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bahaas <bahaas@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/21 00:44:41 by bahaas            #+#    #+#             */
/*   Updated: 2021/03/22 18:35:31 by bahaas           ###   ########.fr       */
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
		ft_putstr("cd: Too many arguments\n");
		return (0);
	}
	else if(i == 1)
	{
		path = ft_getenv(&ms->env, "HOME");
		if(!path)
		{
			ft_putstr("cd: No home directory\n");
			return (0);
		}
		chdir(path);
		new_pwd = getcwd(new_pwd, 2048);
		set_location(&ms->env, ms, path, old_pwd);
		return (0);
	}
	if(chdir(cmd->content[1]) == -1)
	{
		printf("cd: No such file or direcotry: %s\n", cmd->content[1]);
		return(0);
	}
	new_pwd = getcwd(new_pwd, 2048);
	set_location(&ms->env, ms, new_pwd, old_pwd);
	return (0);
}
