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

void		init_termcaps(t_termcaps *tc)
{
	char	*name;
	struct winsize w;
	(void)w;

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
}

int		tc_putchar(int c)
{
	write(1, &c, 1);
	return (0);
}

char		*get_line(t_termcaps *tc)
{
	int	ret;
	long	c;

	ret = 1;
	c = 0;
	tc->cur_pos = 0;
	while (ret > 0)
	{
		ret = read(STDIN, &c, sizeof(c));
		if (c == '\n')
		{
			if (!tc->line)
				get_cursor_position(&tc->col, &tc->row);
			break;
		}
		if (c == EOF_KEY && !tc->line)
			exit(0); // TO DO make free exit
		keys_tree(c, tc);
		c = 0;
	}
	set_cursor_position(tc, tc->col, tc->row);//
	write(STDOUT, "\n", 1);
	return (tc->line);
}
