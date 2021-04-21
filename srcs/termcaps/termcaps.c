/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   termcaps.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clorin <clorin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/17 15:11:21 by clorin            #+#    #+#             */
/*   Updated: 2021/04/21 15:48:04 by bahaas           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static void		init_termcaps(t_termcaps *tc)
{
	char		*name;

	name = getenv("TERM");
	if (!name)
		name = ft_strdup("xterm");
	if (tgetent(NULL, name) == 1)
	{
		tcgetattr(0, &tc->term);
		tcgetattr(0, &tc->old_termcaps);
		tc->term.c_lflag &= ~(ICANON | ECHO);
		tc->term.c_cc[VMIN] = 1;
		tc->term.c_cc[VTIME] = 0;
		tcsetattr(0, TCSANOW, &tc->term);
		tc->cm = tgetstr("cm", NULL);
		tc->ce = tgetstr("ce", NULL);
		tc->dl = tgetstr("DL", NULL);
		tc->cur_pos = 0;
		tc->line = NULL;
	}
	else
	{
		ft_putstr_fd("minishell: Error termcaps \n", STDERR);
		exit(-1);
	}
}

static int		new_line(t_termcaps *tc, t_ms *mini)
{
	if (!tc->line)
		get_cursor_position(&tc->col, &tc->row);
	else
		mini->cur_histo = add_history(&mini->history, tc->line);
	return (1);
}

static int		eof_key(t_termcaps *tc)
{
	tc->line = ft_strdup("exit");
	return (1);
}

static int		boucle(t_termcaps *tc, t_ms *mini)
{
	long		c;

	c = 0;
	while (read(STDIN, &c, sizeof(c)) >= 0)
	{
		if (g_ms->signal)
		{
			ctr_c(tc, mini);
		}
		window_size(tc);
		get_cursor_position(&tc->col, &tc->row);
		if (tc->col == (tc->size_col - 1) && c != BACKSPACE
			&& c != LEFT_ARROW && c != RIGHT_ARROW
			&& tc->row + 1 == tc->size_row)
			tc->start_row--;
		if (c == '\n')
			return (new_line(tc, mini));
		if (c == EOF_KEY && !tc->line)
			return (eof_key(tc));
		keys_tree(c, tc, mini);
		c = 0;
	}
	return (-1);
}

int				get_line(t_ms *mini)
{
	t_termcaps	tc;
	int			status;

	ft_bzero(&tc, sizeof(t_termcaps));
	init_termcaps(&tc);
	get_cursor_position(&tc.start_col, &tc.start_row);
	prompt(mini);
	get_cursor_position(&tc.col, &tc.row);
	status = boucle(&tc, mini);
	set_cursor_position(&tc, tc.col, tc.row);
	tcsetattr(0, TCSANOW, &tc.old_termcaps);
	if (status == 1)
		write(1, "\n", 1);
	ft_strdel(&mini->line);
	if (tc.line)
		mini->line = ft_strdup(tc.line);
	free_termcaps(&tc);
	return (status);
}
