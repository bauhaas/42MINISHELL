/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bahaas <bahaas@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/21 00:44:41 by bahaas            #+#    #+#             */
/*   Updated: 2021/03/21 13:57:57 by bahaas           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

char *ft_getenv(t_list *env, char *elem)
{
	t_var *var;

	while(env)
	{
		var = (t_var *)env->content;
		//printf("var name: %s\n", var->name);
		if(!ft_strcmp(var->name, elem))
			return (var->value);
		env = env->next;
	}
	return (NULL);
}

int	ft_cd(t_ms *ms, t_cmd *cmd)
{
	char *path;
	int i;	
	
	i = 0;
	printf("test\n");
	while(cmd->content[i])
		i++;
	if(i >= 2)
	{
		ft_putstr("cd: Too many arguments");
		return (0);
	}
	else if(i == 1)
	{
		path = ft_getenv(ms->env, "HOME");
		printf("test HOME: %s\n", path);
		if(!path)
		{
			ft_putstr("cd: No home directory");
			return (0);
		}
		chdir(path);
	}
	if(chdir(cmd->content[1]) == -1)
		return(0);
	return (0);
}
