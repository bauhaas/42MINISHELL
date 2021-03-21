/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_builtin.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bahaas <bahaas@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/21 14:52:26 by bahaas            #+#    #+#             */
/*   Updated: 2021/03/21 15:01:37 by bahaas           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	test_builtin(t_ms *ms)
{
	t_cmd *cmd;

	cmd = malloc(sizeof(t_cmd));

/*	
	cmd->content = malloc(sizeof(char *) * 3);
	cmd->content[0]= ft_strdup("pwd");
	cmd->content[1]= ft_strdup("args");
	cmd->content[2]= NULL;
*/
/*
	cmd->content = malloc(sizeof(char *) * 5);
	cmd->content[0]= ft_strdup("echo");
	cmd->content[1]= ft_strdup("-n");
	cmd->content[2]= ft_strdup("Hello");
	cmd->content[3]= ft_strdup("World");
	cmd->content[4]= NULL;*/
	
	/*cmd->content = malloc(sizeof(char *) * 2);
	cmd->content[0]= ft_strdup("cd");
	//cmd->content[1]= ft_strdup("srcs");
	cmd->content[1]= NULL;
	cmd->next = NULL;
	execute(&ms, cmd);

	cmd->content = malloc(sizeof(char *) * 2);
	cmd->content[0]= ft_strdup("pwd");
	cmd->content[1]= NULL;
	execute(&ms, cmd);

	cmd->content = malloc(sizeof(char *) * 2);
	cmd->content[0]= ft_strdup("unset");
	cmd->content[1]= ft_strdup("HOME");
	cmd->content[2]= NULL;
	execute(&ms, cmd);

	cmd->content = malloc(sizeof(char *) * 2);
	cmd->content[0]= ft_strdup("cd");
	//cmd->content[1]= ft_strdup("srcs");
	cmd->content[1]= NULL;
	cmd->next = NULL;
	execute(&ms, cmd);
*/

	//"env" - OK
	cmd->content = malloc(sizeof(char *) * 2);
	cmd->content[0]= ft_strdup("env");
	cmd->content[1]= NULL;
	cmd->next = NULL;

	execute(ms, cmd);
}
