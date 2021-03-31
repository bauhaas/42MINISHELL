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

	//ret = ft_strlen(ms->pwd);
	char checkmark[] = { 0xe2, 0x9c, 0x93 };
	char cross[] = { 0xe2, 0x9d, 0x8c };
	//if(!ms->i)
	//{
	prompt_ = home_prompt(ms, ms->pwd);
	ret = ft_strlen(prompt_);		//write (1, ms->pwd, ret);
	write(1, prompt_, ret);
		write (1, "$> ", 3);
	ft_strdel(&prompt_);
	return (ret + 3);
	//}
	/*
	if(ms->cmd->ret_value)
	{
		write(1, "\e[0;31m", 7);
		write(1, cross, 3);
		write (1, ms->pwd, ret);
		write (1, "$> ", 3);
		write(1, "\e[0m", 4);
	}*/
	/*
	else
	{
		write(1, "\e[0;32m", 7);
		write(1, checkmark, 3);
		write (1, ms->pwd, ret);
		write (1, "$> ", 3);
		write(1, "\e[0m", 4);
	}*/
	//return (ret + 5);
}
