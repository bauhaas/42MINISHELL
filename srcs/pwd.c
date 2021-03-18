/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bahaas <bahaas@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/18 14:15:54 by bahaas            #+#    #+#             */
/*   Updated: 2021/03/18 16:11:46 by bahaas           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void pwd(t_ms *ms, char **line)
{
	char *pwd;

	pwd = malloc(2048);
	if(line[1])
		write(1, "pwd: too many arguments\n" ,24);
	else
	{
		ms->pwd=getcwd(pwd, 2048);
		write(1, ms->pwd, ft_strlen(ms->pwd));
		write(1, "\n", 1);
	}
}

void	main_pwd(t_ms *ms)
{
	//TEST AVEC PWD
	char *test[2];
	printf("return of pwd without args = ");
	fflush(stdout);
	test[0] = "pwd";
	test[1] = NULL;
	pwd(ms, test);
	
	// TEST AVEC PWD + ARGS - OK
	char *test2[3];
	printf("return of pwd with args    = ");
	fflush(stdout);
	test2[0] = "pwd";
	test2[1] = "agre";
	test2[2] = NULL;
	pwd(ms, test2);	
}
