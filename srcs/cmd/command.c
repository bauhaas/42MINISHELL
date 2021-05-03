/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bahaas <bahaas@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/21 14:52:26 by bahaas            #+#    #+#             */
/*   Updated: 2021/05/03 15:37:38 by bahaas           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void		first_cmd_is_redir(t_ms *ms, t_cmd **cmd)
{
	int			has_redir_first;

	has_redir_first = 0;
	while (cmd && is_redir(*cmd))
	{
		select_redirection(ms, *cmd, (*cmd)->next);
		if (*cmd)
			*cmd = (*cmd)->next->next;
		has_redir_first = 1;
		if (ms->ret)
		{
			ms->last_ret = 1;
			break ;
		}
	}
	if (has_redir_first && is_type(*cmd, PIPES) && (*cmd)->next)
		*cmd = (*cmd)->next;
	reset_fd(ms);
}

int			print_cmd_error(t_ms *ms, t_cmd *cmd)
{
	if (cmd->type_link == 4 && cmd->next == NULL)
		printf("minishell: syntax error near unexpected token  « | »\n");
	else if (is_redir(cmd) && (cmd->next && (is_redir(cmd->next)
					|| is_type(cmd, PIPES))))
	{
		printf("minishell: syntax error near unexpected token");
		printf(" « %s »\n", cmd->next->content[0]);
	}
	else if ((is_pipe(cmd) || is_redir(cmd)) && cmd->prev
			&& (is_redir(cmd->prev) || is_pipe(cmd->prev)))
	{
		printf("minishell: syntax error near unexpected token");
		printf(" « %s »\n", cmd->content[0]);
	}
	else if ((is_redir(cmd)) && cmd->next == NULL)
		printf("minishell: syntax error near unexpected token  « newline »\n");
	ms->last_ret = 2;
	return (0);
}

int			check_cmd_status(t_ms *ms, t_cmd *cmd)
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
			printf("minishell: syntax error near unexpected token  « | »\n");
			return (0);
		}
		else if (ms->total_consecutive_pipes > 2)
		{
			printf("minishell: syntax error near unexpected token  « || »\n");
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

void		line_to_cmd(t_ms *ms, char *line, t_cmd *cmd)
{
	t_tokens	*head;
	t_cmd		*tmp;
	t_cmd		*to_free;

	ms->tokens = NULL;
	ms->total_consecutive_pipes = 0;
	cmd = NULL;
	parse(line, ms);
	print_tokens(ms->tokens);
	head = ms->tokens;
	while (head)
		tokens_to_cmd(ms, &cmd, &head);
	free_tokens(ms->tokens);
	print_cmd(cmd);
	to_free = cmd;
	tmp = cmd;
	if (nb_cmd(cmd) > 1 || (nb_cmd(cmd) == 1 && !get_bltn(ms, cmd->content[0])))
	{
		if (check_cmd_status(ms, cmd))
			pipeline(tmp, ms);
	}
	else
		select_execution(ms, cmd, 0);
	free_cmd(to_free);
}
