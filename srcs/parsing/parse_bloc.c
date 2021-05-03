/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_bloc.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clorin <clorin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/27 09:10:50 by clorin            #+#    #+#             */
/*   Updated: 2021/04/27 09:13:10 by clorin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int					nb_semicolon(char *str)
{
	int				i;
	int				nb;

	i = 0;
	nb = 0;
	while (str[i])
	{
		if (str[i] != ';' || valid_quotes(str, i) || (str[i] == ';' &&
			escaped(str, i)))
			nb = 0;
		else
			nb++;
		i++;
	}
	ft_putstr_fd("Minishell: ", 2);
	if (nb >= 2)
		ft_putstr_fd("syntax error near unexpected token `;;'\n", 2);
	else
		ft_putstr_fd("syntax error near unexpected token `;'\n", 2);
	return (FALSE);
}

t_list				*parse_bloc(char *str)
{
	t_list			*block;
	int				i;
	char			*word;

	i = 0;
	word = NULL;
	block = NULL;
	while (str[i])
	{
		if (str[i] != ';' || valid_quotes(str, i) || (str[i] == ';' &&
			escaped(str, i)))
			word = ft_add_char(word, str[i]);
		else
		{
			ft_lstadd_back(&block, ft_lstnew(ft_strdup(word)));
			ft_strdel(&word);
		}
		i++;
	}
	if (word)
	{
		ft_lstadd_back(&block, ft_lstnew(ft_strdup(word)));
		ft_strdel(&word);
	}
	return (block);
}
