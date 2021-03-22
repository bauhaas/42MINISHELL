/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clorin <clorin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/18 10:14:39 by clorin            #+#    #+#             */
/*   Updated: 2021/03/18 10:58:04 by clorin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void			del_char(t_termcaps *tc)
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
	write(1, "\033[0J", 4);
	//tputs(tc->ce, 1, tc_putchar);
}

void			print_line(t_termcaps *tc)
{
	set_cursor_position(tc, 0, tc->row);
	clear_line(tc);
	prompt();
	int start = ft_strlen(getenv("PWD")) + 3;
	if (tc->line)
		write(STDOUT, tc->line, ft_strlen(tc->line));
	get_cursor_position(&tc->col, &tc->row);
	clear_line(tc);
	set_cursor_position(tc, start + tc->cur_pos, tc->row);
}
