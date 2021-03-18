/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   keyboard.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clorin <clorin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/18 10:06:58 by clorin            #+#    #+#             */
/*   Updated: 2021/03/18 10:12:49 by clorin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void			keys_tree(long c, t_termcaps *tc)
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
	// if (tc->line)
	// 	tc->len_line = ft_strlen(tc->line);
	print_line(tc);
}
