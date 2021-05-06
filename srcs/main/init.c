/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clorin <clorin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/17 12:52:12 by clorin            #+#    #+#             */
/*   Updated: 2021/05/05 15:12:09 by bahaas           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static void		init_sep(t_ms *ms)
{
	ms->sep_set[0] = ft_strdup(";");
	ms->sep_set[1] = ft_strdup(">");
	ms->sep_set[2] = ft_strdup(">>");
	ms->sep_set[3] = ft_strdup("<");
	ms->sep_set[4] = ft_strdup("|");
	ms->sep_set[5] = NULL;
}

static void		upgrade_shlvl(t_ms *ms)
{
	int		old_shlvl;
	t_var	*shlvl;

	shlvl = ft_get_t_var(&ms->env, "SHLVL");
	if (shlvl)
	{
		old_shlvl = ft_atoi(shlvl->value);
		ft_strdel(&shlvl->value);
		shlvl->value = ft_itoa(old_shlvl + 1);
	}
	else
	{
		shlvl = malloc(sizeof(t_var));
		if (!shlvl)
			return ;
		shlvl->name = ft_strdup("SHLVL");
		shlvl->value = ft_strdup("1");
		ft_lstadd_back(&ms->env, ft_lstnew(shlvl));
	}
}

static void		create_var(t_ms *ms, char *name, char *value)
{
	t_var	*new;

	new = (t_var*)malloc(sizeof(t_var));
	if (new)
	{
		new->name = ft_strdup(name);
		new->value = ft_strdup(value);
		ft_lstadd_back(&ms->env, ft_lstnew(new));
	}
}

static void		init_pwd(t_ms *ms)
{
	char *tmp;

	tmp = NULL;
	tmp = getcwd(tmp, sizeof(char) * 2048);
	if (!ft_getenv(&ms->env, "PWD", 1))
	{
		ft_strdel(&ms->pwd);
		ms->pwd = ft_strdup(tmp);
		create_var(ms, "PWD", ms->pwd);
		ft_strclr(tmp);
	}
	if (!ft_getenv(&ms->env, "OLDPWD", 1))
	{
		ft_strdel(&ms->old_pwd);
		ms->old_pwd = ft_strdup(tmp);
		create_var(ms, "OLDPWD", ms->old_pwd);
	}
	ft_strdel(&tmp);
}

void			init_ms(t_ms *ms, char **env)
{
	init_lstenv(ms, &ms->env, env);
	init_bltn(ms);
	init_sep(ms);
	ms->last_ret = 0;
	ms->signal = 0;
	ms->cmd = NULL;
	ms->exit = 1;
	ms->in = dup(STDIN);
	ms->out = dup(STDOUT);
	ms->arr_env = lst_to_arr(ms->env);
	ms->echo = FALSE;
	ms->head_bloc = NULL;
	upgrade_shlvl(ms);
	init_pwd(ms);
}
