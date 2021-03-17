/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clorin <clorin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/16 12:52:46 by clorin            #+#    #+#             */
/*   Updated: 2020/09/16 12:53:00 by clorin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char		*ft_strdup(const char *str)
{
	char	*dest;
	size_t	len_str;
	size_t	i;

	i = 0;
	len_str = ft_strlen(str) + 1;
	dest = (char *)malloc(sizeof(char) * len_str);
	if (!dest)
		return (NULL);
	ft_bzero(dest, len_str);
	while (str[i])
	{
		dest[i] = str[i];
		i++;
	}
	return (dest);
}
