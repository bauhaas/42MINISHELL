/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bahaas <bahaas@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/21 14:52:26 by bahaas            #+#    #+#             */
/*   Updated: 2021/04/30 21:04:26 by bahaas           ###   ########.fr       */
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

void		tokens_to_cmd(t_ms *ms, t_cmd **cmd, t_tokens **tokens)
{
	t_cmd		*new_cmd;
	int			i;

	//printf("enter with : (*tokens)->content : %s\n", (*tokens)->content);
	if((*tokens)->type_content == PIPES && ((*tokens)->prev && ((*tokens)->prev->type_content == PIPES)))
	{
		*tokens = (*tokens)->next;
		ms->total_consecutive_pipes++;
		return ;
	}
	else
	{
		new_cmd = create_cmd(cmd);
		i = token_number_in_cmd(tokens);
		new_cmd->content = malloc(sizeof(char *) * (i + 1));
	}
	/*
	   new_cmd = create_cmd(cmd);
	   i = token_number_in_cmd(tokens);
	   new_cmd->content = malloc(sizeof(char *) * (i + 1));
	   */
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
	else if (is_redir(cmd) && (cmd->next && (is_redir(cmd->next) || is_type(cmd, PIPES))))
	{
		printf("minishell: syntax error near unexpected token");
		printf(" « %s »\n", cmd->next->content[0]);
	}
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
	if (!strcmp(tmp->content[0], ";") && !tmp->next)
	{
		printf("test ;\n");
		printf("minishell: syntax error near unexpected token");
		printf(" « %s »\n", tmp->content[0]);
		ms->last_ret = 2;
		return (0);
	}
	while (tmp)
	{
		if (((tmp->type_link == 4 || tmp->type_link == 6 || tmp->type_link == 7
						|| tmp->type_link == 8) && tmp->next == NULL))
			return (print_cmd_error(ms, tmp));
		if(is_redir(tmp) && (tmp->next && is_redir(tmp->next)))
		{
			printf("test\n");
			return (print_cmd_error(ms, tmp));
		}
		else if(ms->total_consecutive_pipes == 2)
		{
			printf("tast\n");
			printf("minishell: syntax error near unexpected token  « | »\n");
			return (0);
		}
		else if(ms->total_consecutive_pipes > 2)
		{
			printf("tost\n");
			printf("minishell: syntax error near unexpected token  « || »\n");
			return (0);
		}
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

static void pipeline(t_cmd *cmd, t_ms *ms)
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
		//	printf("cmd in redir loop : %s\n", cmd->content[0]);
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
		if(cmd)
			printf("cmd before pipe loop : %s\n", cmd->content[0]);
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
			//// TRY REDIR BEFORE FORK
			/*
			t_cmd *file_to_redirect;
			if(cmd->next)
			{
				if(cmd->next->next)
					file_to_redirect = cmd->next->next;
				if(file_to_redirect)
				{

					if (is_type(file_to_redirect->prev, DRIGHT))
						redir(ms, cmd, DRIGHT);
					else if (is_type(file_to_redirect->prev, RIGHT))
						redir(ms, cmd, RIGHT);
					else if (is_type(file_to_redirect->prev, LEFT))
						input(ms, cmd);
				}
			}
			*/
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
				{
					//fd_open=fonctionsuperdrole(cmd->next)
					/*fonction
					  ouvre les fichiers >
					  renvoi le fd_open
					  int fd_open = open(cmd->next->next->content[0], O_CREAT | O_WRONLY | O_APPEND, S_IRWXU);*/
					//dup2(fd_open, STDOUT);
					//int fd_open;
					t_cmd *file_to_redirect;

					if(cmd->next->next)
						file_to_redirect = cmd->next->next;
					//	while(file_to_redirect)
					//	{

					/*
					   printf("test file to redir : %s\n", file_to_redirect->content[0]);
					   printf("test file to redir next : %s\n", file_to_redirect->next->next->content[0]);
					   printf("test file to redir next next : %s\n", file_to_redirect->next->next->content[0]);
					   */
					
					   if(is_type(file_to_redirect->prev, RIGHT))
					   {
					   ft_close(fd_open);
					   fd_open = open(file_to_redirect->content[0], O_CREAT | O_WRONLY | O_APPEND, S_IRWXU);
					   dup2(fd_open, STDOUT);
					   }
					   else if(is_type(file_to_redirect->prev, DRIGHT))
					   {
					   ft_close(fd_open);
					   fd_open = open(file_to_redirect->content[0], O_CREAT | O_WRONLY | O_TRUNC, S_IRWXU);
					   dup2(fd_open, STDOUT);
					   }
					   else if(is_type(file_to_redirect->prev, LEFT))
					   {
					   ft_close(fd_open);
					   fd_open = open(file_to_redirect->content[0], O_RDONLY, S_IRWXU);
					   dup2(fd_open, STDIN);
					   }
					  
					   ///TEST PR LOOP
					   /*
						if(file_to_redirect->next->next)
						{
						//	ft_close(fd_open);
						//	dup2(fd[1], STDOUT);
						printf("next file to redirect to : %s\n", file_to_redirect->next->next->content[0]);
						file_to_redirect = file_to_redirect->next->next;
						}
						*/
					/*
					if(file_to_redirect->next->next)
					{
					//printf("next file to redirect to : %s\n", file_to_redirect->next->next->content[0]);
					file_to_redirect = file_to_redirect->next->next;
					}
					*/


					/*
					else
					{
					//printf("no other file to redirect\n");
					break;
					}
					*/
				}

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
	ms->total_consecutive_pipes = 0;
	cmd = NULL;
	parse(line, ms);
	if (DEBUG >= 3)
		print_tokens(ms->tokens);
	head = ms->tokens;
	while (head)
		tokens_to_cmd(ms, &cmd, &head);
	free_tokens(ms->tokens);
	if (DEBUG)
		print_cmd(cmd);
	to_free = cmd;
	tmp = cmd;
	//printf("ms->total_consecutive_pipes : %d\n", ms->total_consecutive_pipes);
	if (nb_cmd(cmd) > 1 || (nb_cmd(cmd) == 1 && !get_bltn(ms, cmd->content[0])))
	{
		if(last_cmd_status(ms, cmd))
			pipeline(tmp, ms);
	}
	else
		cmd_is_bltn(ms, cmd, 0);
	free_cmd(to_free);
}
