/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bahaas <bahaas@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/20 16:00:10 by bahaas            #+#    #+#             */
/*   Updated: 2021/03/29 15:43:20 by bahaas           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

/*
 ** Set our bltn structure with pointer function and names linked to it.
 */

void	init_bltn(t_ms *ms)
{
	ms->bltn = malloc(sizeof(t_bltn));
	ms->bltn->bltn_name[0] = ft_strdup("echo");
	ms->bltn->bltn_cmd[0] = &ft_echo;
	ms->bltn->bltn_name[1] = ft_strdup("cd");
	ms->bltn->bltn_cmd[1] = &ft_cd;
	ms->bltn->bltn_name[2] = ft_strdup("pwd");
	ms->bltn->bltn_cmd[2] = &ft_pwd;
	ms->bltn->bltn_name[3] =  ft_strdup("export");
	ms->bltn->bltn_cmd[3] = &ft_export;
	ms->bltn->bltn_name[4] =  ft_strdup("unset");
	ms->bltn->bltn_cmd[4] = &ft_unset;
	ms->bltn->bltn_name[5] =ft_strdup("env");
	ms->bltn->bltn_cmd[5] = &ft_env;
	ms->bltn->bltn_name[6] = ft_strdup("exit");
	ms->bltn->bltn_cmd[6] = &ft_exit;
	ms->bltn->bltn_name[7] = NULL;
	ms->bltn->bltn_cmd[7] = NULL;
}

/*
 ** Detecte if the command used is a builtin or not.
 */

int	get_bltn(t_ms *ms, char *cmd)
{
	int	i;

	i = -1;
	while (ms->bltn->bltn_name[++i])
	{
		if (!ft_strcmp(ms->bltn->bltn_name[i], cmd))
			return (1);
	}
	return (0);
}

/*
 ** Execute the builtin function and store the ret value in our struct cmd
 */

int		launch_bltn(t_ms *ms, t_cmd *cmd)
{

	int i;

	i = -1;
	while (ms->bltn->bltn_name[++i])
	{
		if (!ft_strcmp(ms->bltn->bltn_name[i], cmd->content[0]))
		{
			//printf("bltn name cmd: %s\n", ms->bltn->bltn_name[i]);
			cmd->ret_value = ms->bltn->bltn_cmd[i](ms, cmd);
			return (cmd->ret_value);
		}
	}
	return (1);
}

/*
 ** When you have filled cmd with all the instructions received. Go check
 ** if the 1st word is equivalent to a builtin or an exec that require to fork
 */

char **lst_to_arr(t_list *env)
{
	char **arr_env;
	t_list *tmp = env;
	t_var *var;
	int i = 0;
	arr_env = malloc(sizeof(char *) * ft_lstsize(tmp) + 1);
	if(!arr_env)
		return (NULL);
	while(tmp)
	{
		var = (t_var *)tmp->content;
		arr_env[i] = ft_strdup(var->name);
		arr_env[i] = ft_strjoin(arr_env[i], "=");
		arr_env[i] = ft_strjoin(arr_env[i], var->value);
		tmp = tmp->next;
		i++;
	}
	arr_env[i] = NULL;
	return (arr_env);
}
/*
**	update cmd-> ret value.
**		if not exists => 1
**		if not executable => 2
**		if directory => 3
*/

static	void		valid_file(t_cmd *cmd)
{
	struct stat        buffer;
	int                exist;

	exist = (stat(cmd->content[0], &buffer));
	if (exist != 0)
		cmd->ret_value = 1;
	else if ((buffer.st_mode & S_IXUSR) == 0)
		cmd->ret_value = 2;
	else if (S_ISDIR(buffer.st_mode) != 0)
		cmd->ret_value = 3;
	else
		cmd->ret_value = 0;
}

/*
**	This function returns TRUE if *file exists
*/
static int		file_exist(const char *file)
{
	struct stat	buffer;

	return (stat(file, &buffer) == 0);
}

/*
**	show errors when looking for executable
**	and update minishell-> exit
*/
static void		error_file(t_ms *ms, t_cmd *cmd)
{
	ft_putstr_fd("Minishell: ", STDERR);
	ft_putstr_fd(cmd->content[0], STDERR);
	ft_putstr_fd(": ", STDERR);
	if (cmd->ret_value == 1)
		ft_putstr_fd("Command not found\n", STDERR);
	else if (cmd->ret_value == 2)
		ft_putstr_fd("Permission denied\n", STDERR);
	else if (cmd->ret_value == 3)
		ft_putstr_fd("is a directory\n", STDERR);
}

/*
**	search in the PATH and update cmd->content[0]
*/
static void		find_absolute_path(t_ms *ms, t_cmd *cmd)
{
	char 		**path_to_check;
	char 		*path_env;
	char 		*program;
	int 		i;

	program = NULL;
	ms->arr_env = lst_to_arr(ms->env);
	path_env = ft_getenv(&ms->env, "PATH", 1);
	path_to_check = ft_split(path_env, ':');
	i = 0;
	while(path_to_check[i])
	{
		program = ft_strdup(path_to_check[i]);
		program = ft_strjoin(path_to_check[i], "/");
		program = ft_strjoin(program, cmd->content[0]);
		printf("path checked : %s\n", program);
		if(file_exist(program))
		{
			ft_strdel(&cmd->content[0]);
			cmd->content[0] = program;
			break;
		}
		i++;
	}
}

/*
**	search and validate the order
*/
static void		search_prog(t_ms *ms, t_cmd *cmd)
{
	char 		**path_to_check;
	char 		*path_env;
	int i;

	if (cmd->content[0][0] == '.')
		valid_file(cmd);
	else
		find_absolute_path(ms, cmd);
	valid_file(cmd);
	ms->last_ret = cmd->ret_value;
}

void fork_exec(t_ms *ms, t_cmd *cmd)
{
	int pid;
	int status;

	pid = 0;
	search_prog(ms, cmd);
	if(cmd->ret_value == 0)
	{
		 pid = fork();
		 if(pid == -1)
		 	printf("error  msg to display\n");
		 else if(pid == 0)
			 execve(cmd->content[0], cmd->content, ms->arr_env);
		 else
		 {
			 waitpid(pid, &status, 0);
			 kill(pid, SIGTERM);
		 }
	}
	else
		error_file(ms, cmd);
}

int	execute(t_ms *ms, t_cmd *cmd)
{
	while (cmd != NULL)
	{
		if (get_bltn(ms, cmd->content[0]))
			launch_bltn(ms, cmd);
		else
			fork_exec(ms, cmd);
		cmd = cmd->next;
	}
	return (0);
}
