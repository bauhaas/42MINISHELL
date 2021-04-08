/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clorin <clorin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/18 10:14:39 by clorin            #+#    #+#             */
/*   Updated: 2021/03/22 17:36:27 by bahaas           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void			del_back(t_termcaps *tc)
{
	char	*str;
	int		len;

	if (!tc->line)
		return ;
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

void			create_line(long c, t_termcaps *tc)
{
	char	car[2];
	char	*new;
	int	i;

	car[0] = c;
	car[1] = '\0';
	i = tc->cur_pos;
	if (!tc->line)
		tc->line = ft_strdup(car);
	else
	{
		new = (char*)malloc(sizeof(char) * (ft_strlen(tc->line) + 2));
		if (!new)
			return ;
		int j = 0;
		while(j < i)
		{
			new[j] = tc->line[j];
			j++;
		}
		new[j++] = car[0];
		while (tc->line[i])
			new[j++] = tc->line[i++];
		new[j] = '\0';
		ft_strdel(&tc->line);
		tc->line = ft_strdup(new);
		free(new);
	}
}

void			clear_line(t_termcaps *tc)
{
	(void)tc;
	write(1, "\033[0J", 4);
	//tputs(tc->ce, 1, tc_putchar);
}

void			print_line(t_termcaps *tc, t_ms *ms)
{
	int			start;
	int			pos_x;
	int			pos_y;

	set_cursor_position(tc, 0, tc->start_row);
	start = prompt(ms);
	if (tc->line)
		write(STDOUT, tc->line, ft_strlen(tc->line));
	get_cursor_position(&tc->col, &tc->row);
	clear_line(tc);
	pos_x = (start + tc->cur_pos) % tc->size_col;
	pos_y = ((start + tc->cur_pos) / tc->size_col) + tc->start_row;
	//set_cursor_position(tc, start + tc->cur_pos, tc->row);
	set_cursor_position(tc,pos_x, pos_y);
}
