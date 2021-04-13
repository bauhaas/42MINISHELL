/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   keyboard.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clorin <clorin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/18 10:06:58 by clorin            #+#    #+#             */
/*   Updated: 2021/04/12 17:54:20 by bahaas           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void			window_size(t_termcaps *tc)
{
	struct winsize	window;

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
	else if (c == BACKSPACE)
		del_back(tc);
	else if (c == EOF_KEY || c == DEL)
		del_char(tc);
	else if (c == LEFT_ARROW)
		left(tc);
	else if (c == RIGHT_ARROW)
		right(tc);
	else if (c == UP_ARROW)
		up_history(tc, mini);
	else if (c == DOWN_ARROW)
		down_history(tc, mini);
	else if (c == CTR_L)
		cls(tc);
	print_line(tc, mini);
}
