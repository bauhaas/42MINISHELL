/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bahaas <bahaas@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/21 00:44:02 by bahaas            #+#    #+#             */
/*   Updated: 2021/04/12 19:07:37 by bahaas           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	afree_(void *env)
{
	t_var *e;

	e = (t_var*)env;
	ft_strdel(&e->name);
	ft_strdel(&e->value);
	free(e);
}

void	free_cmd(t_cmd *cmd)
{
	int	i;

	i = 0;
	while (cmd)
	{
		while (cmd->content[i])
		{
			free(cmd->content[i]);
			cmd->content[i] = NULL;
			i++;
		}
		free(cmd->content);
		cmd->content = NULL;
		cmd = cmd->next;
	}
	free(cmd);
	cmd = NULL;
}

void	free_arrstr(char **arr_env)
{
	int i;

	i = -1;
	while(arr_env[++i])
	{
		free(arr_env[i]);
		arr_env[i] = NULL;
	}
	free(arr_env);
	arr_env = NULL;
}

static int	verif_exit(t_cmd *cmd)
{
	ft_putstr_fd("exit\n", 1);
	if (cmd->content[2])
	{
		ft_putstr_fd("Minishell: exit: Too many arguments\n", 2);
		return (2);		
	}
	else if (cmd->content[1])
	{
		if (ft_is_nbr(cmd->content[1]))
			return (ft_atoi(cmd->content[1]));
		else
		{
			ft_putstr_fd("Minishell: exit: ", 2);
			ft_putstr_fd(cmd->content[1], 2);
			ft_putstr_fd(" : Numeric argument required\n", 2);
			return (2);
		}
	}
	return (0);
}

int		ft_exit(t_ms *ms, t_cmd *cmd)
{
	int	i;
	int	status;

	i = 0;
	status = verif_exit(cmd);
	while (ms->bltn->bltn_name[i])
		free(ms->bltn->bltn_name[i++]);
	i = 0;
	while (ms->sep_set[i])
	{
		free(ms->sep_set[i]);
		ms->sep_set[i] = NULL;
		i++;
	}
	free(ms->bltn);
	free(ms->line);
	free(ms->pwd);
	free(ms->old_pwd);
	free_cmd(cmd);
	free_history(&ms->cur_histo);
	ft_lstclear(&ms->env, &afree_);
	free_arrstr(ms->arr_env);
	exit(status);
	return (0);
}
