/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bahaas <bahaas@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/21 14:52:26 by bahaas            #+#    #+#             */
/*   Updated: 2021/05/06 03:49:32 by bahaas           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static int	print_cmd_error(t_ms *ms, t_cmd *cmd)
{
	if (cmd->type_link == 4 && cmd->next == NULL)
		ft_putstr_fd("minishell: syntax error near unexpected token  « | »\n"
			, 2);
	else if (is_redir(cmd) && (cmd->next && (is_redir(cmd->next)
					|| is_type(cmd, PIPES))))
	{
		ft_putstr_fd("minishell: syntax error near unexpected token « ", 2);
		ft_putstr_fd(cmd->next->content[0], 2);
		ft_putstr_fd(" »\n", 2);
	}
	else if ((is_pipe(cmd) || is_redir(cmd)) && cmd->prev
			&& (is_redir(cmd->prev) || is_pipe(cmd->prev)))
	{
		ft_putstr_fd("minishell: syntax error near unexpected token « ", 2);
		ft_putstr_fd(cmd->content[0], 2);
		ft_putstr_fd(" »\n", 2);
	}
	else if ((is_redir(cmd)) && cmd->next == NULL)
	{
		ft_putstr_fd("minishell: syntax error near unexpected token ", 2);
		ft_putstr_fd("« newline »\n", 2);
	}
	ms->last_ret = 2;
	return (0);
}

static int	check_cmd_status(t_ms *ms, t_cmd *cmd)
{
	t_cmd *tmp;

	tmp = cmd;
	while (tmp)
	{
		if (((is_pipe(tmp) || is_redir(tmp)) && tmp->next == NULL))
			return (print_cmd_error(ms, tmp));
		if (is_redir(tmp) && (tmp->next && is_redir(tmp->next)))
			return (print_cmd_error(ms, tmp));
		else if (ms->total_consecutive_pipes == 2)
		{
			ft_putstr_fd("minishell: ", 2);
			ft_putstr_fd("syntax error near unexpected token « | »\n", 2);
			return (0);
		}
		else if (ms->total_consecutive_pipes > 2)
		{
			ft_putstr_fd("minishell: ", 2);
			ft_putstr_fd("syntax error near unexpected token « || »\n", 2);
			return (0);
		}
		tmp = tmp->next;
	}
	return (1);
}

static int	nb_cmd(t_cmd *cmd)
{
	t_cmd	*tmp;
	int		i;

	tmp = cmd;
	i = 0;
	while (tmp)
	{
		if (tmp->type_link != 4)
			i++;
		tmp = tmp->next;
	}
	return (i);
}

void		line_to_cmd(t_ms *ms, char *line)
{
	t_tokens	*token;
	t_cmd		*to_free;

	ms->total_consecutive_pipes = 0;
	parse(line, ms, 0);
	token = ms->head_tokens;
	while (token)
		tokens_to_cmd(ms, &token);
	free_tokens(ms->head_tokens);
	to_free = ms->cmd;
	if (nb_cmd(ms->cmd) > 1 || (nb_cmd(ms->cmd) == 1
				&& !get_bltn(ms, ms->cmd->content[0])))
	{
		if (check_cmd_status(ms, ms->cmd))
			pipeline(ms->cmd, ms);
	}
	else
	{
		if (check_cmd_status(ms, ms->cmd))
			select_execution(ms, ms->cmd, 0);
	}
	free_cmd(to_free);
}
