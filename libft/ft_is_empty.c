/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_is_empty.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clorin <clorin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/26 14:23:56 by clorin            #+#    #+#             */
/*   Updated: 2021/04/26 14:24:01 by clorin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int			ft_is_empty(char *str)
{
	int				i;

	i = 0;
	if (!str)
		return (TRUE);
	while (str[i])
	{
		if (!ft_isspace(str[i]))
			return (FALSE);
		i++;
	}
	return (TRUE);
}
