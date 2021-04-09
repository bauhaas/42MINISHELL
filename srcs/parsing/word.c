/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   word.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bahaas <bahaas@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/20 14:29:47 by bahaas            #+#    #+#             */
/*   Updated: 2021/04/09 11:22:01 by bahaas           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/*
 ** While our line exist & isn't a sep / space / escaped char we increment
 ** the len of our word and then return it.
 */

size_t	strlen_tok(char *line)
{
	size_t	i;

	i = 0;
	if(line[i] == '$')
		return (i);
	while (line[i] && !is_separator(line[i]) && !is_space(line[i])
			&& line[i] != QUOTE && line[i] != DQUOTE && line[i] != BSLASH && line[i] != '$')
		i++;
	return (i);
}

/*
 ** Fill our token content until we are on a incorrect char in line.
 */

void	fill_str_tok(t_ms *ms, char *word, char *line)
{
	size_t	j;

	j = 0;
	while (line[j] != '\0' && !is_separator(line[j]) && !is_space(line[j])
			&& line[j] != QUOTE && line[j] != DQUOTE && line[j] != BSLASH && line[j] != '$')
	{
		word[j] = line[j];
		j++;
	}
}

/*
 ** In the case where a string/word is find, we set a new empty token with
 ** the size of the string/word found and then fill it. We increment the
 ** line index by the value of our word length.
 */

int		is_str_tok(t_ms *ms, t_tokens **tokens, char *line, size_t *i, int *join)
{

	printf("\nGO IN WORD\n");
	printf("current line : %s\n", &line[*i]);

	size_t		len;
	t_tokens	*new;

	len = strlen_tok(&line[*i]);
	if (len == 0)
		return (0);
	if(*join == 0)
	{
		printf("create a token\n");
		new = create_token(tokens);
		if (new == NULL)
			return (1);
		new->content = ft_strnew(len);
		if (new->content == NULL)
			return (1);
		fill_str_tok(ms, new->content, &line[*i]);
		new->type_content = ARGS;
		if(!new->prev || new->prev->type_content == PIPES || new->prev->type_content == END_CMD)
			new->type_content = CMD;
	}
	else
	{
		printf("join to prev token\n");
		t_tokens *tmp;
		tmp = *tokens;
		while(tmp->next)
			tmp = tmp->next;
		char *tmp_token;
		tmp_token = ft_strnew(len);
		fill_str_tok(ms,tmp_token, &line[*i]);
		tmp->content = ft_strjoin(tmp->content, tmp_token);
	}
	*i += len;
	if(line[*i] == ' ')
	{
		while(line[*i] == ' ')
			*i += 1;
		*join = 0;
	}
	else
		*join = 1;
	return (0);
}
