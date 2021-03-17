/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clorin <clorin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/22 13:30:36 by clorin            #+#    #+#             */
/*   Updated: 2020/09/22 13:57:18 by clorin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char		*ft_strtrim(char const *s, char const *set)
{
	size_t	min;
	size_t	max;
	char	*str;

	if (!s || !set)
		return (NULL);
	min = 0;
	while (s[min] && ft_strchr(set, s[min]))
		min++;
	max = ft_strlen(s);
	while (min < max && ft_strchr(set, s[max - 1]))
		max--;
	if (min == max)
	{
		str = (char *)malloc(sizeof(char) * 1);
		if (!str)
			return (NULL);
		str[0] = 0;
		return (str);
	}
	return (ft_substr(s, min, (max - min)));
}
