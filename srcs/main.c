/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bahaas <bahaas@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/17 12:52:12 by clorin            #+#    #+#             */
/*   Updated: 2021/04/20 15:35:46 by bahaas           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static void			free_list(void *list)
{
	 t_list			*e;

	e = (t_list*)list;
	free(e->content);
	free(e);
}

int					prompt_loop(t_ms *ms)
{
	t_list			*bloc;
	t_list			*head_bloc;

	bloc = NULL;
	head_bloc = bloc;
	while (1)
	{
		ms->exit = get_line(ms);
		if (ms->line && ms->exit)
		{
			if (!valid_quotes(ms->line, ft_strlen(ms->line)))
			{
				bloc = parse_bloc(ms->line);
				while(bloc)
				{
					ms->echo = FALSE;
					line_to_cmd(ms, (char*)bloc->content, ms->cmd);
					bloc = bloc->next;
				}
				ft_lstclear(&head_bloc, &free_list);
			}
			else
			{
				ft_putstr_fd("minishell: syntax error with open quotes\n", 2);
				ms->last_ret = 2;
			}
			ft_strdel(&ms->line);
			free_cmd(ms->cmd);
		}
	}
	return (0);
}

int					main(int argc, char **argv, char **envp)
{
	t_ms			ms;
	(void)argc;
	(void)argv;

	ft_bzero(&ms, sizeof(t_ms));
	init_ms(&ms, envp);
	signal(SIGINT, &sig_int);
	signal(SIGQUIT, &sig_quit);
	g_ms = &ms;
	if (argc == 1)
		prompt_loop(&ms);
	else
	{
		int		r;
		char	*line;
		int		fd;

		line = NULL;
		fd = open(argv[2], O_RDONLY);
		while ((r = get_next_line(fd, &line)) > 0)
		{
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
