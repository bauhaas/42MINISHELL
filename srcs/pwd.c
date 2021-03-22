/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bahaas <bahaas@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/18 14:15:54 by bahaas            #+#    #+#             */
/*   Updated: 2021/03/21 14:51:19 by bahaas           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int ft_pwd(t_ms *ms, t_cmd *cmd)
{
	char *pwd;

	pwd = malloc(2048);
	if(cmd->content[1])
		write(1, "pwd: too many arguments\n" ,24);
	else
	{
		ms->pwd=getcwd(pwd, 2048);
		write(1, ms->pwd, ft_strlen(ms->pwd));
		write(1, "\n", 1);
	}
	return (0);
}
