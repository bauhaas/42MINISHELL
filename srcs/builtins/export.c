/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bahaas <bahaas@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/20 20:09:25 by bahaas            #+#    #+#             */
/*   Updated: 2021/04/23 03:49:03 by bahaas           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static int	has_not_egal(char *arg, t_ms *ms)
{
	t_var	*new;

	if (!valid_name(arg))
	{
		ft_putstr_fd("Minishell: export: '", STDERR);
		ft_putstr_fd(arg, STDERR);
		ft_putstr_fd("': not a valid identifier\n", STDERR);
		return (1);
	}
	if (!ft_getenv(&ms->env, arg, FALSE))
	{
		new = malloc(sizeof(t_var));
		if (new)
		{
			new->name = ft_strdup(arg);
			new->value = NULL;
			ft_lstadd_back(&ms->env, ft_lstnew(new));
		}
		else
			return (1);
	}
	return (0);
}

static int	export_ok(char **arg_split, t_ms *ms)
{
	t_var	*new;

	new = ft_get_t_var(&ms->env, arg_split[0]);
	if (!new)
	{
		new = malloc(sizeof(t_var));
		if (!new)
			return (1);
		new->name = ft_strdup(arg_split[0]);
		if (arg_split[1] != NULL)
			new->value = ft_strdup(arg_split[1]);
		else
			new->value = ft_strnew(0);
		ft_lstadd_back(&ms->env, ft_lstnew(new));
	}
	else
	{
		ft_strdel(&new->value);
		if (arg_split[1] != NULL)
			new->value = ft_strdup(arg_split[1]);
		else
			new->value = ft_strnew(0);
	}
	return (0);
}

static int	arg_to_var(char *arg, t_ms *ms)
{
	char	**arg_split;
	int		ret;

	if (ft_strchr(arg, '='))
	{
		arg_split = ft_nsplit(arg, '=', 1);
		if (arg[0] != '=' && valid_name(arg_split[0]))
		{
			ret = export_ok(arg_split, ms);
			free_split(&arg_split);
			return (ret);
		}
		else
		{
			ft_putstr_fd("Minishell: export: `", STDERR);
			ft_putstr_fd(arg, STDERR);
			ft_putstr_fd("': not a valid identifier\n", STDERR);
			return (1);
		}
	}
	else
		return (has_not_egal(arg, ms));
}

int			ft_export(t_ms *ms, t_cmd *cmd)
{
	t_list	*tmp;
	t_list	*copy;
	int		i;
	int		status;

	tmp = ms->env;
	copy = NULL;
	i = 1;
	status = 0;
	if (!cmd->content[i] || cmd->content[i][0] == '\0')
	{
		list_sort(&copy, tmp);
		while (copy)
		{
			print_env((t_var *)copy->content, EXPORT);
			copy = copy->next;
		}
		ft_lstclear(&copy, &free_env);
		return (status);
	}
	else
		while (cmd->content[i])
			status |= arg_to_var(cmd->content[i++], ms);
	return (status);
}
