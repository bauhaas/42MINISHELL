/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcpy.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clorin <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/08/07 15:05:53 by clorin            #+#    #+#             */
/*   Updated: 2020/08/07 15:06:54 by clorin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t			ft_strlcpy(char *dest, const char *src, size_t size)
{
	size_t		i;
	int			j;

	i = 0;
	j = 0;
	if (!dest || !src)
		return (0);
	if (size > 0)
	{
		while (src[i])
		{
			if (size > 0 && i < (size - 1))
			{
				dest[j] = src[j];
				j++;
			}
			i++;
		}
		dest[j] = '\0';
	}
	return (ft_strlen(src));
}
