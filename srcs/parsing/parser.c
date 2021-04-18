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

static int			back_slash(char *str, char **word, int i)
{
	if (valid_quotes(str, i) == 0)
	{
		i++;
		*word = ft_add_char(*word, str[i]);
		if (str[i])
			i++;
	}
	else if (valid_quotes(str, i) == DQUOTE)
	{
		i++;
		if (str[i] != '$' && str[i] != '\\' && str[i] != '"')
			*word = ft_add_char(*word, '\\');
		*word = ft_add_char(*word, str[i++]);
	}
	else
		*word = ft_add_char(*word, str[i++]);
	return (i);
}

static	void		new_token(t_tokens **tokens, char **word)
{
	t_tokens		*new;

	if (*word)
	{
		new = create_token(tokens);
		if (new == NULL)
			return ;
		new->content = ft_strdup(*word);
		new->type_content = set_token_type(*word, NULL);
		ft_strdel(word);
	}
}

static int			dbl_quote(char *str, char **word, int i)
{
	if (valid_quotes(str, i) == QUOTE)
		*word = ft_add_char(*word, str[i]);
	else
		*word = ft_add_char(*word, '\0');
	i++;
	return (i);
}

static int			simple_quote(char *str, char **word, int i)
{
	if (valid_quotes(str, i) == DQUOTE)
		*word = ft_add_char(*word, str[i]);
	else
		*word = ft_add_char(*word, '\0');
	i++;
	return (i);
}

static int			special(char *str, char **word, int i, t_tokens **tokens)
{
	if (valid_quotes(str, i) == 0)
	{
		new_token(tokens, word);
		//ft_strdel(word);
		if (str[i] == '>')
		{
			if (str[i + 1] == '>')
			{
				*word = ft_strdup(">>");
				i++;
			}
			else
				*word = ft_strdup(">");
		}
		else if (str[i] != '\t' && str[i] != ' ')
			*word = ft_add_char(*word, str[i]);
		new_token(tokens, word);
		//ft_strdel(word);
	}
	else
		*word = ft_add_char(*word, str[i]);
	i++;
	return (i);
}

static char			**expansion(char *str, char **word, t_ms *mini, int *i)
{
	int j;
	int k = 0;
	char **split_word;

	j = *i;
	split_word = NULL;
	*word = ft_add_str(*word, value(mini, str + *i, i));
	if (!(!ft_strchr(*word, ' ') || valid_quotes(str, j) || (j > 0 && str[j - 1] == '=')))
	{
		split_word = ft_split(*word, ' ');
		return (split_word);
	}
	return (NULL);
}

static int			parse3(char *str, int i, char **word, t_tokens **tokkens)
{
	if (str[i] == '\\')
		i = back_slash(str, word, i);
	else if (str[i] == ' ' || str[i] == '\t' || str[i] == ';'
	|| str[i] == '>' || str[i] == '|' || str[i] == '<')
		i = special(str, word, i, tokkens);
	else if (str[i] == '"')
		i = dbl_quote(str, word, i);
	else if (str[i] == '\'')
		i = simple_quote(str, word, i);
	return (i);
}

void				parse(char *str, t_ms *mini, t_tokens **tokens)
{
	char 			**split_word;
	char			*word;
	int				i;

	i = 0;
	word = NULL;
	while (str[i])
	{
		if (!is_spec_car(str[i]) || valid_quotes(str, i) == QUOTE)
			word = ft_add_char(word, str[i++]);
		else if (str[i] == '\\' || str[i] == ' ' || str[i] == '\t' ||
			str[i] == ';' || str[i] == '>' || str[i] == '|' || str[i] == '<'
			|| str[i] == '"' || str[i] == '\'')
			i = parse3(str, i, &word, tokens);
		else if (str[i] == '$' && valid_quotes(str, i) != QUOTE)
		{
			if((split_word = expansion(str, &word, mini, &i)))
			{
				while (*split_word)
					new_token(tokens, (split_word)++);
				ft_strdel(&word);
			}
		}
	}
	new_token(tokens, &word);
}

void				parse2(char *str, t_ms *mini, t_tokens **tokens)
{
	char 			**split_word;
	char			*word;
	int				i;

	i = 0;
	word = NULL;
	while (str[i])
	{
		if (!is_spec_car(str[i]))
			word = ft_add_char(word, str[i++]);
		else
		{
			if (str[i] == '\\')
				i = back_slash(str, &word, i);
			else if (str[i] == ' ' || str[i] == '\t' || str[i] == ';'
				|| str[i] == '>' || str[i] == '|' || str[i] == '<')
				i = special(str, &word, i, tokens);
			else if (str[i] == '"')
				i = dbl_quote(str, &word, i);
			else if (str[i] == '\'')
				i = simple_quote(str, &word, i);
			else if (str[i] == '$' && valid_quotes(str, i) != QUOTE)
			{
				if((split_word = expansion(str, &word, mini, &i)))
				{
					while (*split_word)
						new_token(tokens, (split_word)++);
					ft_strdel(&word);
				}
			}
			else
				word = ft_add_char(word, str[i++]);
		}
	}
	new_token(tokens, &word);
}
