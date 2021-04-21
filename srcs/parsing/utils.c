/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bahaas <bahaas@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/20 14:29:47 by bahaas            #+#    #+#             */
/*   Updated: 2021/04/20 16:10:22 by bahaas           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int			is_spec_car(char c)
{
	return (c == '\\' || c == '$' || c == '"' || c == '\'' || c == ' ' ||
			c == '\t' || c == '|' || c == '>' || c == '<');
}

/*
** This function checks the the string str
** if open return the ascii code of the quote
** otherwise 0 if closed
*/

int			valid_quotes(const char *s, int len)
{
	char	*str;
	int		open;
	int		i;

	str = ft_strdup(s);
	open = 0;
	i = -1;
	while (str[++i] && i <= len)
	{
		if (i > 0 && str[i - 1] == '\\' && str[i] != '\'')
		{
			if (str[i] == '\\')
				str[i] = '_';
		}
		else if (open == 0 && str[i] == '\"')
			open = 34;
		else if (open == 0 && str[i] == '\'' && str[i - 1] != '\\')
			open = 39;
		else if (open == 34 && str[i] == '\"')
			open = 0;
		else if (open == 39 && str[i] == '\'')
			open = 0;
	}
	ft_strdel(&str);
	return (open);
}

int			valid_name(char *name)
{
	int	i;

	i = 0;
	if (!name || !*name)
		return (FALSE);
	if (name[i] == '$')
		i++;
	if (!ft_isalpha(name[i]) && name[i] != '_')
		return (FALSE);
	i++;
	while (name[i])
	{
		if (!ft_isalnum(name[i]) && name[i] != '_')
			return (FALSE);
		i++;
	}
	return (TRUE);
}

char		*get_name_var(char *str)
{
	char	*dest;
	int		i;
	int		j;

	i = 0;
	if (!str || !*str)
		return (NULL);
	if (str[i++] == '$')
	{
		j = i;
		if (ft_isalpha(str[i]) || str[i] == '_')
		{
			i++;
			while (str[i] && (ft_isalnum(str[i]) || str[i] == '_'))
				i++;
			dest = ft_strndup(str + j, i - j);
			return (dest);
		}
	}
	return (NULL);
}
