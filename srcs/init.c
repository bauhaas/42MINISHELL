/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clorin <clorin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/17 12:52:12 by clorin            #+#    #+#             */
/*   Updated: 2021/04/01 18:50:05 by bahaas           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

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
	if (!new)
		return (NULL);
	split_var = ft_split(env_var, '=');
	if (split_var[0])
		new->name = ft_strdup(split_var[0]);
	else
		new->name = NULL;
	if (split_var[1])
		new->value=ft_strdup(split_var[1]);
	else
		new->value = NULL;
	if (ft_strchr(env_var, '='))
		new->export_display = 1;
	else
		new->export_display = 0;
	free_split(&split_var);
	return (new);
}

/*
** Create a new node & fill it with name + value of the var sent.
*/

t_list		*lstnew_var(t_var *content)
{
	t_list		*new;

	new = (t_list*)malloc(sizeof(t_list));
	if (!new)
		return (NULL);
	new->content = malloc(sizeof(t_var));
	if (!new->content)
		return (NULL);
	((t_var *)new->content)->value = ft_strdup(content->value);
	((t_var *)new->content)->name = ft_strdup(content->name);
	new->next = NULL;
	return (new);
}

/*
** Loop to create list of env variables. We init the t_var and then init
** a new element in our list
*/

void init_lstenv(t_ms *ms, t_list **lst_env, char **env)
{
	t_list	*new;
	t_var	*var;
	int		i;

	i = 0;
	while(env[i] != NULL)
	{
		var = init_envvar(env[i]);
		new = lstnew_var(var);
		ft_lstadd_back(lst_env, new);
		if(!strcmp(var->name, "PWD"))
			ms->pwd = ft_strdup(var->value);
		if(!strcmp(var->name, "OLDPWD"))
			ms->old_pwd = ft_strdup(var->value);
		free(var->name);
		free(var->value);
		free(var);
		i++;
	}
}

void	init_sep(t_ms *ms)
{
	ms->sep_set[0] = ft_strdup(";");
	ms->sep_set[1] = ft_strdup(">");
	ms->sep_set[2] = ft_strdup(">>");
	ms->sep_set[3] = ft_strdup("<");
	ms->sep_set[4] = ft_strdup("|");
	ms->sep_set[5] = NULL;
}

void	init_ms(t_ms *ms, char **env)
{
	init_lstenv(ms, &ms->env, env);
	init_bltn(ms);
	init_sep(ms);
	ms->last_ret = 0;
}
