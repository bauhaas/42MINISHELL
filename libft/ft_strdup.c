/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clorin <clorin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/16 12:52:46 by clorin            #+#    #+#             */
/*   Updated: 2021/03/24 12:22:59 by bahaas           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char		*ft_strdup(const char *str)
{
	char	*dest;
	size_t	len_str;
	size_t	i;

	i = 0;
	if (!str)
		len_str = 1;
	else
		len_str = ft_strlen(str) + 1;
	dest = (char *)malloc(sizeof(char) * len_str);
	if (!dest)
		return (NULL);
	ft_bzero(dest, len_str);
	if (str)
	{
		while (str[i])
		{
			dest[i] = str[i];
			i++;
		}
	}
	return (dest);
}
