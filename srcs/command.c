/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bahaas <bahaas@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/21 14:52:26 by bahaas            #+#    #+#             */
/*   Updated: 2021/04/30 13:18:26 by bahaas           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

/*
 ** Create a new node in our cmd linked list.
 */

t_cmd		*create_cmd(t_cmd **cmd)
{
	t_cmd	*tmp;
	t_cmd	*new;

	tmp = *cmd;
	new = ft_memalloc(sizeof(t_cmd));
	if (new == NULL)
		return (NULL);
	new->type_link = 0;
	while (tmp != NULL && tmp->next != NULL)
		tmp = tmp->next;
	if (tmp == NULL)
		*cmd = new;
	else
	{
		tmp->next = new;
		new->prev = tmp;
	}
	return (new);
}

void		set_redirection_type(t_cmd *cmd, t_tokens **tokens)
{
	if ((*tokens)->type_content == REDIR)
	{
		cmd->type_link = 6;
		if (!strcmp((*tokens)->content, ">"))
			cmd->type_link = 7;
		else if (!strcmp((*tokens)->content, ">>"))
			cmd->type_link = 8;
	}
}

/*
 ** Find the number of strings to malloc in our cmd->content based on token type.
 */

int			token_number_in_cmd(t_tokens **tokens)
{
	t_tokens	*count;
	int			i;

	count = *tokens;
	i = 0;
	while (count)
	{
		if (count->type_content == CMD_ARGS)
		{
			count = count->next;
			i++;
		}
		else
		{
			i++;
			break ;
		}
	}
	return (i);
}

/*
 ** We translate a grp of tokens into a cmd. We find number of tokens until
 ** tokens isn't a CMD/ARGS. Then we create a cmd and fill content with our
 ** tokens. We also set flags to know if our cmd is piped or redirected.
 */

void		tokens_to_cmd(t_cmd **cmd, t_tokens **tokens)
{
	t_cmd		*new_cmd;
	int			i;

	new_cmd = create_cmd(cmd);
	i = token_number_in_cmd(tokens);
	new_cmd->content = malloc(sizeof(char *) * (i + 1));

	i = 0;
	if ((*tokens)->type_content != CMD_ARGS)
	{
		new_cmd->content[i] = ft_strdup((*tokens)->content);
		if ((*tokens)->type_content == PIPES)
			new_cmd->type_link = PIPES;
		else if ((*tokens)->type_content == END_CMD)
			new_cmd->type_link = END_CMD;
		else if ((*tokens)->type_content == REDIR)
			set_redirection_type(new_cmd, tokens);
		i++;
		new_cmd->content[i] = NULL;
		*tokens = (*tokens)->next;
		return ;
	}
	while ((*tokens))
	{
		if ((*tokens)->type_content == CMD_ARGS && ((*tokens)->prev && (*tokens)->prev->type_content == REDIR))
		{
			new_cmd->content[i] = ft_strdup((*tokens)->content);
			new_cmd->type_link = CMD_ARGS;
			i++;
			*tokens = (*tokens)->next;
			break ;
		}
		if ((*tokens)->type_content == CMD_ARGS)
		{
			if ((*tokens)->is_env)
				new_cmd->is_env = 1;
			new_cmd->content[i] = ft_strdup((*tokens)->content);
			new_cmd->type_link = CMD_ARGS;
			i++;
			*tokens = (*tokens)->next;
		}
		else
			break ;
	}
	new_cmd->content[i] = NULL;
}

int			print_cmd_error(t_ms *ms, t_cmd *cmd)
{
	if (cmd->type_link == 4 && cmd->next == NULL)
		printf("minishell: syntax error near unexpected token  « | »\n");
	else if ((cmd->type_link == 4 || cmd->type_link == 6 || cmd->type_link == 7 || cmd->type_link == 8)
			&& cmd->prev
			&& (cmd->prev->type_link == 4 || cmd->prev->type_link == 6 || cmd->prev->type_link == 7 || cmd->prev->type_link == 8))
	{
		printf("minishell: syntax error near unexpected token");
		printf(" « %s »\n", cmd->content[0]);
	}
	else if ((cmd->type_link == 6 || cmd->type_link == 7 || cmd->type_link == 8)
			&& cmd->next == NULL)
		printf("minishell: syntax error near unexpected token  « newline »\n");
	ms->last_ret = 2;
	return (0);
}

int			last_cmd_status(t_ms *ms, t_cmd *cmd)
{
	t_cmd *tmp;

	tmp = cmd;
	if (!strcmp(tmp->content[0], ";"))
	{
		printf("minishell: syntax error near unexpected token");
		printf(" « %s »\n", tmp->content[0]);
		ms->last_ret = 2;
		return (0);
	}
	while (tmp)
	{
		if ((tmp->type_link == 4 || tmp->type_link == 6 || tmp->type_link == 7
					|| tmp->type_link == 8) && tmp->next == NULL)
			return (print_cmd_error(ms, tmp));
		tmp = tmp->next;
	}
	return (1);
}

