/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bahaas <bahaas@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/21 14:52:26 by bahaas            #+#    #+#             */
/*   Updated: 2021/03/30 01:32:59 by bahaas           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

t_cmd		*create_cmd(t_cmd **cmd)
{
	t_cmd	*tmp;
	t_cmd	*new;

	tmp = *cmd;
	new = ft_memalloc(sizeof(t_cmd));
	if (new == NULL)
		return (NULL);
	while (tmp != NULL && tmp->next != NULL)
		tmp = tmp->next;
	if (tmp == NULL)
		*cmd = new;
	else
		tmp->next = new;
	return (new);
}

int			tokens_to_cmd(t_ms *ms, t_cmd **cmd, t_tokens *tokens)
{
	return (0);
}

void	tmp_line_to_cmd(t_ms *ms, char *line)
{
	t_tokens *tokens;
	int ret;
	t_cmd *cmd;

	tokens = NULL;
	ret = 0;
	printf("(%d)line = %s\n", ms->exit, ms->line);
	if(ret == 0)
		ret = get_tokens(ms, &tokens, line);
	int i = 0;
	printf("tokens = \n");
	t_tokens *a;
	a = tokens;
	while(a)
	{
		printf("|%s|\n", a->content);
		a = a->next;
	}
	//ret = token_to_cmd(ms, &cmd, tokens);
/*	
	cmd = malloc(sizeof(t_cmd));
	cmd->content = ft_split(line, ' ');
	int i = 0;
		printf("cmd = ");
	while(cmd->content[i])
	{
		printf("\"%s\" ", cmd->content[i]);
		i++;
	}
	printf("\n");
	cmd->next = NULL;
	cmd->ret_value = 0;
	execute(ms, cmd);
*/	
	//CHECK UPDATE PWD & OLDPWD with cd use
	/*
	printf("ms.pwd = %s\n", ms->pwd);
	printf("ms.oldpwd = %s\n", ms->old_pwd);
	char *pwd_env = ft_getenv(&ms->env, "PWD", 1);
	char *oldpwd_env = ft_getenv(&ms->env, "OLDPWD", 1);
	printf("ms->env->pwd = %s\n", pwd_env);
	printf("ms->env->oldpwd = %s\n", oldpwd_env);
	*/
}
