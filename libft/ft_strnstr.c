/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnstr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clorin <clorin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/18 10:49:52 by clorin            #+#    #+#             */
/*   Updated: 2020/09/18 10:54:22 by clorin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char		*ft_strnstr(const char *str, const char *substr, size_t len)
{
	size_t		len_substr;

	if (!*substr)
		return ((char *)str);
	len_substr = ft_strlen(substr);
	while (*str && len >= len_substr)
	{
		if (*str == *substr && ft_memcmp(str, substr, len_substr) == 0)
			return ((char *)str);
		str++;
		len--;
	}
	return (NULL);
}
