/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   keyboard.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clorin <clorin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/18 10:06:58 by clorin            #+#    #+#             */
/*   Updated: 2021/05/06 14:39:54 by bahaas           ###   ########.fr       */
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

static void		del_back(t_termcaps *tc)
{
	char		*str;
	int			len;

	if (!tc->line || tc->cur_pos == 0)
	{
		beep();
		return ;
	}
	len = ft_strlen(tc->line);
	if (len == 1 && tc->cur_pos == 1)
	{
		ft_strdel(&tc->line);
		tc->cur_pos--;
		return ;
	}
	if (!(str = (char*)ft_calloc(1, sizeof(char) * (len))))
		return ;
	ft_strlcpy(str, tc->line, tc->cur_pos);
	ft_strncat(str, tc->line + tc->cur_pos, len - tc->cur_pos);
	ft_strdel(&tc->line);
	tc->line = str;
	tc->cur_pos--;
}

static void		del_char(t_termcaps *tc)
{
	char		*str;
	int			len;

	if (!tc->line || tc->cur_pos == (int)ft_strlen(tc->line))
	{
		beep();
		return ;
	}
	len = ft_strlen(tc->line);
	if (len == 1)
	{
		ft_strdel(&tc->line);
		return ;
	}
	if (!(str = (char*)ft_calloc(1, sizeof(char) * (len))))
		return ;
	ft_strlcpy(str, tc->line, tc->cur_pos + 1);
	ft_strncat(str, tc->line + tc->cur_pos + 1, len - tc->cur_pos);
	ft_strdel(&tc->line);
	tc->line = str;
}

void			keys_tree(long c, t_termcaps *tc, t_ms *ms)
{
	if (ft_isprint(c))
		create_line(c, tc);
	else if (c == BACKSPACE)
		del_back(tc);
	else if (c == EOF_KEY || c == DEL)
		del_char(tc);
	else if (c == LEFT_ARROW)
		left(tc);
	else if (c == RIGHT_ARROW)
		right(tc);
	else if (c == UP_ARROW)
		up_history(tc, ms);
	else if (c == DOWN_ARROW)
		down_history(tc, ms);
	else if (c == CTR_L)
		cls(tc);
	else if (c == HOME)
		tc->cur_pos = 0;
	else if (c == END)
		tc->cur_pos = ft_strlen(tc->line);
	else if (c == CTR_LEFT)
		left_word(tc);
	else if (c == CTR_RIGHT)
		right_word(tc);
	print_line(tc, ms);
}
