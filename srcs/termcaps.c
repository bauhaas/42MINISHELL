/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   termcaps.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clorin <clorin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/17 15:11:21 by clorin            #+#    #+#             */
/*   Updated: 2021/03/22 22:03:05 by bahaas           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void			init_termcaps(t_termcaps *tc)
{
	char		*name;
	int			ret;

	ft_bzero(tc, sizeof(t_termcaps));
	name = getenv("TERM");
	ret = tgetent(NULL, name);
	if (ret == 1)
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
		printf("minishell: Error termcaps %d\n",ret);
		exit(-1);
	}
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

static int		boucle(t_termcaps *tc, t_ms *mini)
{
	long		c;

	c = 0;
	while (read(STDIN, &c, sizeof(c)) >= 0)
	{
		get_cursor_position(&tc->col, &tc->row);
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
	return (-1);
}

int				get_line(t_ms *mini)
{
	int status;
	t_termcaps	tc;

	init_termcaps(&tc);
	prompt(mini);
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
