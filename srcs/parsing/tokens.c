/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokens.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bahaas <bahaas@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/20 14:29:47 by bahaas            #+#    #+#             */
/*   Updated: 2021/04/12 17:04:45 by bahaas           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void		free_tokens(t_tokens *tokens)
{
	while (tokens)
	{
		free(tokens->content);
		tokens->content = NULL;
		tokens = tokens->next;
	}
	free(tokens);
	tokens = NULL;
}

/*
** Parse our list of tokens. Set a new one and add it at the end of our list
*/

t_tokens	*create_token(t_tokens **tokens)
{
	t_tokens	*tmp;
	t_tokens	*new;

	new = ft_memalloc(sizeof(t_tokens));
	tmp = *tokens;
	while (tmp != NULL && tmp->next != NULL)
		tmp = tmp->next;
	if (tmp == NULL)
		*tokens = new;
	else
	{
		tmp->next = new;
		new->prev = tmp;
	}
	return (new);
}

int			set_token_type(char *word_list, t_tokens *token)
{
	int type;

	type = CMD_ARGS;
	if (!strcmp(word_list, "|"))
		type = PIPES;
	else if (!strcmp(word_list, ";"))
		type = END_CMD;
	else if ((!strcmp(word_list, ">")) ||
		(!strcmp(word_list, ">>")) ||
		(!strcmp(word_list, "<")))
		type = REDIR;
	return (type);
}
