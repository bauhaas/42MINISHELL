/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strrchr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clorin <clorin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/18 10:29:36 by clorin            #+#    #+#             */
/*   Updated: 2020/09/18 10:30:25 by clorin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char		*ft_strrchr(const char *str, int c)
{
	char		*s;
	int			i;

	s = (char *)str;
	i = ft_strlen(str);
	while (*s)
		s++;
	while (i >= 0)
	{
		if (*s == (char)c)
			return (s);
		s--;
		i--;
	}
	return (NULL);
}
