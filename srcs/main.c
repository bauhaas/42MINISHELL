/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clorin <clorin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/17 12:52:12 by clorin            #+#    #+#             */
/*   Updated: 2021/03/17 15:42:54 by clorin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int					main(int argc, char **argv, char **envp)
{
	(void)argc;
	(void)argv;
	(void)envp;
	t_mini			minishell;

	ft_bzero(&minishell, sizeof(t_mini));
	minishell.exit = 1;
	printf("\n ~~~~~~~ Minishell42 ~~~~~~~\n  by (Bahaas / Clorin)\n           V0.1:\n");
	while (minishell.exit)
	{
		minishell.exit = get_line(&minishell);
		if (minishell.line && minishell.exit)
		{
			printf("(%d)line = %s\n", minishell.exit, minishell.line);
			ft_strdel(&minishell.line);
		}
	}
	ft_strdel(&minishell.line);
	free_history(&minishell.history);
	printf("exit\n");
	return (0);
}
