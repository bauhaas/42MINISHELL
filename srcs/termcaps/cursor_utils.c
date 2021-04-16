/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cursor_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clorin <clorin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/16 14:40:42 by clorin            #+#    #+#             */
/*   Updated: 2021/04/16 14:40:58 by clorin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	left(t_termcaps *tc)
{
	if (tc->cur_pos > 0)
		tc->cur_pos--;
	else
		beep();
}

void	right(t_termcaps *tc)
{
	if (tc->cur_pos < (int)ft_strlen(tc->line))
		tc->cur_pos++;
	else
		beep();
}

void	left_word(t_termcaps *tc)
{
	int	word;

	if (tc->cur_pos == 0)
		return ;
	word = 0;
	if (!tc->line)
		return ;
	tc->cur_pos--;
	while (tc->cur_pos > 0)
	{
		if (!word && tc->line[tc->cur_pos] != ' ')
			word = 1;
		if (word && tc->line[tc->cur_pos - 1] == ' ')
			break ;
		left(tc);
	}
}

void	right_word(t_termcaps *tc)
{
	int	len;
	int	word;

	word = 0;
	if (!tc->line)
		return ;
	len = ft_strlen(tc->line);
	while (tc->cur_pos < len)
	{
		if (!word && tc->line[tc->cur_pos] != ' ')
			word = 1;
		if (word && tc->line[tc->cur_pos] == ' ')
			break ;
		right(tc);
	}
}
