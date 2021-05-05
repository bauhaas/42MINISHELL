/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bahaas <bahaas@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/21 00:44:02 by bahaas            #+#    #+#             */
/*   Updated: 2021/05/05 15:10:45 by bahaas           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void			free_list(void *content)
{
	char	*e;

	e = (char *)content;
	ft_strdel(&e);
}

void			free_cmd(t_cmd *cmd)
{
	t_cmd		*ptr_list;
	t_cmd		*ptr_next;
	int			i;

	ptr_list = cmd;
	while (ptr_list)
	{
		i = 0;
		ptr_next = ptr_list->next;
		while (ptr_list->content[i])
		{
			ft_strdel(&ptr_list->content[i]);
			i++;
		}
		free(ptr_list->content);
		free(ptr_list);
		ptr_list = ptr_next;
	}
	cmd = NULL;
}

void			free_arrstr(char **arr_env)
{
	int			i;

	i = -1;
	while (arr_env[++i])
	{
		free(arr_env[i]);
		arr_env[i] = NULL;
	}
	free(arr_env);
	arr_env = NULL;
}

static int		valid_exit(t_cmd *cmd, int last_ret)
{
	int			ret;

	ret = last_ret;
	if (cmd->content[1])
	{
		if (!ft_is_nbr(cmd->content[1]))
		{
			ft_putstr_fd("minishell: exit: ", 2);
			ft_putstr_fd(cmd->content[1], 2);
			ft_putstr_fd(" : Numeric argument required\n", 2);
			ret = 2;
		}
		else
			ret = ft_atoi(cmd->content[1]);
	}
	return (ret);
}

static void		free_exit(t_ms *ms, t_cmd *cmd)
{
	int			i;

	i = 0;
	while (ms->bltn->bltn_name[i])
		free(ms->bltn->bltn_name[i++]);
	i = 0;
	while (ms->sep_set[i])
	{
		free(ms->sep_set[i]);
		ms->sep_set[i++] = NULL;
	}
	free(ms->bltn);
	free(ms->line);
	free(ms->pwd);
	free(ms->old_pwd);
	free_cmd(cmd);
	free_history(&ms->cur_histo);
	ft_lstclear(&ms->env, &free_env);
	free_arrstr(ms->arr_env);
	ft_lstclear(&ms->head_bloc, &free_list);
}

int				ft_exit(t_ms *ms, t_cmd *cmd)
{
	int			status;

	if (cmd->prev && !ft_strcmp(cmd->prev->content[0], "|"))
		;
	else
		ft_putstr_fd("exit\n", 1);
	if (cmd->next && cmd->next->content[0])
		return (0);
	if (cmd->content[1] && ft_is_nbr(cmd->content[1]) && cmd->content[2])
	{
		ft_putstr_fd("Minishell: exit: Too many arguments\n", 2);
		return (1);
	}
	status = valid_exit(cmd, ms->last_ret);
	free_exit(ms, cmd);
	exit(status);
	return (0);
}
