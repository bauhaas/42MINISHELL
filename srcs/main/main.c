/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bahaas <bahaas@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/17 12:52:12 by clorin            #+#    #+#             */
/*   Updated: 2021/05/06 03:54:38 by bahaas           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static int			valid_bloc(t_list **list_bloc, char *line)
{
	t_list			*bloc;
	char			*trim_bloc;

	bloc = *list_bloc;
	while (bloc)
	{
		trim_bloc = ft_strtrim((char*)bloc->content, " \t\n\v\f\r");
		if (ft_is_empty(trim_bloc))
		{
			ft_strdel(&trim_bloc);
			return (nb_semicolon(line));
		}
		else if (trim_bloc[0] == '|')
		{
			ft_strdel(&trim_bloc);
			ft_putstr_fd("minishell: ", 2);
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

	if (valid_quotes(ms->line, ft_strlen(ms->line)))
	{
		ft_putstr_fd("minishell: syntax error with open quotes\n", 2);
		return (2);
	}
	bloc = parse_bloc(ms->line);
	ms->head_bloc = bloc;
	if (!valid_bloc(&bloc, ms->line))
	{
		ft_lstclear(&ms->head_bloc, &free_list);
		return (2);
	}
	while (bloc)
	{
		ms->echo = FALSE;
		ms->head_tokens = NULL;
		ms->cmd = NULL;
		if (bloc->content && ((char*)bloc->content)[0] != 0)
			line_to_cmd(ms, (char*)bloc->content);
		bloc = bloc->next;
	}
	ft_lstclear(&ms->head_bloc, &free_list);
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
		}
	}
}

int					main(int argc, char **argv, char **envp)
{
	t_ms			ms;

	ft_bzero(&ms, sizeof(t_ms));
	init_ms(&ms, envp);
	signal(SIGINT, &sig_int);
	signal(SIGQUIT, &sig_quit);
	g_ms = &ms;
	if (argc == 1)
		prompt_loop(&ms);
	else
	{
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd(argv[1], 2);
		ft_putstr_fd(": No such file or directory\n", 2);
		free_exit(&ms);
		return (127);
	}
	return (0);
}
