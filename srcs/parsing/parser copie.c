/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clorin <clorin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/17 16:05:15 by clorin            #+#    #+#             */
/*   Updated: 2021/04/17 16:05:40 by clorin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void				parse(char *str, t_ms *mini, t_tokens **tokens)
{
	t_tokens		*new;
	char			*word;
	int				i;

	if (!str)
		return ;
	if (valid_quotes(str, ft_strlen(str)) > 0)
	{
		printf("minishell: syntax error with open quotes\n");
		return ;
	}
	i = 0;
	word = NULL;
	while (str[i])
	{
		if (!is_spec_car(str[i]))
		{
			word = ft_add_char(word, str[i]);
			i++;
		}
		else
		{
			if (str[i] == '\\')
			{
				if (valid_quotes(str, i) == 0)
				{
					i++;
					word = ft_add_char(word, str[i]);
					if (str[i])
						i++;
				}
				else if (valid_quotes(str, i) == DQUOTE)
				{
					i++;
					if (str[i] != '$' && str[i] != '\\' && str[i] != '"')
						word = ft_add_char(word, '\\');
					word = ft_add_char(word, str[i]);
					i++;
				}
				else
				{
					word = ft_add_char(word, str[i]);
					i++;
				}
			}
			else if (str[i] == ' ' || str[i] == '\t' || str[i] == ';'
				|| str[i] == '>' || str[i] == '|' || str[i] == '<')
			{
				if (valid_quotes(str, i) == 0)
				{
					if (word)
					{
						new = create_token(tokens);
						if (new == NULL)
							return ;
						new->content = ft_strdup(word);
						new->type_content = CMD_ARGS;
					}
					ft_strdel(&word);
					if (str[i] == '>')
					{
						if (str[i + 1] == '>')
						{
							word = ft_strdup(">>");
							i++;
						}
						else
							word = ft_strdup(">");
					}
					else if (str[i] != '\t' && str[i] != ' ')
						word = ft_add_char(word, str[i]);
					if (word)
					{
						new = create_token(tokens);
						if (new == NULL)
							return ;
						new->content = ft_strdup(word);
						new->type_content = set_token_type(word, new);
					}
					ft_strdel(&word);
				}
				else
					word = ft_add_char(word, str[i]);
				i++;
			}
			else if (str[i] == '"')
			{
				if (valid_quotes(str, i) == QUOTE)
					word = ft_add_char(word, str[i]);
				else
					word = ft_add_char(word, '\0');
				i++;
			}
			else if (str[i] == '\'')
			{
				if (valid_quotes(str, i) == DQUOTE)
					word = ft_add_char(word, str[i]);
				else
					word = ft_add_char(word, '\0');
				i++;
			}
			else if (str[i] == '$' && valid_quotes(str, i) != QUOTE)
			{
				int j;
				j = i;
				word = ft_add_str(word, value(mini, str + i, &i));
				if (!(!ft_strchr(word, ' ') || valid_quotes(str, j) || (j > 0 && str[j - 1] == '=')))
				{
					char **split_word;
					split_word = ft_split(word, ' ');
					int k = 0;
					while (split_word[k])
					{
						new = create_token(tokens);
						if (new == NULL)
							return ;
						new->content = ft_strdup(split_word[k]);
						new->type_content = CMD_ARGS;
						k++;
					}
					free_split(&split_word);
					ft_strdel(&word);
				}
			}
			else
			{
				word = ft_add_char(word, str[i]);
				i++;
			}
		}
	}
	if (word)
	{
		new = create_token(tokens);
		if (new == NULL)
			return ;
		new->content = ft_strdup(word);
		new->type_content = CMD_ARGS;
	}
	ft_strdel(&word);
}
