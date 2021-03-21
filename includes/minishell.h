/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clorin <clorin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/17 12:28:05 by clorin            #+#    #+#             */
/*   Updated: 2021/03/21 14:58:11 by bahaas           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdlib.h>
# include <stdio.h>
# include <unistd.h>
# include "../libft/libft.h"

/*
** Env variable structure
*/
typedef	struct	s_var
{
	char *name;
	char *value;
}				t_var;

/*
** Group all the instructions sent to execute 1 command
*/

typedef struct		s_cmd
{
	char			**content;
	int 			ret_value;
	struct s_cmd	*next;
}					t_cmd;

/*
** Main structure
*/

typedef struct	s_ms
{
	t_list	*env;
	char	*pwd;
	struct s_bltn	*bltn;
}				t_ms;

/*
** Group all the builtin function pointers.
*/

typedef struct	s_bltn
{
	char	*bltn_name[8];
	int		(*bltn_cmd[8])(t_ms *ms, t_cmd *cmd);
}				t_bltn;

void	init_bltn(t_ms *ms);
int		execute(t_ms *ms, t_cmd *cmd);
char	**get_tokens(char *line);
char	*ft_getenv(t_list *env, char *elem);

void	test_builtin(t_ms *ms);

int		ft_echo(t_ms *ms, t_cmd *cmd);
int		ft_pwd(t_ms *ms, t_cmd *cmd);
int		ft_export(t_ms *ms, t_cmd *cmd);
int		ft_unset(t_ms *ms, t_cmd *cmd);
int		ft_env(t_ms *ms, t_cmd *cmd);
int		ft_cd(t_ms *ms, t_cmd *cmd);
int		ft_exit(t_ms *ms, t_cmd *cmd);
#endif
