/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokens.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bahaas <bahaas@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/20 14:29:47 by bahaas            #+#    #+#             */
/*   Updated: 2021/03/20 15:22:29 by bahaas           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

#include <string.h>
#define TOK_BUF_SIZE 100
#define TOK_DELIM " \t\r\n\a"

char **get_tokens(char *line)
{
	int buf_size = TOK_BUF_SIZE;
	int i = 0;
	char **tokens;
	char *token;

	tokens = malloc(sizeof(char *) * buf_size);
	if (!tokens)
		return (NULL);
	token = strtok(line, TOK_DELIM);
	while (token != NULL)
	{
		tokens[i] = token;
		i++;
		if (i >= buf_size)
		{
			buf_size += TOK_BUF_SIZE;
			tokens = realloc(tokens, buf_size * sizeof(char*));
			if (!tokens)
				return (NULL);
		}
		token = strtok(NULL, TOK_DELIM);
	}
	tokens[i] = NULL;
	return (tokens);
}
