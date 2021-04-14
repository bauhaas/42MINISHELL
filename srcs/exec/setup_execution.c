/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   setup_execution.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bahaas <bahaas@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/13 14:06:30 by bahaas            #+#    #+#             */
/*   Updated: 2021/04/14 17:20:09 by bahaas           ###   ########.fr       */
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
	//printf("ENTER IN RESET FD\n");
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
//	printf("ENTER IN NEXT RUN\n");
//	printf("Current cmd : %s\n", cmd->content[0]);
	//permet de passer la cmd séparatrice ou de passer à null si pas de cmd suivante
	cmd = cmd->next;
	//print info
//	if (cmd)
//		printf("Current cmd after one inc: %s\n", cmd->content[0]);
	// tant que cmd sont des separateurs go à la suivante
	while (cmd && cmd->type_link != CMD_ARGS)
	{
//		printf("AAAAAAAAAAAA\n");
		cmd = cmd->next;
		if (cmd && cmd->type_link == CMD_ARGS && cmd->prev->type_link != 5)
		{
//			printf("BBBBBBBBBBBBBBB\n");
			cmd = cmd->next;
		}
	}
	//PRINT INFO
	/*
	if (cmd)
		printf("Next cmd : %s\n", cmd->content[0]);
	else
		printf("Next cmd : NONE\n");
		*/
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
	//status permet d'attendre qu'un bloc de commande s'execute avant de lancer
	//un nouveau bloc (sans, les commandes s'execute dans l'ordre inverse)
	int status;
//	printf("ENTER IN SETUP\n");
//	printf("INITIAL PID : %d", getpid());
	while (ms->exit == 1 && cmd)
	{
		ms->flag = 0;
		ms->recursive = 1;
		choose_action(ms, cmd);
		reset_fd(ms);
		waitpid(-1, &status, 0);
	//	printf("CURR PID : %d", getpid());
		if(ms->flag == 1)
		{
//			printf("cmd content before exit : %s\n", cmd->content[0]);
//			ft_exit(ms, cmd);//imprime exit autant de fois que de pipe
			exit(0);
		}
		cmd = next_cmd_to_execute(cmd);
	}
//	printf("END OF SET_UP_EXECUTE\n");
}
