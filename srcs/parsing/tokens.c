/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokens.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bahaas <bahaas@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/20 14:29:47 by bahaas            #+#    #+#             */
/*   Updated: 2021/05/06 13:13:37 by bahaas           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void		free_tokens(t_tokens *tokens)
{
	t_tokens		*ptr_list;
	t_tokens		*ptr_next;

	ptr_list = tokens;
	while (ptr_list)
	{
		ptr_next = ptr_list->next;
		ft_strdel(&ptr_list->content);
		free(ptr_list);
		ptr_list = ptr_next;
	}
	tokens = NULL;
}

int			set_token_type(char *word_list)
{
	int type;

	type = CMD_ARGS;
	if (!ft_strcmp(word_list, "|"))
		type = PIPES;
	else if ((!ft_strcmp(word_list, ">")) ||
		(!ft_strcmp(word_list, ">>")) ||
		(!ft_strcmp(word_list, "<")))
		type = REDIR;
	return (type);
}

void		fill_token(t_tokens **new, t_ms *ms, char **word)
{
	(*new)->content = ft_strdup(*word);
	if (ms->escaped_tokens != 1)
		(*new)->type_content = set_token_type(*word);
	else
		(*new)->type_content = 1;
	ms->escaped_tokens = 0;
	if (ms->is_env)
		(*new)->is_env = 1;
	if (ft_strcmp(*word, "echo") == 0)
		ms->echo = TRUE;
	ft_strdel(word);
}

void		create_token(t_ms *ms, char **word)
{
	t_tokens		*new;
	t_tokens		*tmp;

	tmp = ms->head_tokens;
	if (*word)
	{
		new = ft_memalloc(sizeof(t_tokens));
		if (new == NULL)
			return ;
		fill_token(&new, ms, word);
		if (ms->head_tokens == NULL)
			ms->head_tokens = new;
		else
		{
			while (tmp != NULL && tmp->next != NULL)
				tmp = tmp->next;
			tmp->next = new;
			new->prev = tmp;
		}
	}
}
