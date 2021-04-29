/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bahaas <bahaas@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/21 14:52:26 by bahaas            #+#    #+#             */
/*   Updated: 2021/04/29 13:44:05 by bahaas           ###   ########.fr       */
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
	//printf("token number in cmd : %d\n", i);
	return (i);
}

/*
   void 	token_is_arg_or_cmd(t_cmd **new_cmd, t_tokens **tokens, int i)
   {
   while ((*tokens))
   {
   if ((*tokens)->type_content == CMD_ARGS)
   {
   (*new_cmd)->content[i] = ft_strdup((*tokens)->content);
   (*new_cmd)->type_link = CMD_ARGS;
   i++;
 *tokens = (*tokens)->next;
 }
 else
 break ;
 }
 }*/

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
	//token_is_arg_or_cmd(&new_cmd, tokens, i);
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

/*
static void exec_last_cmd(t_ms* ms, t_cmd *cmd, int in, int out)
{
	int pid;
	int fd[2];
	int fdd = 0;
if (cmd && get_bltn(ms, cmd->content[0]))
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
		}
		else
		{
			pipe(fd);
			pid = fork();
			if (pid == -1)
			{
				perror("fork");
				exit(1);
			}
			else if (pid == 0) {
				if (DEBUG)
					printf("fils\n");
				dup2(fdd, 0);
				if (cmd->next && cmd->next->type_link == 4)
					dup2(fd[1], 1);
				close(fd[0]);
				search_prog(ms, cmd);
				if (cmd->ret_value)
				{
					if(!ft_strcmp(cmd->content[0], ".."))
						cmd->ret_value = 4;
					error_file(ms, cmd);
					exit(cmd->ret_value);
				}
				if (DEBUG)
					printf("execvp(%s)(",(char*)cmd->content[0]);
				execvp(cmd->content[0], cmd->content);
				//exit(1);
			}
			else {
				if (DEBUG)
					printf("wait du pere\n");
				int status;
				waitpid(-1, &status, 0);
				if (WIFEXITED(status))
				{
					ms->last_ret = WEXITSTATUS(status);
					if (DEBUG)
						printf("status de sortie du fils = %d\n", ms->last_ret);
				}
				if (WIFSIGNALED(status))
					ms->last_ret = WTERMSIG(status) + 128;
				close(fd[1]);
				fdd = fd[0];
			}
		}
}*/

static void pipeline(t_cmd *cmd, t_ms *ms)
{
	int fd[2];
	pid_t pid;
	int fdd = 0;				/* Backup */

	// while (*cmd != NULL)

	if (DEBUG)
		printf("Pipeline : minishell pid : %d\n", getpid());
	//while (cmd && cmd->next)
	while (cmd)
	{
		if (DEBUG)
		{
			printf("start loop\n");
			printf("cmd en cours = %s\n", cmd->content[0]);
			printf("\tcmd suivante = %s\n", (cmd->next)?cmd->next->content[0]:"(null)");
		}
		if (cmd && get_bltn(ms, cmd->content[0]))
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
		}
		else
		{
			pipe(fd);
			pid = fork();
			if (pid == -1)
			{
				perror("fork");
				exit(1);
			}
			else if (pid == 0) {
				if (DEBUG)
					printf("fils\n");
				dup2(fdd, 0);
				if (cmd->next && cmd->next->type_link == 4)
					dup2(fd[1], 1);
				close(fd[0]);
				search_prog(ms, cmd);
				if (cmd->ret_value)
				{
					if(!ft_strcmp(cmd->content[0], ".."))
						cmd->ret_value = 4;
					error_file(ms, cmd);
					exit(cmd->ret_value);
				}
				execvp(cmd->content[0], cmd->content);
				//exit(1);
			}
			else {
				if (DEBUG)
					printf("wait du pere\n");
				int status;
				waitpid(-1, &status, 0); 		/* Collect childs */
				if (WIFEXITED(status))
				{
					ms->last_ret = WEXITSTATUS(status);
					if (DEBUG)
						printf("status de sortie du fils = %d\n", ms->last_ret);
				}
				if (WIFSIGNALED(status))
					ms->last_ret = WTERMSIG(status) + 128;
				close(fd[1]);
				fdd = fd[0];
			}
		}
		cmd = cmd->next;
		if(cmd && cmd->type_link == 4)
			cmd=cmd->next;
	}
/*
	if (DEBUG)
		printf("go to execute last cmd : %s\n", cmd->content[0]);
	exec_last_cmd(ms, cmd, fdd, STDOUT);
*/
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
	// if (nb_cmd(cmd) > 1)
	pipeline(tmp, ms);
	// else
	// {
	// 	while(tmp)
	// 	{
	// 		if(!tmp->next)
	// 			tmp->is_last = 1;
	// 		tmp = tmp->next;
	// 	}
	// 	if (last_cmd_status(ms, cmd))
	// 	setup_execution(ms, cmd);
	// }


	free_cmd(to_free);
}
