/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sep_and_escaped.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bahaas <bahaas@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/29 17:32:41 by bahaas            #+#    #+#             */
/*   Updated: 2021/04/08 21:49:43 by bahaas           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"


void	set_token_type(t_tokens *new, char *sep)
{
	if(!ft_strcmp(";", sep))
		new->type_content = END_CMD;
	else if(!ft_strcmp(">", sep) || !ft_strcmp(">>", sep) || !ft_strcmp("<", sep))
		new->type_content = REDIR;
	else if(!ft_strcmp("|", sep))
		new->type_content = PIPES;
}

/*
 ** Parse all the sep set and if we found that a sep is part of our line, we
 ** set a new token equals to the sep. Then inc our index to the sep len.
 */

int	is_sep_tok(t_ms *ms, t_tokens **tokens, char *line, size_t *i, int *join)
{
	size_t				j;
	t_tokens			*new;
	printf("\nGO IN SEP\n");
	printf("current line : %s\n", &line[*i]);
	j = 0;
	while (ms->sep_set[j] != NULL)
	{
		// it holds case with >>.
		if(ft_strnequ(ms->sep_set[j], &line[*i], ft_strlen(ms->sep_set[j])) && j == 1 && line[*i + 1] == '>')
			j++;
		if (ft_strnequ(ms->sep_set[j], &line[*i], ft_strlen(ms->sep_set[j])))
		{
			new = create_token(tokens);
			if (new == NULL)
				return (1);
			*i += ft_strlen(ms->sep_set[j]);
			new->content = ft_strdup(ms->sep_set[j]);
			set_token_type(new, ms->sep_set[j]);
			*join = 0;
		}
		j++;
	}
	return (0);
}

int	is_escaped_tok(t_ms *ms, t_tokens **tokens, char *line, size_t *i, int *join)
{
	printf("\nGO IN ESCAPED\n");
	printf("current line: %s\n", &line[*i]);
	int had_to_tok = 0;
	t_tokens	*new;
	if (line[*i] == BSLASH && ft_strlen(&line[*i]) >= 2)
	{
		if (line[*i + 1] == '\n')
		{
			*i += 2;
			return (0);
		}
		if(*join == 0)
		{
			printf("create a token\n");
			new = create_token(tokens);
			if (new == NULL)
				return (1);
			new->content = ft_strnew(1);
			if (new->content == NULL)
				return (1);
			new->content[0] = line[*i + 1];
			new->type_content = ARGS;
		}
		else
		{
			printf("join to prev token\n");
			t_tokens *tmp;
			tmp = *tokens;
			while(tmp->next)
				tmp = tmp->next;
			char *tmp_token;
			tmp_token = ft_strnew(1);
			tmp_token[0] = line[*i + 1];
			tmp->content = ft_strjoin(tmp->content, tmp_token);
		}
		had_to_tok = 1;
		*i += 2;
	}
	else if (line[*i] == BSLASH)
		*i += 1;
	if(line[*i] == ' ' && had_to_tok == 1)
	{
		while(line[*i] == ' ')
			*i += 1;
		*join = 0;
	}
	else if(had_to_tok == 1)
		*join = 1;
	
	return (0);
}

int len_var(char *line)
{
	int i;

	i = 1;
	while (line[i] && !is_separator(line[i]) && !is_space(line[i])
			&& line[i] != QUOTE && line[i] != DQUOTE && line[i] != BSLASH && line[i] != '$')
		i++;
	return (i);
}

void	fill_exp_tok(t_ms *ms, char *word, char *line)
{
	size_t	j;

	j = 0;
	while (line[j] != '\0' && !is_separator(line[j]) && !is_space(line[j])
			&& line[j] != QUOTE && line[j] != DQUOTE && line[j] != BSLASH)
	{
		word[j] = line[j];
		j++;
	}
}

int	is_expand_tok(t_ms *ms, t_tokens **tokens, char *line, size_t *i, int *join)
{
	printf("\nGO IN EXPAND\n");
	printf("current line: %s\n", &line[*i]);
	t_tokens	*new;
	int len;
	if(line[*i] == '$')
	 	len = len_var(&line[*i]);
	else
		len = 0;
	 printf("test\n");
	  printf("test len : %d\n", len);
	 printf("value of line: %s\n", &line[*i]);
	  if (line[*i] == '$' && ft_strlen(&line[*i]) >= 2)
	  {
		  printf("test\n");
		if(*join == 0)
		{
			printf("create a token\n");
			new = create_token(tokens);
			if (new == NULL)
				return (1);
			new->content = ft_strnew(len);
			if (new->content == NULL)
				return (1);
			fill_exp_tok(ms, new->content, &line[*i]);
			printf("token before substitute : %s\n", new->content);
			new->content = substitute(new->content, ms);
			printf("token after substitute  : %s\n", new->content);
			new->type_content = ARGS;
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
			fill_exp_tok(ms, tmp_token, &line[*i]);
			printf("token before substitute : %s\n", tmp_token);
			tmp_token = substitute(tmp_token, ms);
			printf("token after substitute  : %s\n", tmp_token);
			tmp->content = ft_strjoin(tmp->content, tmp_token);
		}
	  }
	  printf("test len : %d\n", len);
	 *i += len;
	return (0);
}
