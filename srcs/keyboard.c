/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   keyboard.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clorin <clorin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/18 10:06:58 by clorin            #+#    #+#             */
/*   Updated: 2021/03/22 17:36:10 by bahaas           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void			window_size(t_termcaps *tc)
{
	struct		winsize window;
	ioctl(STDOUT_FILENO, TIOCGWINSZ, &window);
	tc->size_col = window.ws_col;
	tc->size_row = window.ws_row;
}

void			keys_tree(long c, t_termcaps *tc, t_ms *mini)
{
	if (ft_isprint(c))
	{
		create_line(c, tc);
		tc->cur_pos++;
	}
	else if (c == BACKSPACE && tc->cur_pos > 0)
	 	del_char(tc);
	else if (c == LEFT_ARROW && tc->cur_pos > 0)
		tc->cur_pos--;
	else if (c == RIGHT_ARROW && tc->cur_pos < (int)ft_strlen(tc->line))
		tc->cur_pos++;
	else if (c == UP_ARROW)
	 	up_history(tc, mini);
	else if (c == DOWN_ARROW)
		down_history(tc, mini);
	else if (c == HOME)
		tc->cur_pos = 0;
	else if (c == END)
		tc->cur_pos = ft_strlen(tc->line);
	else if (c == CTR_L)
	{
		write(1, "\33[H\33[2J", 7);
		//tc->start_col = 0;
		//tc->start_row = 0;
		tc->row = 0;
		tc->col = 0;
	}
	print_line(tc, mini);
}
