/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bahaas <bahaas@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/17 12:52:12 by clorin            #+#    #+#             */
/*   Updated: 2021/04/20 15:35:46 by bahaas           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static void			free_list(void *list)
{
	t_list			*e;

	e = (t_list*)list;
	free(e->content);
	free(e);
}

static int			valid_bloc(t_list **list_bloc)
{
	t_list			*bloc;
	char			*trim_bloc;

	bloc = *list_bloc;
	while (bloc)
	{
		trim_bloc = ft_strtrim((char*)bloc->content, " \t\n\v\f\r");
		if (ft_is_empty(trim_bloc))
		{
			ft_putstr_fd("Minishell: ", 2);
			ft_putstr_fd("syntax error near unexpected token `;'\n", 2);
			return (FALSE);
		}
		else if (trim_bloc[0] == '|')
		{
			ft_putstr_fd("Minishell: ", 2);
			ft_putstr_fd("syntax error near unexpected token `|'\n", 2);
			return (FALSE);
		}
		ft_strdel(&trim_bloc);
		bloc = bloc->next;
	}
	return (TRUE);
}

static int			line_processing(t_ms *ms)
{
	t_list			*bloc;
	t_list			*head_bloc;

	bloc = NULL;
	head_bloc = bloc;
	if (valid_quotes(ms->line, ft_strlen(ms->line)))
	{
		ft_putstr_fd("Minishell: syntax error with open quotes\n", 2);
		return (2);
	}
	bloc = parse_bloc(ms->line);
	if (!valid_bloc(&bloc))
	{
		ft_lstclear(&head_bloc, &free_list);
		return (2);
	}
	while (bloc)
	{
		ms->echo = FALSE;
		if (bloc->content && ((char*)bloc->content)[0] != 0)
			line_to_cmd(ms, (char*)bloc->content, ms->cmd);
		bloc = bloc->next;
	}
	ft_lstclear(&head_bloc, &free_list);
	return (ms->last_ret);
}

static void			prompt_loop(t_ms *ms)
{
	while (1)
	{
		ms->exit = get_line(ms);
		if (ms->line && ms->exit)
		{
			ms->last_ret = line_processing(ms);
			ft_strdel(&ms->line);
			free_cmd(ms->cmd);
		}
	}
}

int					main(int argc, char **argv, char **envp)
{
	t_ms			ms;

	(void)argv;
	ft_bzero(&ms, sizeof(t_ms));
	init_ms(&ms, envp);
	signal(SIGINT, &sig_int);
	signal(SIGQUIT, &sig_quit);
	g_ms = &ms;
	if (argc == 1)
		prompt_loop(&ms);
	else
		ft_putstr_fd("Minishell: no arguments needed\n", 2);
	return (0);
}
