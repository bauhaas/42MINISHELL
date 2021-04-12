/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cursor.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clorin <clorin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/17 14:32:50 by clorin            #+#    #+#             */
/*   Updated: 2021/04/12 17:50:39 by bahaas           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void		get_cursor_position(int *col, int *rows)
{
	int		a;
	int		i;
	char	buf[255];
	int		ret;

	a = 0;
	i = 1;
	*col = 0;
	*rows = 0;
	write(0, "\033[6n", 4);
	ret = read(0, buf, 254);
	buf[ret] = '\0';
	while (i < 254 && buf[i])
	{
		if (buf[i] >= 48 && buf[i] <= 57)
		{
			if (a == 0)
				*rows = ft_atoi(&buf[i]) - 1;
			else
				*col = ft_atoi(&buf[i]) - 1;
			a++;
			i += ft_intlen(*col) - 1;
		}
		i++;
	}
}

void		set_cursor_position(t_termcaps *tc, int col, int row)
{
	tputs(tgoto(tc->cm, col, row), 1, tc_putchar);
}

void		beep(void)
{
	write(1, "\a", 1);
}

void		left(t_termcaps *tc)
{
	if (tc->cur_pos > 0)
		tc->cur_pos--;
	else
		beep();
}

void		right(t_termcaps *tc)
{
	if (tc->cur_pos < (int)ft_strlen(tc->line))
		tc->cur_pos++;
	else
		beep();
}
