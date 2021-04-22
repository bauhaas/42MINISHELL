/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bahaas <bahaas@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/21 14:52:26 by bahaas            #+#    #+#             */
/*   Updated: 2021/04/21 22:45:38 by bahaas           ###   ########.fr       */
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
	if((*tokens)->is_env)
		new_cmd->is_env = 1;
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
		if ((*tokens)->type_content == CMD_ARGS)
		{
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
	else if ((cmd->type_link == 6 || cmd->type_link == 7 || cmd->type_link == 8)
				&& cmd->prev
				&& (cmd->prev->type_link == 6 || cmd->prev->type_link == 7 || cmd->prev->type_link == 8))
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

void		line_to_cmd(t_ms *ms, char *line, t_cmd *cmd)
{
	t_tokens *head;

	ms->tokens = NULL;
	cmd = NULL;
	parse(line, ms);
	print_tokens(ms->tokens);
	head = ms->tokens;
	while (head)
		tokens_to_cmd(&cmd, &head);
	free_tokens(ms->tokens);
	print_cmd(cmd);
	if (last_cmd_status(ms, cmd))
		setup_execution(ms, cmd);
}
