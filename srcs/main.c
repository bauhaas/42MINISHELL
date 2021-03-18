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
	t_termcaps		termcaps;

	ft_bzero(&termcaps, sizeof(t_termcaps));
	printf("\n ~~~~~~~ Minishell42 ~~~~~~~\n     (Bahaas / Clorin)\n           V0.1:\n");
	init_termcaps(&termcaps);
	prompt();
	get_cursor_position(&termcaps.col, &termcaps.row);
	get_line(&termcaps);
	tcsetattr(0, TCSANOW, &termcaps.old_termcaps);
	printf("line = %s\n", termcaps.line);
	free(termcaps.line);
	return (0);
}
