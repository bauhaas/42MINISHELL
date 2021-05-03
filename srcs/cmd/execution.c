/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bahaas <bahaas@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/03 11:47:55 by bahaas            #+#    #+#             */
/*   Updated: 2021/05/03 12:45:38 by bahaas           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static t_cmd	*next_cmd_to_execute(t_ms *ms, t_cmd *cmd)
{
	if(DEBUG)
		printf("enter in next cmd to exec\n");

	// On se place sur la commande suivante
	cmd = cmd->next;
	//Tant que notre cmd est une redir, un pipes ou est precedé d'une redir on ne doit pas 
	//l'execute donc on avance
	while(cmd)
	{
		cmd = cmd->next;
		if(cmd && ((is_redir(cmd)) || (is_type(cmd, PIPES)) || (cmd->prev && is_redir(cmd->prev))))
			cmd = cmd->next;
		else
			break ;
	}
	if(DEBUG)
	{
		if(cmd)
			printf("return cmd : %s\n", cmd->content[0]);
		else
			printf("no more cmd to exec\n\n");
	}
	return(cmd);
}


static void waiting_loop(t_ms *ms, t_cmd *cmd)
{
	if (ms->forked)
	{
		if (DEBUG)
			printf("wait du pere car il y a eu %d fork(s)\n", ms->forked);
		int status;
		int pid_fils = 0;
		int last_status[100][2];
		int i = 0;
		while (pid_fils >= 0 )
		{
			//reset_fd(ms);
			if (DEBUG)
				printf("last_ret = %d\n", ms->last_ret);
			pid_fils = waitpid(-1, &status, 0);
			last_status[i][0] = pid_fils;

			if (WIFEXITED(status))
			{
				ms->last_ret = WEXITSTATUS(status);
				last_status[i][1] = ms->last_ret;
				if (DEBUG)
					printf("status de sortie du fils %d = %d\n", pid_fils, ms->last_ret);
			}
			if (WIFSIGNALED(status))
			{
				if(DEBUG)
					printf("Sortie Signal du fils %d = %d\n", pid_fils, WTERMSIG(status) + 128);
				ms->last_ret = WTERMSIG(status) + 128;
				last_status[i++][1] = ms->last_ret;
			}
			i++;
		}
		int dernier_pid = 0;
		int indice = 0;
		for(i=0;i < ms->forked; i++)
		{
			if (DEBUG)
				printf("last_status pid=%d\tvalue=%d\n", last_status[i][0], last_status[i][1]);
			if (dernier_pid < last_status[i][0])
			{
				dernier_pid = last_status[i][0];
				indice = i;
			}
		}
		if (DEBUG)
			printf("le dernier pid est %d avec un ret = %d\n", last_status[indice][0], last_status[indice][1]);
		ms->last_ret = last_status[indice][1];
		ms->forked = 0;
	}
	else
	{
		if (DEBUG)
			printf("pas de Fork \t ms->last_ret = %d\n", ms->last_ret);
	}
}


void	cmd_is_bltn(t_ms *ms, t_cmd *cmd, int exit_in_pipeline)
{
	if (cmd && get_bltn(ms, cmd->content[0]))
	{
		if(!cmd->next || (cmd->next && cmd->next->type_link != 4))
		{
			if (DEBUG)
				printf("builtins : %s\n", cmd->content[0]);
			ms->last_ret = launch_bltn(ms, cmd);
			if (DEBUG)
				printf("sortie de builtins avec ret = %d\n", ms->last_ret);
			if(exit_in_pipeline)
				exit(ms->last_ret);
		}
		else
		{
			if (DEBUG)
				printf("On n'execute pas le builtin car il est suivit d'un pipe\n");
			exit(0);
		}
	}
	else
	{
		if(DEBUG)
			printf("*** %s =>pas builtins \n", cmd->content[0]);
	}
}

void pipeline(t_cmd *cmd, t_ms *ms)
{
	int fd[2];
	pid_t pid;
	int fdd = 0;
	int fdd_in = 0;
	int has_redir_first = 0;
	ms->ret = 0;
	int fd_open;

	ms->forked = 0;
	if (DEBUG)
		printf("Pipeline : minishell pid : %d\n", getpid());
	while(cmd && is_redir(cmd))
	{
		if (is_type(cmd, DRIGHT))
			redir(ms, cmd->next, DRIGHT);
		else if (is_type(cmd, RIGHT))
			redir(ms, cmd->next, RIGHT);
		else if (is_type(cmd, LEFT))
			input(ms, cmd->next);
		if(cmd)
			cmd = cmd->next->next;
		has_redir_first = 1;
		if(ms->ret)
		{
			ms->last_ret = 1;
			break;
		}
	}
	if(has_redir_first && is_type(cmd, PIPES) && cmd->next)
		cmd = cmd->next;
	reset_fd(ms);
	if(DEBUG)
	{
		if(cmd)
		{
			printf("cmd before pipe loop : %s\n", cmd->content[0]);
			printf("ms->ret : %d\n", ms->ret);
		}
		printf("ms->ret : %d\n", ms->ret);
	}
	while (cmd && !ms->ret)
	{
		if (DEBUG)
		{
			printf("start loop\n");
			printf("cmd en cours = %s\n", cmd->content[0]);
			printf("\tcmd suivante = %s\n", (cmd->next)?cmd->next->content[0]:"(null)");
		}
		if (cmd && !ft_strcmp(cmd->content[0], "exit"))
			;
		else
		{
			ms->forked++;
			pipe(fd);
			pid = fork();
			if (pid == -1)
			{
				perror("fork");
				exit(1);
			}
			else if (pid == 0)
			{
				/*
				   close_fd(fd[1]);
				   dup2(fd[0], STDIN);
				   ms->pipin = fd[0];
				   */
				if (DEBUG)
					printf("fils(%d) cmd = %s\n",getpid(), cmd->content[0]);
				dup2(fdd, STDIN);
				if (cmd->next && cmd->next->type_link == 4)
					dup2(fd[1], STDOUT);
				if (cmd->next && is_redir(cmd->next))
					set_redirection(ms, cmd);
				if(DEBUG)
					printf("ms->ret after redir : %d\n", ms->ret);
				if(ms->ret)
					exit(ms->ret);
				close(fd[0]);
				if(DEBUG)
					printf("Check cmd before bltn or exec : %s\n", cmd->content[0]);
				cmd_is_bltn(ms, cmd, 1);
				search_prog(ms, cmd);
				if(DEBUG)
					printf("cmd->ret_value = %d\n", cmd->ret_value);
				if (cmd->ret_value)
				{
					// if(!ft_strcmp(cmd->content[0], ".."))
					// 	cmd->ret_value = 4;
					//error_file(ms, cmd);
					if (DEBUG)
						printf("ms->last_ret =%d\n", ms->last_ret);
					exit(ms->last_ret);
				}
				execve(cmd->content[0], cmd->content, ms->arr_env);
			}
			else
			{
				//reset_fd(ms);

				close(fd[1]);
				fdd = fd[0];
				//ft_close(fd_open);
			}
		}
		cmd = next_cmd_to_execute(ms, cmd);
		if(DEBUG)
			if(cmd && cmd->content[0])
				printf("next cmd that will be executed : %s\n\n", cmd->content[0]);
	}
	waiting_loop(ms, cmd);
	if (DEBUG)
		printf("sortie Pipeline\n");
}

