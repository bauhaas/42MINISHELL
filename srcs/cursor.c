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

void		get_cursor_position(int *col, int *rows)
{
	int	ret;
	char	buf[255];
	int	i;
	int	a;

	i = 1;
	a = 0;
	write(STDIN, "\033[6n", 4);
	ret = read(0, buf, 254);
	buf[ret] = '\0';
	while (buf[i])
	{
		if (buf[i] >= '0' && buf[i] <= '9')
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
