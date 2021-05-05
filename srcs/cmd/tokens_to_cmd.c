/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokens_to_cmd.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bahaas <bahaas@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/03 12:13:07 by bahaas            #+#    #+#             */
/*   Updated: 2021/05/05 11:59:40 by bahaas           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/*
** Create a new node in our cmd linked list.
*/

t_cmd	*create_cmd(t_cmd **cmd)
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
	{
		printf("cmd* = new\n");
		*cmd = new;
	}
	else
	{
		printf("else lol\n");
		tmp->next = new;
		new->prev = tmp;
	}
	return (new);
}

/*
** Find the number of strings to malloc in our cmd->content via token type.
*/

int		token_number_in_cmd(t_tokens **tokens)
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

int		set_cmd_creation(t_ms *ms, t_cmd **new, t_cmd **cmd, t_tokens **tokens)
{
	int i;

	i = 0;
	if ((*tokens)->type_content == PIPES && ((*tokens)->prev
				&& ((*tokens)->prev->type_content == PIPES)))
	{
		*tokens = (*tokens)->next;
		ms->total_consecutive_pipes++;
		return (FALSE);
	}
	else
	{
		*new = create_cmd(cmd);
		i = token_number_in_cmd(tokens);
		(*new)->content = malloc(sizeof(char *) * (i + 1));
		return (TRUE);
	}
}

void	tokens_to_cmd(t_ms *ms, t_cmd **cmd, t_tokens **tokens)
{
	t_cmd		*new;
	t_cmd		*tmp;
	(void)cmd;
	int i;

	i = 0;
	if ((*tokens)->type_content == PIPES && ((*tokens)->prev
				&& ((*tokens)->prev->type_content == PIPES)))
	{
		*tokens = (*tokens)->next;
		ms->total_consecutive_pipes++;
		return ;
	}
	else
	{
		//*new = create_cmd(cmd);
		tmp = ms->cmd;
		new = ft_memalloc(sizeof(t_cmd));
		i = token_number_in_cmd(tokens);
		new->content = malloc(sizeof(char *) * (i + 1));
		if ((*tokens)->type_content != CMD_ARGS)
			fill_sep_cmd(&new, tokens);
		else
			fill_arg_cmd(&new, tokens);
		
		if(ms->cmd == NULL)
			ms->cmd = new;
		else
		{
			while (tmp != NULL && tmp->next != NULL)
				tmp = tmp->next;
			tmp->next = new;
			new->prev = tmp;
		}
	//	printf("PRINT NEW IN TOKTOCMD\n");
	//	print_cmd(new);
	}
/*
	if (!set_cmd_creation(ms, cpy, cmd, tokens))
		return ;
	if ((*tokens)->type_content != CMD_ARGS)
		fill_sep_cmd(cpy, tokens);
	else
		fill_arg_cmd(cpy, tokens);
	printf("test cpy : %s\n", (*cpy)->content[0]);
	if((*cpy)->prev)
	printf("test cpy prev : %s\n", (*cpy)->prev->content[0]);
*/
}
