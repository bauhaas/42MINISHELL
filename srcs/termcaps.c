/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   termcaps.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clorin <clorin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/17 15:11:21 by clorin            #+#    #+#             */
/*   Updated: 2021/03/18 10:02:43 by clorin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void			init_termcaps(t_termcaps *tc)
{
	char		*name;

	ft_bzero(tc, sizeof(t_termcaps));
	name = getenv("TERM");
	tgetent(NULL, name);
	tcgetattr(STDIN, &tc->term);
	tcgetattr(STDIN, &tc->old_termcaps);
	tc->term.c_lflag = tc->term.c_lflag & ~ICANON;
	tc->term.c_lflag = tc->term.c_lflag & ~ECHO;
	tc->term.c_cc[VMIN] = 1;
	tc->term.c_cc[VTIME] = 0;
	tcsetattr(STDIN, TCSANOW, &tc->term);
	tc->cm = tgetstr("cm", NULL);
	tc->ce = tgetstr("ce", NULL);
	tc->dl = tgetstr("dl", NULL);
	tc->cur_pos = 0;
	tc->line = NULL;
}

void			free_termcaps(t_termcaps *tc)
{
	ft_strdel(&tc->line);
}

int				tc_putchar(int c)
{
	write(1, &c, 1);
	return (0);
}

static int		boucle(t_termcaps *tc, t_mini *mini)
{
	long		c;

	c = 0;
	while (1)
	{
		read(STDIN, &c, sizeof(c));
		if (c == '\n')
		{
			if (!tc->line)
				get_cursor_position(&tc->col, &tc->row);
			else
				mini->cur_histo = add_history(&mini->history, tc->line);
			return (1);
		}
		if (c == EOF_KEY && !tc->line)
		{
			ft_strdel(&mini->line);
			mini->line = ft_strdup("exit");
			free_termcaps(tc);
			return (0); // TO DO make free exit
		}
		keys_tree(c, tc, mini);
		c = 0;
	}
}

int				get_line(t_mini *mini)
{
	int status;
	t_termcaps	tc;

	init_termcaps(&tc);
	prompt();
	get_cursor_position(&tc.col, &tc.row);
	status = boucle(&tc, mini);
	set_cursor_position(&tc, tc.col, tc.row);
	tcsetattr(0, TCSANOW, &tc.old_termcaps);
	if (status == 1)
		write (1, "\n", 1);
	ft_strdel(&mini->line);
	if (tc.line)
		mini->line = ft_strdup(tc.line);
	free_termcaps(&tc);
	return (status);
}
