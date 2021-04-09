/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bahaas <bahaas@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/20 14:29:47 by bahaas            #+#    #+#             */
/*   Updated: 2021/04/09 14:14:37 by bahaas           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int		is_separator(char c)
{
	if (ft_strchr("|<>;&", c))
		return (1);
	return (0);
}

int		is_space(char c)
{
	if (c == ' ' || c == '\n' || c == '\t')
		return (1);
	return (0);
}

int		is_escaped(char c, char *line, int i)
{
	int res;
	if(i > 0 && i < ft_strlen(line))
	{
		printf("test line[i] = %s\n", &line[i]);
		printf("test line[i - 1] = %s\n", &line[i - 1]);
		res = line[i - 1] == '\\' && line[i] == c;
		printf("test res : %d\n", res);
		return(res);
	}
	return(0);
}
