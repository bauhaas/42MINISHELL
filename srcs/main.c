/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bahaas <bahaas@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/17 12:52:12 by clorin            #+#    #+#             */
/*   Updated: 2021/04/06 15:09:03 by bahaas           ###   ########.fr       */
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
	ms.exit = 1;
	printf("\n ~~~~~~~ Minishell42 ~~~~~~~\n  by (Bahaas / Clorin)\n           V%.1f:\n", VERSION);
	if (argc == 1)
		{
		while (ms.exit)
		{
			ms.exit = get_line(&ms);
			if (ms.line && ms.exit)
			{
				ms.no_exec = 0;
				line_to_cmd(&ms, ms.line, ms.cmd);
				ft_strdel(&ms.line);
				free_cmd(ms.cmd);
			}
		}
		ft_strdel(&ms.line);
		free_history(&ms.history);
		printf("exit in main\n");
		return (0);
	}
	else
	{
		int		r;
		char	*line;

		line = NULL;
		int fd = open(argv[2], O_RDONLY);
		while ((r = get_next_line(fd, &line)) > 0)
		{
			printf("%s\n", line);
			ms.line = ft_strdup(line);
			line_to_cmd(&ms, ms.line, ms.cmd);
			ft_strdel(&ms.line);
			free_cmd(ms.cmd);
			free(line);
			line = NULL;
		}
		printf("%s", line);
		free(line);
		line = NULL;
	}
}
