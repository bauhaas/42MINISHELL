/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bahaas <bahaas@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/26 11:35:35 by bahaas            #+#    #+#             */
/*   Updated: 2021/04/27 11:00:48 by bahaas           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int			multiple_redirection_error(t_ms *ms, t_cmd *prev, t_cmd *current)
{
	if(prev && is_redir(prev))
	{
		print_cmd_error(ms, current);
		return 1;
	}
	return (0);
}

int			cmd_is_redirection_instruction(t_ms *ms, t_cmd **cmd)
{
//	printf("cmd is a redi instr\n");
	t_cmd *tmp;

	tmp = (*cmd);
	if (tmp->type_link == 6)
	{
		if(multiple_redirection_error(ms, (*cmd)->prev, (*cmd)))
			return(0);
		//sinon on effectue la redir sur la prochaine cmd/arg
		tmp = (*cmd)->next;
		valid_file(tmp, 1);
	//	printf("tmp : %s\n", tmp->content[0]);
		if (tmp->ret_value == 0)
		{
		//	printf("test\n");
			if(tmp->prev->prev)
			{
			//	printf("gonna launch redir\n");
				launch_redirection(ms, tmp, tmp->prev->type_link);
			}
		}
		else
		{
			error_file(ms, tmp);
			ms->last_ret = 1;
		}
		//on se deplace maintenant sur la cmd
		//SUREMENT LA QUE CA VA PAS
		//printf("test *cmd : %s\n", (*cmd)->content[0]);
		//printf("test *cmd next : %s\n", (*cmd)->next->content[0]);
		(*cmd) = tmp;
	}
	if (tmp->type_link == 7 || tmp->type_link == 8)
	{
		if(multiple_redirection_error(ms, (*cmd)->prev, (*cmd)))
			return(0);
		tmp = (*cmd)->next;
		launch_redirection(ms, tmp, tmp->prev->type_link);
		(*cmd) = tmp;
		//(*cmd) = tmp->next;
	}
	return (1);

}

int		cmd_is_after_redirection_instruction(t_ms *ms, t_cmd **cmd)
{
	//printf("cmd is after redi inst\n");
	if ((*cmd)->prev && (*cmd)->prev->type_link != 5)
	{
		//printf("cmd : %s\n", cmd->content[0]);
		//printf("cmd : %d\n", cmd->is_env);
		//printf("cmd prev: %s\n", cmd->prev->content[0]);
		if((*cmd)->type_link == 4 && ((*cmd)->prev->type_link == 4 || (*cmd)->prev->type_link == 6 || (*cmd)->prev->type_link == 7 || (*cmd)->prev->type_link == 8))
		{
			print_cmd_error(ms, (*cmd));
			return 0;
		}
		if ((*cmd)->prev->type_link == 6)
		{
			valid_file((*cmd), 1);
			if ((*cmd)->ret_value == 0)
			{
				close_fd(ms->fdin);
				ms->fdin = open((*cmd)->content[0], O_RDONLY, 0644);
				dup2(ms->fdin, STDIN);
			}
			else
				error_file(ms, (*cmd));
		}
		else if ((*cmd)->prev->type_link == 7 || (*cmd)->prev->type_link == 8)
			launch_redirection(ms, (*cmd), (*cmd)->prev->type_link);
	}
	return 1;
}

int			select_redirection(t_ms *ms, t_cmd **cmd)
{
	printf("ENTER IN REDIRECT\n");
	if(*cmd)
		printf("with cmd : %s\n", (*cmd)->content[0]);
	if(!(*cmd))
		return (0);
	if(is_redir(*cmd))
	{

		if(cmd_is_redirection_instruction(ms, cmd) == 0)
		{
		//	printf("test\n");
			printf("SORTIE REDIR : cmd->content: %s \n", (*cmd)->content[0]);
			return (0);
		}
	}
	else if(cmd_is_after_redirection_instruction(ms, cmd) == 0)
	{
//		printf("tes redir\n");
		printf("SORTIE REDIR : cmd->content: %s \n", (*cmd)->content[0]);
		return (0);
	}
	printf("SORTIE REDIR : cmd->content: %s \n", (*cmd)->content[0]);
	return (1);
}
