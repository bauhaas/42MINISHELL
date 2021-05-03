/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bahaas <bahaas@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/21 14:52:26 by bahaas            #+#    #+#             */
/*   Updated: 2021/05/03 11:56:01 by bahaas           ###   ########.fr       */
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
			return (print_cmd_error(ms, tmp));
		else if(ms->total_consecutive_pipes == 2)
		{
			printf("minishell: syntax error near unexpected token  « | »\n");
			return (0);
		}
		else if(ms->total_consecutive_pipes > 2)
		{
			printf("minishell: syntax error near unexpected token  « || »\n");
			return (0);
		}
		tmp = tmp->next;
	}
	return (1);
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
		select_execution(ms, cmd, 0);
	free_cmd(to_free);
}
