/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clorin <clorin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/23 08:24:09 by clorin            #+#    #+#             */
/*   Updated: 2020/09/23 08:32:20 by clorin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	ft_len_nbr(int n)
{
	unsigned int	size;

	size = 1;
	if (n < 0)
	{
		n = -n;
		size++;
	}
	while (n >= 10)
	{
		n = n / 10;
		size++;
	}
	return (size);
}

char		*ft_itoa(int n)
{
	char	*str;
	int		size;
	int		i;

	i = 0;
	if (n == -2147483648)
		return (ft_strdup("-2147483648"));
	size = ft_len_nbr(n);
	str = (char *)malloc(size + 1);
	if (!str)
		return (NULL);
	ft_bzero(str, size + 1);
	if (n < 0)
	{
		n = -n;
		str[i++] = '-';
	}
	while (size > i)
	{
		size--;
		str[size] = (n % 10) + '0';
		n /= 10;
	}
	return (str);
}
