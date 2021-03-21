/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bahaas <bahaas@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/20 16:00:10 by bahaas            #+#    #+#             */
/*   Updated: 2021/03/21 14:50:31 by bahaas           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

/*
** Set our bltn structure with pointer function and names linked to it.
*/

void	init_bltn(t_ms *ms)
{

	ms->bltn->bltn_name[0] = "echo";
	ms->bltn->bltn_cmd[0] = &ft_echo;
	ms->bltn->bltn_name[1] = "cd";
	ms->bltn->bltn_cmd[1] = &ft_cd;
	ms->bltn->bltn_name[2] = "pwd";
	ms->bltn->bltn_cmd[2] = &ft_pwd;
	ms->bltn->bltn_name[3] = "export";
	ms->bltn->bltn_cmd[3] = &ft_export;
	ms->bltn->bltn_name[4] = "unset";
	ms->bltn->bltn_cmd[4] = &ft_unset;
	ms->bltn->bltn_name[5] = "env";
	ms->bltn->bltn_cmd[5] = &ft_env;
	ms->bltn->bltn_name[6] = "exit";
	ms->bltn->bltn_cmd[6] = &ft_exit;
	ms->bltn->bltn_name[7] = NULL;
	ms->bltn->bltn_cmd[7] = NULL;
}

/*
** Detecte if the command used is a builtin or not.
*/

int	get_bltn(t_ms *ms, char *cmd)
{
	int	i;

	i = -1;
	printf("test, name cmd received: %s\n", cmd);
	while (ms->bltn->bltn_name[++i])
	{
		if (!ft_strcmp(ms->bltn->bltn_name[i], cmd))
			return (1);
	}
	return (0);
}

/*
** Execute the builtin function and store the ret value in our struct cmd
*/

int		launch_bltn(t_ms *ms, t_cmd *cmd)
{

	int i;

	i = -1;
	while (ms->bltn->bltn_name[++i])
	{
		if (!ft_strcmp(ms->bltn->bltn_name[i], cmd->content[0]))
		{
			printf("bltn name cmd: %s\n", ms->bltn->bltn_name[i]);
			cmd->ret_value = ms->bltn->bltn_cmd[i](ms, cmd);
			return (cmd->ret_value);
		}
	}
	return (1);
}

/*
** When you have filled cmd with all the instructions received. Go check
** if the 1st word is equivalent to a builtin or an exec that require to fork
*/

int	execute(t_ms *ms, t_cmd *cmd)
{
	while (cmd != NULL)
	{
		if (get_bltn(ms, cmd->content[0]))
			launch_bltn(ms, cmd);
		//else
			//fork_exec(ms, cmd)		// TO DO LATER :)
		cmd = cmd->next;
	}
	return (0);
}
