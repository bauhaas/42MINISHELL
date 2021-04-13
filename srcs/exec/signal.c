/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clorin <clorin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/13 10:00:01 by clorin            #+#    #+#             */
/*   Updated: 2021/04/13 10:00:16 by clorin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	sig_(int code)
{
	write(1, "^C\n" ,3);
	g_signal = code;
}

void	ctr_c(t_termcaps *tc)
{
	ft_strdel(&tc->line);
	tc->cur_pos = 0;
	g_signal = FALSE;
	get_cursor_position(&tc->start_col, &tc->start_row);
}