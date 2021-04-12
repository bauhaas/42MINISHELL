/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokens.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bahaas <bahaas@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/20 14:29:47 by bahaas            #+#    #+#             */
/*   Updated: 2021/04/12 15:31:26 by bahaas           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void		free_tokens(t_tokens *tokens)
{
	while(tokens)
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

/*
** We trim all the space at the beginning of the line
** Then, we'll iterate on each type of token.
*/

/*
int	get_tokens(t_ms *ms, t_tokens **tokens, char *line)
{
	size_t	i;
	int		join;

	i = 0;
	join = 0;
	while (line[i])
	{
		while (is_space(line[i]))
			i++;
		if (is_str_tok(ms, tokens, line, &i, &join))
			return (1);
		if (is_sep_tok(ms, tokens, line, &i, &join))
			return (1);
		if (is_quote_tok(ms, tokens, line, &i, &join))
			return (1);
		if (is_dquote_tok(ms, tokens, line, &i, &join))
			return (1);
		if (is_escaped_tok(ms, tokens, line, &i, &join))
			return (1);
		if (is_expand_tok(ms, tokens, line, &i, &join))
			return (1);
	}
	return (0);
}*/


int		set_token_type_aaa(char *word_list, t_tokens *token)
{
	int type;

	type = CMD;
	if(!strcmp(word_list, "|"))
		type = PIPES;
	else if(!strcmp(word_list, ";"))
		type = END_CMD;
	else if((!strcmp(word_list, ">")) ||
		(!strcmp(word_list, ">>")) ||
		(!strcmp(word_list, "<")))
		type = REDIR;
	return(type);
}

void	get_tokens(t_ms *ms, t_tokens **tokens, t_list *word_list)
{
	t_tokens *new;

	new = *tokens;
	while(word_list)
	{
		new = create_token(tokens);
		if(new == NULL)
			return ;
		new->content = ft_strdup((char *)word_list->content);
	//	new->type_content = set_token_type_aaa(word_list, new);
		word_list = word_list->next;
	}
}
