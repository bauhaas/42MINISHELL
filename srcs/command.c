/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bahaas <bahaas@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/21 14:52:26 by bahaas            #+#    #+#             */
/*   Updated: 2021/04/07 14:30:32 by bahaas           ###   ########.fr       */
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

void set_type_link(t_cmd *cmd, t_tokens **tokens)
{
	if((*tokens)->type_content == REDIR)
	{
		if(!strcmp((*tokens)->content, ">"))
			cmd->type_link = 7;
		else if(!strcmp((*tokens)->content, ">>"))
			cmd->type_link = 8;
		else
			cmd->type_link = 6;
	}
}

/*
 ** Find the number of stings to malloc in our cmd->content based on token type.
 */

int	token_number_in_cmd(t_tokens **tokens)
{
	t_tokens	*count;
	int			i;

	count = *tokens;
	i = 0;
	while(count)
	{
		if(count->type_content == CMD || count->type_content == ARGS)
		{
			count = count->next;
			i++;
		}
		else
		{
			i++;
			break;
		}
	}
	return (i);	
}

/*
 ** We translate a grp of tokens into a cmd. We find number of tokens until
 ** tokens isn't a CMD/ARGS. Then we create a cmd and fill content with our
 ** tokens. We also set flags to know if our cmd is piped or redirected.
 */

void tokens_to_cmd(t_ms *ms, t_cmd **cmd, t_tokens **tokens)
{
	t_cmd		*new_cmd;
	t_tokens	*head;
	int			i;

	new_cmd = create_cmd(cmd);
	i = token_number_in_cmd(tokens);
//	printf("\nCOMMAND IN CREATION\n");
//	printf("number of strings to malloc in cmd->content: %d\n", i);
	new_cmd->content = malloc(sizeof(char *) * (i + 1));
	i = 0;
	if((*tokens)->type_content != CMD && (*tokens)->type_content != ARGS)
	{
	//	printf("NOT ARG OR CMD\n");
	//	printf("*tokens->content : %s\n", (*tokens)->content);
		new_cmd->content[i] = ft_strdup((*tokens)->content);
		if((*tokens)->type_content == PIPES)
			new_cmd->type_link = PIPES;
		else if((*tokens)->type_content == END_CMD)
			new_cmd->type_link = END_CMD;
		else if((*tokens)->type_content == REDIR)
			set_type_link(new_cmd, tokens);
		else
			new_cmd->type_link = NO_END;
		i++;
		new_cmd->content[i] = NULL;
		*tokens = (*tokens)->next;
		return ;
	}
	while((*tokens))
	{
		if((*tokens)->type_content == CMD || (*tokens)->type_content == ARGS)
		{
		//	printf("*tokens->content : %s\n", (*tokens)->content);
			new_cmd->content[i] = ft_strdup((*tokens)->content);
			/*if((*tokens)->type_content == PIPES)
				new_cmd->type_link = PIPES;
			else if((*tokens)->type_content == END_CMD)
				new_cmd->type_link = END_CMD;
			else if((*tokens)->type_content == REDIR)
				set_type_link(new_cmd, tokens);
			else
				new_cmd->type_link = NO_END;*/
			new_cmd->type_link = CMD;
		//	printf("test type_link : %d\n", new_cmd->type_link);
			i++;
			*tokens = (*tokens)->next;
		}
		else
			break;
	}
	new_cmd->content[i] = NULL;
}

void	line_to_cmd(t_ms *ms, char *line, t_cmd *cmd)
{
	t_tokens *tokens;
	t_tokens *head;
	int ret;

	tokens = NULL;
	cmd = NULL;
	ret = 0;
	if(ret == 0)
		ret = get_tokens(ms, &tokens, line);
//	print_tokens(tokens);
	head = tokens;
	while(head)
	{
		//printf("first sent to t_to_c : %s", head->content);
		tokens_to_cmd(ms, &cmd, &head);
	}
	free_tokens(tokens);
	print_cmd(cmd);
	ms->start = cmd;
	setup_execution(ms, cmd);
}
