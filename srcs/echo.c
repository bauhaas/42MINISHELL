/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bahaas <bahaas@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/18 14:15:43 by bahaas            #+#    #+#             */
/*   Updated: 2021/03/18 15:57:21 by bahaas           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	echo(char **line)
{
	int no_newline;
	int i;

	no_newline = 0;
	i = 1;
	while(line[i] && !ft_strcmp(line[i], "-n")) //loop here with cmp cuz "echo -n -n -n" return nothing
	{
		no_newline = 1;
		i++;
	}
	while(line[i])
	{
		write(1, line[i], ft_strlen(line[i]));
		if(line[i + 1])
			write(1, " ", 1);
		i++;
	}
	if(!no_newline)
		write(1, "\n", 1);
}

void	main_echo()
{
	//basic test : gcc echo.c ../libft/libft.a && ./a.out

		
	//TEST AVEC -N OK
	char *test[6];
	test[0] = "echo";
	test[1] = "-n";
	test[2] = "bonjour";
	test[3] = "toi";
	test[4] = "!";
	test[5] = NULL;
	echo(test);
	
	/*
	// TEST SANS -N OK
	char *test2[6];
	test2[0] = "echo";
	test2[1] = "Ah,";
	test2[2] = "bonjour";
	test2[3] = "toi";
	test2[4] = "!";
	test2[5] = NULL;
	echo(test2);
	*/
	/*	
	// TEST SANS ARGS OK
	char *test4[2];
	test4[0] = "echo";
	test4[1] = NULL;
	echo(test4);
	*/
	/*
	// TEST SANS ARGS AVEC -N OK
	char *test3[3];
	test3[0] = "echo";
	test3[1] = "-n";
	test3[2] = NULL;
	echo(test3);
	*/
}
