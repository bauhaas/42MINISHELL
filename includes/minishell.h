/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clorin <clorin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/17 12:28:05 by clorin            #+#    #+#             */
/*   Updated: 2021/03/22 11:20:47 by bahaas           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdlib.h>
# include <stdio.h>
# include <unistd.h>
# include <term.h>
# include <sys/ioctl.h>
# include <signal.h>
# include "../libft/libft.h"

# define STDIN 0
# define STDOUT 1
# define STDERR 2

# define EOF_KEY 4
# define BACKSPACE 127
# define LEFT_ARROW 4479771
# define RIGHT_ARROW 4414235
# define UP_ARROW 4283163
# define DOWN_ARROW 4348699
# define HOME 4741915
# define END 4610843

typedef struct			s_termcaps
{
	struct termios		term;
	struct termios		old_termcaps;
	char				*cm;
	char				*ce;
	char				*dl;
	char				*line;
	int					col;
	int					row;
	int					cur_pos;
	int					start_col;
}						t_termcaps;

typedef struct			s_hist
{
	char				*line;
	struct s_hist		*prev;
	struct s_hist		*next;
}						t_hist;

typedef struct			s_mini
{
	t_hist				*history;
	t_hist				*cur_histo;
	char				*line;
	int					exit;
}						t_mini;

void				get_cursor_position(int *col, int *rows);
void				set_cursor_position(t_termcaps *tc, int col, int row);
void				keys_tree(long c, t_termcaps *tc, t_mini *mini);

void				init_termcaps(t_termcaps *tc);
int					get_line(t_mini *mini);
int					tc_putchar(int c);
void				create_line(long c, t_termcaps *tc);
void				clear_line(t_termcaps *tc);
void				print_line(t_termcaps *tc);
void				del_char(t_termcaps *tc);
void				prompt(void);

/*
*	history
*/
t_hist				*add_history(t_hist **begin, char *line);
void				up_history(t_termcaps *tc, t_mini *min);
void				down_history(t_termcaps *tc, t_mini *mini);
void				free_history(t_hist **begin);

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
