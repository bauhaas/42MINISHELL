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

int			prompt(t_ms *ms)
{
	int ret;

	ret = ft_strlen(ms->pwd);
	char checkmark[] = { 0xe2, 0x9c, 0x93 };
	char cross[] = { 0xe2, 0x9d, 0x8c };
	//if(!ms->i)
	//{
		write (1, ms->pwd, ret);
		write (1, "$> ", 3);
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
