/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   history.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clorin <clorin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/19 11:01:39 by clorin            #+#    #+#             */
/*   Updated: 2021/03/19 11:10:21 by clorin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

t_hist			*add_history(t_hist **begin, char *line)
{
	t_hist		*new;

	new = NULL;
	if (line)
	{
		new = ft_calloc(1, sizeof(t_hist));
		if (new)
		{
			new->line = ft_strdup(line);
			if (*begin)
			{
				new->next = *begin;
				(*begin)->prev = new;
				*begin = new;
			}
			else
				*begin = new;
		}
	}
	return (new);
}

void			up_history(t_termcaps *tc, t_mini *mini)
{
	if (!mini->history)
		return ;
	if (!tc->line && mini->cur_histo)
	{
		ft_strdel(&tc->line);
		tc->line = ft_strdup(mini->cur_histo->line);
	}
	 else if (mini->cur_histo->next)
	{
		mini->cur_histo = mini->cur_histo->next;
		ft_strdel(&tc->line);
		tc->line = ft_strdup(mini->cur_histo->line);	
	}
	tc->cur_pos = ft_strlen(tc->line);
}

void			down_history(t_termcaps *tc, t_mini *mini)
{
	if (!mini->history)
		return ;
	if (mini->cur_histo->prev)
	{
		mini->cur_histo = mini->cur_histo->prev;
		ft_strdel(&tc->line);
		tc->line = ft_strdup(mini->cur_histo->line);
	}
	else
		ft_strdel(&tc->line);
	tc->cur_pos = ft_strlen(tc->line);
}

void			free_history(t_hist **begin)
{
	t_hist		*tmp;
	t_hist		*tmp2;

	tmp = *begin;
	while (tmp)
	{
		ft_strdel(&tmp->line);
		tmp2 = tmp->next;
		free(tmp);
		tmp = tmp2;
	}
}
