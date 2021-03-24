/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clorin <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/24 13:13:22 by clorin            #+#    #+#             */
/*   Updated: 2021/03/24 13:13:27 by clorin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int			is_quote(const char c)
{
	return (c == '\'' || c== '\"');
}

int			next_quote(const char *str, char c, int i)
{
	if (i >= ft_strlen(str))
		return (ft_strlen(str));
	while (str[i])
	{
		if (str[i] == c)
			return i;
		i++;
	}
	return (ft_strlen(str));
}

/*
**	This function returns TRUE if pos is in a string surrounded by quotes.
**	False otherwise.
*/
int			is_in_quote(const char *str, int pos)
{
	int		i;
	int		j;
	char	c;
	int		quote;

	i = 0;
	quote = FALSE;
	if (!str || *str == '\0')
		return (FALSE);

	/**
	on boucle pour trouver la premier quote
	on stock c
	quote = true
	on boucle pour trouver la fin de la quote
	 si pos dqns l'interval => True
	 sinon
	 	on continue
	*/
	while (str[i])
	{
		c = '\0';
		while (str[i] && !is_quote(str[i]))
			i++;
		if (!str[i] || pos <= i)
			return (FALSE);
		c = str[i++];
		j = next_quote(str, c, i) - 1;
		//printf("%d in [%d,%d] -> ", pos, i,j);
		if (pos >= i && pos <= j)
		{
			//printf("TRUE\n");
			return (TRUE);
		}
		//printf("FALSE\n");
		i = j + 2;
	}
	return (FALSE);
}
/*
** This function checks the number of " or '.
returns FALSE if the first quote is odd. TRUE if even.
*/
int			valid_quote(const char *str)
{
	int		i;
	int		cmpt;
	char	c;

	i = 0;
	cmpt = 0;
	if (!str || *str == '\0')
		return (TRUE);
	while (str[i] && str[i] != '\"' && str[i] != '\'')
		i++;
	if (!str[i])
		return (TRUE);
	cmpt++;
	c = str[i++];
	while (str[i])
	{
		if (str[i] == c)
			cmpt++;
		i++;
	}
	return ((cmpt % 2) == 0);
}

void		parse(const char *str)
{
	int		i;

	i = 0;
	while (str[i])
	{
		if((!ft_isspace(str[i]) && !is_quote(str[i])) || is_in_quote(str, i))
			printf("%c", str[i]);
		i++;
	}
}