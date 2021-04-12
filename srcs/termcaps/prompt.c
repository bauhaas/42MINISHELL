/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clorin <clorin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/18 13:56:39 by clorin            #+#    #+#             */
/*   Updated: 2021/04/12 17:48:34 by bahaas           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static char	*home_prompt(t_ms *ms, char *pwd)
{
	char	*dest;
	char	*home;
	int		diff;
	int		i;

	i = 0;
	home = ft_getenv(&ms->env, "HOME", TRUE);
	diff = ft_strcmp(pwd, home);
	if (diff > 0)
	{
		while (home[i] == pwd[i])
			i++;
		dest = ft_strnew(ft_strlen(pwd + i) + 1);
		dest[0] = '~';
		ft_strcat(dest, pwd + i);
	}
	else if (diff == 0)
	{
		dest = ft_strnew(1);
		dest[0] = '~';
	}
	else
		dest = ft_strdup(ms->pwd);
	return (dest);
}

int			prompt(t_ms *ms)
{
	char	*prompt;
	int		ret;

	prompt = home_prompt(ms, ms->pwd);
	ret = ft_strlen(prompt);
	if (ms->last_ret)
	{
		ft_putstr_fd("\033[7;31m 🤬 \033[0m", STDOUT);
		ft_putstr_fd("\033[0;31;44m \033[0m", STDOUT);
	}
	else
	{
		ft_putstr_fd("\033[7;32m 😀 \033[0m", STDOUT);
		ft_putstr_fd("\033[0;32;44m \033[0m", STDOUT);
	}
	ft_putstr_fd("\033[7;34m", 1);
	ft_putstr_fd(prompt, 1);
	ft_putstr_fd(" \033[0;34;43m\033[0m", 1);
	ft_putstr_fd("\033[7;33m$>\033[0m", 1);
	ft_putstr_fd("\033[0;33;33m\033[0m", 1);
	ft_strdel(&prompt);
	return (ret + 11);
}
