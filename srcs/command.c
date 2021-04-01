/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bahaas <bahaas@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/21 14:52:26 by bahaas            #+#    #+#             */
/*   Updated: 2021/04/01 01:50:17 by bahaas           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

/*
** Create a new command in our cmd linked list.
*/

t_cmd		*create_cmd(t_cmd **cmd)
{
	t_cmd	*tmp;
	t_cmd	*new;

	tmp = *cmd;
	new = ft_memalloc(sizeof(t_cmd));
	if (new == NULL)
		return (NULL);
	new->is_pipe = FALSE;
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

void tokens_to_cmd(t_ms *ms, t_cmd **cmd, t_tokens **tokens)
{
	t_cmd *new_cmd;
	t_tokens *count;
	t_tokens *head;
	int i = 0;

	count = *tokens;
	printf("\nCOMMAND IN CREATION\n");
	new_cmd = create_cmd(cmd);
	// 1st loop, we are searchin the total of strings to malloc in our cmd->content;
	while(count)
	{
		if(count->type_content == CMD || count->type_content == ARGS || count->type_content == REDIR)
		{
			count = count->next;
			i++;
		}
		else
			break;
	}
	printf("number of strings to malloc in cmd->content: %d\n", i);
	new_cmd->content = malloc(sizeof(char *) * i + 1);
	i = 0;
	//after malloc, we fill our cmd->content token by token until we found a pipe/;/redirection
	while((*tokens))
	{
		if((*tokens)->type_content == CMD || (*tokens)->type_content == ARGS || (*tokens)->type_content == REDIR)
		{
			new_cmd->content[i] = ft_strdup((*tokens)->content);
			printf("*tokens->content : %s\n", (*tokens)->content);
			*tokens = (*tokens)->next;
			i++;
		}
		//if our token is a pipe/;/redirection we skip that token and go to the next one
		//comme on skip ce token, on pourrait faire en sorte de set une nouvelle variable ds cmd pr lui indiquer le type de token qu'on a skip
		//pr ensuite s'en souvenir et lier les in & out des différentes cmd.
		else
		{
			*tokens = (*tokens)->next;
			new_cmd->is_pipe = TRUE;
			break;
		}
	}
	//je sais pas si on doit set les autres variables (ret_value etc à ce moment)
	new_cmd->content[i] = NULL;
}

void	tmp_line_to_cmd(t_ms *ms, char *line)
{
	t_tokens *tokens;
	int ret;
	t_cmd *cmd;

	tokens = NULL;
	cmd = NULL;
	ret = 0;

	printf("(%d)line = %s\n", ms->exit, ms->line);
	if(ret == 0)
		ret = get_tokens(ms, &tokens, line);
	print_tokens(tokens);
	while(tokens)
		tokens_to_cmd(ms, &cmd, &tokens);
	print_cmd(cmd);
	execute(ms, cmd);
}
