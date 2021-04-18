/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   setup_execution.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bahaas <bahaas@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/13 14:06:30 by bahaas            #+#    #+#             */
/*   Updated: 2021/04/19 00:19:56 by bahaas           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	close_fd(int fd)
{
	if (fd > 0)
		close(fd);
}

void	reset_fd(t_ms *ms)
{
	dup2(ms->in, STDIN);
	dup2(ms->out, STDOUT);
	close_fd(ms->fdin);
	close_fd(ms->fdout);
	close_fd(ms->pipin);
	close_fd(ms->pipout);
	ms->fdin = -1;
	ms->fdout = -1;
	ms->pipin = -1;
	ms->pipout = -1;
}

t_cmd	*next_cmd_to_execute(t_cmd *cmd)
{
	cmd = cmd->next;
	while (cmd && cmd->type_link != CMD_ARGS)
	{
		cmd = cmd->next;
		if (cmd && cmd->type_link == CMD_ARGS && cmd->prev->type_link != 5)
			cmd = cmd->next;
		if(cmd && (cmd->type_link == 6 || cmd->type_link == 7 || cmd->type_link == 8) && cmd->prev->type_link == 5)
			break;
	}
	return (cmd);
}

/*
 ** Tant qu'il n'y a pas d'exit et que nous ne somme pas au bout de notre liste cmd
 ** 1. Execute in choose_action cmd block per block (séparateur = ; )
 ** 2. Après exec on reset tout nos fd pr avoir des parametres propres pr le prochain bloc
 ** 3. On se déplace au prochain bloc de commande (premiere commande après un ;)
 */

void	setup_execution(t_ms *ms, t_cmd *cmd)
{
	int status;

	while (ms->exit == 1 && cmd)
	{
		ms->flag = 0;
		ms->recursive = 1;
		choose_action(ms, cmd);
		reset_fd(ms);
		waitpid(-1, &status, 0);
		if(ms->flag == 1)
			exit(0);
		cmd = next_cmd_to_execute(cmd);
	}
}
