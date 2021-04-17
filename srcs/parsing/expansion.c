/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clorin <clorin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/24 13:13:22 by clorin            #+#    #+#             */
/*   Updated: 2021/04/12 17:16:03 by bahaas           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

/*
**	this function returns the name's var after $ or NULL if nothing
*/

char				*get_last_ret(t_ms *mini, int *i)
{
	char			*dest;

	dest = ft_strnew(ft_strlen(ft_itoa(mini->last_ret)));
	ft_strcat(dest, ft_itoa(mini->last_ret));
	*i += 2;
	return (dest);
}

char				*get_null_value(t_ms *mini, int len, int *i)
{
	char			*dest;

	dest = ft_strnew(1);
	*i += len + 1;
	return (dest);
}

char				*get_good_value(t_ms *mini, char *name, int len, int *i)
{
	char			*dest;

	dest = ft_strnew(ft_strlen(ft_getenv(&mini->env, name, TRUE)));
	ft_strcat(dest, ft_getenv(&mini->env, name, TRUE));
	*i += len + 1;
	return (dest);
}

char				*get_one_dollar(int *i)
{
	char			*dest;

	dest = ft_strnew(1);
	dest[0] = '$';
	*i += 1;
	return (dest);
}

char				*value(t_ms *mini, char *str, int *i)
{
	char			*dest;
	char			*name;
	int				len;

	dest = NULL;
	name = get_name_var(str);
	len = ft_strlen(name);
	if (*str == '$' && *(str + 1) == '?')
		dest = get_last_ret(mini, i);
	else if (name && ft_getenv(&mini->env, name, TRUE))
		dest = get_good_value(mini, name, len, i);
	else if (name && !ft_getenv(&mini->env, name, TRUE))
		dest = get_null_value(mini, len, i);
	else if (*str != '$' || (*str == '$' && (*(str + 1) == '$' || !*(str + 1))))
	{
		dest = ft_strnew(1);
		ft_strncat(dest, str, 1);
		*i += 1;
	}
	else
		dest = get_one_dollar(i);
	ft_strdel(&name);
	return (dest);
}
