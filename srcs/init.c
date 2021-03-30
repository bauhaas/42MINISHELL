/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clorin <clorin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/17 12:52:12 by clorin            #+#    #+#             */
/*   Updated: 2021/03/29 14:33:26 by bahaas           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void		free_split(char ***split)
{
	int i;

	i = -1;
	while ((*split)[++i])
	{
		free((*split)[i]);
		(*split)[i] = NULL;
	}
	free(*split);
	*split = NULL;
}

/*
** Split the env variable string on the '=' sign, and set the var to the 1st
** string of our split, the 2nd is the value. Making sure that our strings after
** split exist.
*/

t_var	*init_envvar(char *env_var)
{
	char **split_var;
	t_var *new;

	new = malloc(sizeof(t_var));
	if(!new)
		return (NULL);
	split_var = ft_split(env_var, '=');
	if(split_var[0])
		new->name=ft_strdup(split_var[0]);
	else
		new->name=NULL;
	if(split_var[1])
		new->value=ft_strdup(split_var[1]);
	else
		new->value=NULL;
	if(ft_strchr(env_var, '='))
		new->export_display = 1;
	else
		new->export_display = 0;
	free_split(&split_var);
	return(new);
}

/*
** Loop to create list of env variables. We init the t_var and then init
** a new element in our list
*/

void init_lstenv(t_ms *ms, t_list **lst_env, char **env)
{
	int i;
	t_list *new;
	t_var *var;
	t_var *cpy;
	
	i = 0;
	new = NULL;
	var = NULL;
	cpy = NULL;
	while(env[i] != NULL)
	{
		var = init_envvar(env[i]);
		cpy = malloc(sizeof(t_var));
		cpy->name = ft_strdup(var->name);
		cpy->value = ft_strdup(var->value);
		if(!ft_strcmp(var->name, "PWD"))
			ms->pwd = ft_strdup(var->value);
		if(!ft_strcmp(var->name, "OLDPWD"))
			ms->old_pwd = ft_strdup(var->value);
		new = ft_lstnew(cpy);
		ft_lstadd_back(lst_env, new);
		free(var->name);
		free(var->value);
		free(var);
		i++;
	}
}

void	init_location(t_ms *ms)
{
	t_list *tmp;
	t_var *var;

	tmp = ms->env;
	ms->pwd = NULL;
	ms->old_pwd = NULL;
	while(tmp)
	{
		var = (t_var *)tmp->content;
		if(!strcmp(var->name, "PWD"))
			ms->pwd = ft_strdup(var->value);
		if(!strcmp(var->name, "OLDPWD"))
			ms->old_pwd = ft_strdup(var->value);
		tmp = tmp->next;
	}
}

void	init_ms(t_ms *ms, char **env)
{
	init_bltn(ms);
	init_lstenv(ms, &ms->env, env);
	init_location(ms);
	//ms->i = 0;
	//ms->cmd->ret_value = 0;
}
