/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bahaas <bahaas@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/17 12:52:12 by clorin            #+#    #+#             */
/*   Updated: 2021/04/01 19:54:00 by bahaas           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int					main(int argc, char **argv, char **envp)
{
	(void)argc;
	(void)argv;
	(void)envp;
	t_ms			ms;

	ft_bzero(&ms, sizeof(t_ms));
	init_ms(&ms, envp);
	ms.cmd = NULL;
	ms.i = 0;
	ms.exit = 1;
	printf("\n ~~~~~~~ Minishell42 ~~~~~~~\n  by (Bahaas / Clorin)\n           V%.1f:\n", VERSION);
	while (ms.exit)
	{
		ms.exit = get_line(&ms);
		if (ms.line && ms.exit)
		{
			line_to_cmd(&ms, ms.line, ms.cmd);
			ms.i += 1;
			ft_strdel(&ms.line);
			free_cmd(ms.cmd);
		}
	}
	ft_strdel(&ms.line);
	free_history(&ms.history);
	printf("exit in main\n");
}
