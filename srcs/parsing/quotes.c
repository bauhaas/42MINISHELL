/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quotes.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bahaas <bahaas@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/29 17:45:35 by bahaas            #+#    #+#             */
/*   Updated: 2021/04/09 15:51:46 by bahaas           ###   ########.fr       */
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
	printf("WE FIND TOKEN LENGTH\n");
	*len = 0;
	i = 0;
	if (line[i] != c)
		return (0);
	i++;
	printf("Our actual line: %s\n", &line[i]);
	if(c == DQUOTE)
	{
		//OLD TECHNIQUE
		/*
		   while ((line[i]) && (line[i] != c || is_escaped(line[i], line, i)))
		   {
				i++;
				*len += 1;
			}
		*/
		while (line[i] && line[i] != DQUOTE)
		{
			if(line[i] == '\\' && (line[i + 1] == '\\' || line[i + 1] == '$'))
			{
				*len += 1;
				i += 2;
			}
			else if(line[i] == '\"' && !line[i + 1])
				break;
			else
			{
				*len += 1;
				i++;
			}
		}
	}
	else if (c == QUOTE)
	{
		while (line[i] && (line[i] != c))
		{
			*len += 1;
			i++;
		}
	}
	printf("Our line at the end of TOKEN LENGTH CALCUL: %s\n", &line[i]);
	if (!line[i])
		return (1);
	return (0);
}

/*
 ** Copy the content inside quote until we are on the closing quote. 
 ** Exception with \n we have to increment twice to skip n char.
 */

void		fill_word(char *word, char *line, char c, size_t *i)
{
	size_t	j;
	size_t	k;

	// decalage j k car line commence avec la quote
	j = 1;
	k = 0;
	printf("WE FIND TOKEN LENGTH\n");
	printf("Our actual line: %s\n", &line[*i]);
	while (line[j] && line[j] != c)
	{
		if (line[j] == BSLASH)
		{
			if (c == QUOTE && line[j + 1] == '\'')
				;
			if (c == DQUOTE && line[j + 1] == '\"')
				j++;	
			else if (c == DQUOTE && line[j + 1] == '\\')
				j++;
			else if (c == DQUOTE && line[j + 1] == '$')
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
	//if(line[j])
	//	*i += 1;
	printf("Our line at the end of TOKEN FILLING: %s\n", &line[*i]);
}

/*
 ** First, check if there is no error with quotes and set the string length.
 ** Then set a new token and fill it with the content found in quotes.
 */

int alen_var(char *line)
{
	int i;

	i = 1;
	while (line[i] && !is_separator(line[i]) && !is_space(line[i])
			&& line[i] != QUOTE && line[i] != DQUOTE && line[i] != BSLASH && line[i] != '$')
		i++;
	return (i);
}

int				is_dquote_tok(t_ms *ms, t_tokens **tokens, char *line, size_t *i, int *join)
{

	printf("\nGO IN DQUOTE\n");
	printf("Current line : %s\n", &line[*i]);
	size_t	len;
	t_tokens	*new;
	if (word_len(&line[*i], &len, DQUOTE))
	{
		printf("\033[1m\033[31msingle \" DONT NEED TO EXECUTE\n\033[0m");
		return(1);
	}
	if (len == 0)
		return (0);
	//SI PAS BESOIN DE JOIN AVEC LE TOKEN PRECEDENT
	if(*join == 0)
	{
		printf("create a token\n");
		new = create_token(tokens);
		if (new == NULL)
			return (1);
		new->content = ft_strnew(len);
		if (new->content == NULL)
			return (1);
		printf("Length malloced for the token %zu\n", len);
		fill_word(new->content, &line[*i], DQUOTE, i);
		printf("Content of the token after fill %s\n", new->content);
		int j = 0;
		int lenvar = 0;
		while(new->content[j])
		{
			if (new->content[j] == '$' && new->content[j - 1] != '\\' && ft_strlen(&new->content[j]) >= 2)
				//if (new->content[j] == '$' && ft_strlen(&new->content[j]) >= 2)
			{
				lenvar = alen_var(&new->content[j]);
				printf("token before substitute : %s\n", new->content);
				new->content = substitute(new->content, ms);
				printf("token after substitute  : %s\n", new->content);
				j += lenvar;
			}
			else
				j++;
		}
		new->type_quote = DQUOTE;
		new->type_content = ARGS;
	}
	//SI BESOIN DE JOIN AVEC TOKEN
	else
	{
		printf("join to prev token\n");
		t_tokens *tmp;
		tmp = *tokens;
		while(tmp->next)
			tmp = tmp->next;
		char *tmp_token;
		tmp_token = ft_strnew(len);
		fill_word(tmp_token, &line[*i], DQUOTE, i);
		tmp->content = ft_strjoin(tmp->content, tmp_token);
	}
	if(line[*i])
		*i += len + 2;
	//SKIP SPACE FOR NEXT TOKEN
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


int	is_quote_tok(t_ms *ms, t_tokens **tokens, char *line, size_t *i, int *join)
{

	printf("\nGO IN QUOTE\n");
	printf("current line : %s\n", &line[*i]);
	size_t	len;
	t_tokens	*new;

	if (word_len(&line[*i], &len, QUOTE))
	{
		printf("single \'\n");
		return (1);
	}
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
		fill_word(new->content, &line[*i], QUOTE, i);
		new->type_quote = QUOTE;
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
		fill_word(tmp_token, &line[*i], QUOTE, i);
		tmp->content = ft_strjoin(tmp->content, tmp_token);
	}
	*i += len + 2;
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
