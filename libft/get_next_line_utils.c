/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clorin <clorin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/29 17:18:56 by clorin            #+#    #+#             */
/*   Updated: 2020/10/29 17:18:59 by clorin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char			*strjoin_gnl(char const *s1, char const *s2)
{
	char		*dest;
	int			size_s1;
	int			size_s2;
	int			size_total;

	if (!s1 && !s2)
		return (NULL);
	size_s1 = ft_strlen(s1);
	size_s2 = ft_strlen(s2);
	size_total = size_s1 + size_s2;
	dest = ft_strnew(size_total);
	if (!dest)
	{
		free((char *)s1);
		return (NULL);
	}
	ft_memmove(dest, s1, size_s1);
	ft_memmove(dest + size_s1, s2, size_s2);
	dest[size_total] = '\0';
	free((char *)s1);
	return (dest);
}
