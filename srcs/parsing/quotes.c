/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quotes.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bahaas <bahaas@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/29 17:45:35 by bahaas            #+#    #+#             */
/*   Updated: 2021/03/31 13:01:00 by bahaas           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/*
 ** Check if the first char of our position in string isn't the char c, if
 ** not, there is an error matching char c. Otherswise, increment until
 ** we find the matching ' or ". If there is a closing quote, update word_len
 */

size_t	word_len(char *line, size_t *len, char c)
{
	size_t	i;

	*len = 0;
	i = 0;
	if (line[i] != c)
		return (0);
	i++;
	if(c == DQUOTE)
	{
		while (line[i] && (line[i] != c || is_escaped(line[i], line, i)))
				i++;
	}
	else if (c == QUOTE)
	{
		while (line[i] && (line[i] != c))
				i++;
	}
	if (!line[i])
		return (1);
	*len = i + 1;
	return (0);
}

/*
 ** Copy the content inside quote until we are on the closing quote. 
 ** Exception with \n we have to increment twice to skip n char.
 */

void		fill_word(char *word, char *line, char c)
{
	size_t	j;
	size_t	k;

	// decalage j k car line commence avec la quote
	j = 1;
	k = 0;
	while (line[j] && line[j] != c)
	{
		if (line[j] == BSLASH)
		{
			if (line[j + 1] == '\n')
				j++;
			else if (c == QUOTE && line[j + 1] == '\'')
				;
			else if (c == DQUOTE && line[j + 1] == '\"')
				j++;
			else
			{
				word[k] = line[j];
				k++;
				j++;
			}
		}
		word[k] = line[j];
		k++;
		j++;
	}
}

/*
 ** First, check if there is no error with quotes and set the string length.
 ** Then set a new token and fill it with the content found in quotes.
 */

//TO DO : SMASH BOTH FUNCTION IN ONE (just QUOTE/DQUOTE is modified)
int				is_dquote_tok(t_ms *ms, t_tokens **tokens, char *line, size_t *i)
{
	size_t	len;
	t_tokens	*new;

	if (word_len(&line[*i], &len, DQUOTE))
	{
		printf("single \"\n");
		return (1);
	}
	if (len == 0)
		return (0);
	new = create_token(tokens);
	if (new == NULL)
		return (1);
	new->content = ft_strnew(len);
	if (new->content == NULL)
		return (1);
	fill_word(new->content, &line[*i], DQUOTE);
	new->type_quote = DQUOTE;
	new->type_content = ARGS;
	*i += len;
	return (0);
}


int	is_quote_tok(t_ms *ms, t_tokens **tokens, char *line, size_t *i)
{
	size_t	len;
	t_tokens	*new;

	if (word_len(&line[*i], &len, QUOTE))
	{
		printf("single \'\n");
		return (1);
	}
	if (len == 0)
		return (0);
	new = create_token(tokens);
	if (new == NULL)
		return (1);
	new->content = ft_strnew(len);
	if (new->content == NULL)
		return (1);
	fill_word(new->content, &line[*i], QUOTE);
	new->type_quote = QUOTE;
	new->type_content = ARGS;
	*i += len;
	return (0);
}
