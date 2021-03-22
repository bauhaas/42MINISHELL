/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bahaas <bahaas@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/21 14:52:26 by bahaas            #+#    #+#             */
/*   Updated: 2021/03/22 18:27:55 by bahaas           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	tmp_line_to_cmd(t_ms *ms, char *line)
{
	t_cmd *cmd;

	printf("(%d)line = %s\n", ms->exit, ms->line);
	
	cmd = malloc(sizeof(t_cmd));
	cmd->content = ft_split(line, ' ');
	int i = 0;
		printf("cmd = ");
	while(cmd->content[i])
	{
		printf("\"%s\" ", cmd->content[i]);
		i++;
	}
	printf("\n");
	cmd->next = NULL;
	cmd->ret_value = 0;
	execute(ms, cmd);
	
	//CHECK UPDATE PWD & OLDPWD with cd use
	/*
	printf("ms.pwd = %s\n", ms->pwd);
	printf("ms.oldpwd = %s\n", ms->old_pwd);
	char *pwd_env = ft_getenv(&ms->env, "PWD");
	char *oldpwd_env = ft_getenv(&ms->env, "OLDPWD");
	printf("ms->env->pwd = %s\n", pwd_env);
	printf("ms->env->oldpwd = %s\n", oldpwd_env);
	*/
}
