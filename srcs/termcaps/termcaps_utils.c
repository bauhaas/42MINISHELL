/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   termcaps_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clorin <clorin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/13 09:31:11 by clorin            #+#    #+#             */
/*   Updated: 2021/04/21 15:48:34 by bahaas           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	clear_line(void)
{
	write(1, "\033[0J", 4);
}

void	cls(t_termcaps *tc)
{
	write(1, "\33[H\33[2J", 7);
	tc->start_col = 0;
	tc->start_row = 0;
	tc->row = 0;
	tc->col = 0;
}

void	free_termcaps(t_termcaps *tc)
{
	ft_strdel(&tc->line);
}

int		tc_putchar(int c)
{
	write(1, &c, 1);
	return (0);
}
