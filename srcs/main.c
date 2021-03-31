/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bahaas <bahaas@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/17 12:52:12 by clorin            #+#    #+#             */
/*   Updated: 2021/03/29 14:33:38 by bahaas           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int					main(int argc, char **argv, char **envp)
{
	(void)argc;
	(void)argv;
	(void)envp;
	t_ms			minishell;

	ft_bzero(&minishell, sizeof(t_ms));
	init_ms(&minishell, envp);
	minishell.i = 0;
	minishell.exit = 1;
	printf("\n ~~~~~~~ Minishell42 ~~~~~~~\n  by (Bahaas / Clorin)\n           V%.1f:\n", VERSION);
	while (minishell.exit)
	{
		minishell.exit = get_line(&minishell);
		if (minishell.line && minishell.exit)
		{
			tmp_line_to_cmd(&minishell, minishell.line);
			minishell.i += 1;
			ft_strdel(&minishell.line);
		}
	}
	ft_strdel(&minishell.line);
	free_history(&minishell.history);
	printf("exit\n");
}
