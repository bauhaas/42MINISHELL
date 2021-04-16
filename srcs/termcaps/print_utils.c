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

void			del_back(t_termcaps *tc)
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

void			del_char(t_termcaps *tc)
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
