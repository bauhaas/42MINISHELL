/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clorin <clorin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/18 13:56:39 by clorin            #+#    #+#             */
/*   Updated: 2021/03/29 18:19:30 by bahaas           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static char		*home_prompt(t_ms *ms, char *pwd)
{
	char	*dest;
	char	*home;
	int		i;
	int		diff;

	i = 0;
	home = ft_getenv(&ms->env, "HOME", TRUE);
	diff = ft_strcmp(pwd,home);
	if (diff > 0)
	{
		while (home[i] == pwd[i])
			i++;
		//printf("i = %d\n", i);
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
	int 	ret;
	char	*prompt_;

	//char checkmark[] = { 0xe2, 0x9c, 0x93 };
	//char cross[] = { 0xe2, 0x9d, 0x8c };
	prompt_ = home_prompt(ms, ms->pwd);
	ret = ft_strlen(prompt_);
	if(ms->last_ret)
		ft_putstr_fd("🤬 ", STDOUT);
	else
		ft_putstr_fd("\033[32m😀\033[0m ", STDOUT);
	write(1, prompt_, ret);
		write (1, "$> ", 3);
	ft_strdel(&prompt_);
	return (ret + 6);
}
