/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clorin <clorin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/17 12:52:12 by clorin            #+#    #+#             */
/*   Updated: 2021/03/22 15:50:58 by bahaas           ###   ########.fr       */
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
	if(!new)
		return (NULL);
	split_var = ft_split(env_var, '=');
	if(split_var[0])
		new->name=split_var[0];
	else
		new->name=NULL;
	if(split_var[1])
		new->value=split_var[1];
	else
		new->value=NULL;
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
	
	i = -1;
	new = NULL;
	var = NULL;
	cpy = NULL;
	while(env[++i])
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
	}
}

void	init_ms(t_ms *ms, char **env)
{
	ms->env = NULL;
	ms->pwd = NULL;
	ms->bltn = NULL;
	ms->bltn = malloc(sizeof(t_bltn));
	init_bltn(ms);
	init_lstenv(ms, &ms->env, env);
}

/// OLD DEV2 MAIN 
/*
int main(int ac, char **av, char **env)
{
	t_ms ms;

	(void)ac;
	(void)av;
	init_ms(&ms, env);

	//1ST IDEA LEXING & TOKENISATION
	
	char line_test[25] = "echo -n \"Hello    World\"";
	char **line_tokens;
	printf("%s\n", line_test);
	line_tokens = get_tokens(line_test);
	int i = 0;
	while(line_tokens[i])
	{
		printf("token %d : %s\n", i, line_tokens[i]);
		++i;
	}
	char line[2];
	line[0] = 'a';
	line[1] = 0;
	test_builtin(&ms, line);
	return (0);
}*/