static void	cmd_is_bltn(t_ms *ms, t_cmd *cmd, int exit_in_pipeline)
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

static void pipeline(t_cmd *cmd, t_ms *ms)
{
	int fd[2];
	pid_t pid;
	int fdd = 0;				/* Backup */

	ms->forked = 0;
	if (DEBUG)
		printf("Pipeline : minishell pid : %d\n", getpid());
	while (cmd)
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
				if (DEBUG)
					printf("fils(%d) cmd = %s\n",getpid(), cmd->content[0]);
				dup2(fdd, STDIN);
				if (cmd->next && cmd->next->type_link == 4)
					dup2(fd[1], STDOUT);
				if (cmd->next && is_redir(cmd->next))
				{
					fd_open=fonctionsuperdrole(cmd->next)
					/*fonction
					ouvre les fichiers >
					renvoi le fd_open
					int fd_open = open(cmd->next->next->content[0], O_CREAT | O_WRONLY | O_APPEND, S_IRWXU);*/
					dup2(fd_open, STDOUT);

				}
				close(fd[0]);
				if(DEBUG)
					printf("Check cmd before bltn or exec : %s\n", cmd->content[0]);
				// j'ai refacto le code en commentaire dans la fonction cmd_is_bltn
				cmd_is_bltn(ms, cmd, 1);
				/*	if (cmd && get_bltn(ms, cmd->content[0]))
					{
					if(DEBUG)
					printf("cmd : %s is a builtin\n", cmd->content[0]);
					if(!cmd->next || (cmd->next && cmd->next->type_link != 4))
					{
					if (DEBUG)
					printf("builtins : %s\n", cmd->content[0]);
					ms->last_ret = launch_bltn(ms, cmd);
					if (DEBUG)
					printf("sortie de builtins avec ret = %d\n", ms->last_ret);
					exit(ms->last_ret);
					}
					else
					{
					if (DEBUG)
					printf("On n'execute pas le builtin car il est suivit d'un pipe\n");
					}
					}
					else
					{
					if(DEBUG)
					printf("*** %s =>pas builtins \n", cmd->content[0]);
					}*/
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
				close(fd[1]);
				fdd = fd[0];
			}
		}
		cmd = cmd->next;
		/*
		en onction du typelink on se positionne sur la prochaine cmd
		*/
		if(cmd && cmd->type_link == 4)
			cmd=cmd->next;
		if(cmd && cmd->type_link == 7)
			cmd=cmd->next->next;
	}
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
	}
	else
	{
		if (DEBUG)
			printf("pas de Fork \t ms->last_ret = %d\n", ms->last_ret);
	}
	if (DEBUG)
		printf("sortie Pipeline\n");
}

static int	nb_cmd(t_cmd *cmd)
{
	int n_command_to_exec = 0;
	t_cmd *tmp;

	tmp = cmd;
	while(tmp)
	{
		if (tmp->type_link != 4)
			n_command_to_exec++;
		tmp = tmp->next;
	}
	return (n_command_to_exec);
}

void		line_to_cmd(t_ms *ms, char *line, t_cmd *cmd)
{
	t_tokens *head;
	t_cmd *tmp;
	t_cmd *to_free;

	ms->tokens = NULL;
	cmd = NULL;
	parse(line, ms);
	if (DEBUG >= 3)
		print_tokens(ms->tokens);
	head = ms->tokens;
	while (head)
		tokens_to_cmd(&cmd, &head);
	free_tokens(ms->tokens);
	if (DEBUG)
		print_cmd(cmd);
	to_free = cmd;
	tmp = cmd;

	if (nb_cmd(cmd) > 1 || (nb_cmd(cmd) == 1 && !get_bltn(ms, cmd->content[0])))
	{
		if(last_cmd_status(ms, cmd))
			pipeline(tmp, ms);
	}
	else
	{
		// j'ai refacto le code en commentaire dans la fonction cmd_is_bltn
		cmd_is_bltn(ms, cmd, 0);
		/*	if (cmd && get_bltn(ms, cmd->content[0]))
			{
			if(!cmd->next || (cmd->next && cmd->next->type_link != 4))
			{
			if (DEBUG)
			printf("builtins : %s\n", cmd->content[0]);
			ms->last_ret = launch_bltn(ms, cmd);
			if (DEBUG)
			printf("sortie de builtins avec ret = %d\n", ms->last_ret);
			}
			else
			{
			if (DEBUG)
			printf("On n'execute pas le builtin car il est suivit d'un pipe\n");
			}
			}*/
	}
	free_cmd(to_free);
}
