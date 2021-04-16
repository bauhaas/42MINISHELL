/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clorin <clorin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/18 10:14:39 by clorin            #+#    #+#             */
/*   Updated: 2021/04/12 17:52:18 by bahaas           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static char		*add_car(char *str, char c, int pos)
{
	char		*new;
	int			j;

	new = ft_strnew(ft_strlen(str) + 1);
	if (!new)
		return (NULL);
	j = 0;
	while (j < pos)
	{
		new[j] = str[j];
		j++;
	}
	new[j++] = c;
	while (str[pos])
		new[j++] = str[pos++];
	return (new);
}

void			create_line(long c, t_termcaps *tc)
{
	char		car[2];
	char		*new;

	car[0] = c;
	car[1] = '\0';
	new = NULL;
	if (!tc->line)
		tc->line = ft_strdup(car);
	else
	{
		new = add_car(tc->line, (char)c, tc->cur_pos);
		ft_strdel(&tc->line);
		tc->line = ft_strdup(new);
		ft_strdel(&new);
	}
	tc->cur_pos++;
}

void			print_line(t_termcaps *tc, t_ms *ms)
{
	int			start;
	int			pos_x;
	int			pos_y;

	set_cursor_position(tc, tc->start_col, tc->start_row);
	start = prompt(ms) + tc->start_col;
	if (tc->line)
		write(STDOUT, tc->line, ft_strlen(tc->line));
	clear_line(tc);
	pos_x = (start + tc->cur_pos) % tc->size_col;
	pos_y = ((start + tc->cur_pos) / tc->size_col) + tc->start_row;
	set_cursor_position(tc, pos_x, pos_y);
}
