/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clorin <clorin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/24 13:13:22 by clorin            #+#    #+#             */
/*   Updated: 2021/05/06 04:53:49 by bahaas           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

/*
**	this function returns the name's var after $ or NULL if nothing
*/

static char			*get_last_ret(t_ms *ms, int *i)
{
	char			*dest;

	dest = ft_itoa(ms->last_ret);
	*i += 2;
	return (dest);
}

static char			*get_null_value(int len, int *i)
{
	char			*dest;

	dest = ft_strnew(1);
	*i += len + 1;
	return (dest);
}

static char			*get_good_value(t_ms *ms, char *name, int len, int *i)
{
	char			*dest;

	dest = ft_strnew(ft_strlen(ft_getenv(&ms->env, name, TRUE)));
	ft_strcat(dest, ft_getenv(&ms->env, name, TRUE));
	*i += len + 1;
	return (dest);
}

static char			*get_one_dollar(int *i)
{
	char			*dest;

	dest = ft_strnew(1);
	dest[0] = '$';
	*i += 1;
	return (dest);
}

char				*value(t_ms *ms, char *str, int *i)
{
	char			*dest;
	char			*name;
	int				len;

	ms->is_env = 1;
	dest = NULL;
	name = get_name_var(str);
	len = ft_strlen(name);
	if (*str == '$' && *(str + 1) == '?')
		dest = get_last_ret(ms, i);
	else if (name && ft_getenv(&ms->env, name, TRUE))
		dest = get_good_value(ms, name, len, i);
	else if (name && !ft_getenv(&ms->env, name, TRUE))
		dest = get_null_value(len, i);
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
