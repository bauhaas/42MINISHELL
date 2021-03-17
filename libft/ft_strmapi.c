/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strmapi.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clorin <clorin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/21 10:05:07 by clorin            #+#    #+#             */
/*   Updated: 2020/09/21 10:09:15 by clorin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char		*ft_strmapi(char const *s, char (*f)(unsigned int, char))
{
	unsigned int	i;
	char			*new;
	size_t			len;

	if (!s)
		return (NULL);
	len = ft_strlen(s) + 1;
	new = (char *)malloc(sizeof(char) * ft_strlen(s) + 1);
	if (!new)
		return (NULL);
	ft_bzero(new, len);
	i = 0;
	if (f)
	{
		while (s[i])
		{
			new[i] = (*f)(i, s[i]);
			i++;
		}
	}
	return (new);
}
