/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cursor.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clorin <clorin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/17 14:32:50 by clorin            #+#    #+#             */
/*   Updated: 2021/03/17 14:59:40 by clorin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static int		get_cur(char buf[30], char end, int *i)
{
	int			cord;
	int			pow;

	cord = 0;
	pow = 1;
	while (buf[*i] != end)
	{
		cord = cord + (buf[*i] - '0') * pow;
		(*i)--;
		pow *= 10;
	}
	return (cord);
}

void 		get_cursor_position(int *x, int *y)
{
	char 	buf[30];
	int		i;
	char	ch;

	*y = 0;
	*x = 0;
	i = 0;
	ch = 0;
	ft_bzero(&buf, 30);
	write(1, "\033[6n", 4);
	while (ch != 'R')
	{
		read(0, &ch, 1);
		buf[i++] = ch;
	}
	if (i < 2)
		return ;
	i -=2;
	*x = get_cur(buf, ';', &i);
	i--;
	*y = get_cur(buf, '[', &i);
	return ;
}


void		set_cursor_position(t_termcaps *tc, int col, int row)
{
	tputs(tgoto(tc->cm, col, row), 1, tc_putchar);
}
