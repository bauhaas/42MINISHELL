/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bahaas <bahaas@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/20 20:09:25 by bahaas            #+#    #+#             */
/*   Updated: 2021/03/21 00:40:30 by bahaas           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void			ft_list_sort(t_list **begin_list)
{
	t_list		*actuel_node;
	t_list 		*next_node;
	t_var		*env;
	t_var		*env_next;

	actuel_node = *begin_list;
	while (actuel_node->next)
	{
		env = (t_var*)actuel_node->content;
		env_next = (t_var*)actuel_node->next->content;
		if (strcmp(env->name, env_next->name) > 0)
		{
			ft_lstswap(actuel_node,actuel_node->next);
			actuel_node = *begin_list;
		}
		else
			actuel_node = actuel_node->next;
	}
}

void			*list_sort(t_list **dest, t_list *list)
{
		t_list	*tmp;

		tmp = list;
		while (tmp)
		{
			ft_lstadd_front(dest, ft_memdup(tmp, sizeof(tmp)));
			tmp = tmp->next;
		}
		ft_list_sort(dest);
		return (dest);
}

void			free_(void *env)
{
	t_var *e;

	e = (t_var*)env;
	ft_strdel(&e->name);
	ft_strdel(&e->value);
}

void			arg_to_var(char *arg, t_ms *ms)
{
	char		**arg_split;
	t_var		*new;

	if (!arg_split)
		return ;
	if (ft_strchr(arg, '=') == NULL)
	{
		if (!valid_name(arg))
		{
			ft_putstr_fd("Minishell: export: `", STDERR);
			ft_putstr_fd(arg, STDERR);
			ft_putstr_fd("': not a valid identifier\n", STDERR);
			ms->last_ret = 1;
			return ;
		}
		if (!ft_getenv(&ms->env, arg, FALSE))
		{
			new = malloc(sizeof(t_var));
			if (new)
			{
				new->name = ft_strdup(arg);
				new->value = NULL;
				ft_lstadd_back(&ms->env,ft_lstnew(new));
			}
		}
	}
	else	
	{
		arg_split = ft_split(arg, '=');
		if (valid_name(arg_split[0]))
		{
			new = ft_get_t_var(&ms->env, arg_split[0]);
			if (!new)
			{
				new = malloc(sizeof(t_var));
				if (!new)
				{
					free_split(&arg_split);
				}
				new->name = ft_strdup(arg_split[0]);
				if (arg_split[1] != NULL)
					new->value = ft_strdup(arg_split[1]);
				else
					new->value = ft_strnew(0);
				ft_lstadd_back(&ms->env,ft_lstnew(new));
			}
			else
			{
				ft_strdel(&new->value);
				if (arg_split[1] != NULL)
					new->value = ft_strdup(arg_split[1]);
				else
					new->value = ft_strnew(0);
			}
		}
		else
		{
			ft_putstr_fd("Minishell: export: `", STDERR);
			ft_putstr_fd(arg, STDERR);
			ft_putstr_fd("': not a valid identifier\n", STDERR);
			ms->last_ret = 1;
			return ;
		}
	}
	ms->last_ret = 0;
}

int	ft_export(t_ms *ms, t_cmd *cmd)
{
	t_list 		*tmp;
	t_list		*copy;
	int			i;

	tmp = ms->env;
	copy = NULL;
	i = 1;
	if (!cmd->content[i])
	{
		list_sort(&copy, tmp);
		while(copy)
		{
			print_env((t_var *)copy->content, EXPORT);
			copy = copy->next;
		}
		ft_lstclear(&copy, &free_);
		ms->last_ret = 0;
	}
	else
	{
		while(cmd->content[i])
			arg_to_var(cmd->content[i++], ms);
	}
	return (0);
}
