/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clorin <clorin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/17 12:52:12 by clorin            #+#    #+#             */
/*   Updated: 2021/03/18 16:03:14 by bahaas           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

/*
** Split the env variable string on the '=' sign, and set the var to the 1st
** string of our split, the 2nd is the value. Making sure that our strings after
** split exist.
*/

t_var	*init_var(char *env_var)
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

void list_env(t_list *lst_env, char **env)
{
	int i;
	t_list *new;
	t_var *var;
	
	i = -1;
	new = NULL;
	var = NULL;
	while(env[++i])
	{
		var = init_var(env[i]);
		new = ft_lstnew(var);
		ft_lstadd_back(&lst_env, new);
	}

	// TEST DISPLAY LIST ELEM - OK
	/*
	t_var *env_var;
	env_var = NULL;
	env_var = (t_var *)lst_env->content;
	printf("var_name: %s\n", env_var->name);
	printf("var_value: %s\n", env_var->value);
	*/
}

int main(int ac, char **av, char **env)
{
	t_ms ms;
//	size_t buf_size = 1024;
//	char *buf;

	(void)ac;
	(void)av;
	ms.env = NULL;
	list_env(ms.env, env);
	//infinite loop shell
	/*
	buf = malloc(sizeof(char) * buf_size);
	if(!buf)
	{
		buf = NULL;
		return (EXIT_FAILURE);
	}
	write(1, "$>", 2);
	while(read(STDIN_FILENO, buf, buf_size) > 0)
	{
		write(1, "$>", 2);
	}
	free(buf);*/

	ms.pwd = NULL;
	//TEST ECHO
	//main_echo();
	//TEST PWD
	main_pwd(&ms);
	return (0);
}
