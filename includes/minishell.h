/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clorin <clorin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/17 12:28:05 by clorin            #+#    #+#             */
/*   Updated: 2021/03/22 21:55:56 by bahaas           ###   ########.fr       */
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
# include <limits.h>
# include <curses.h>
# include "../libft/libft.h"

# define STDIN 0
# define STDOUT 1
# define STDERR 2

# define EOF_KEY 4
# define BACKSPACE 127
# define CTR_L 12
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
	int					size_col;
	int					size_row;
	// int					start_col;
	// int					start_row;
	// int					end_row;
}						t_termcaps;

typedef struct			s_hist
{
	char				*line;
	struct s_hist		*prev;
	struct s_hist		*next;
}						t_hist;

/*
** Main structure
*/

typedef struct	s_ms
{
	t_hist	*history;
	t_hist	*cur_histo;
	char	*line;
	int		exit;

	t_list	*env;
	char	*pwd;
	char	*old_pwd;
	struct s_bltn	*bltn;
}				t_ms;


void				get_cursor_position(int *col, int *rows);
void				set_cursor_position(t_termcaps *tc, int col, int row);
void				keys_tree(long c, t_termcaps *tc, t_ms *mini);

void				init_termcaps(t_termcaps *tc);
void				window_size(t_termcaps *tc);
int					get_line(t_ms *mini);
int					tc_putchar(int c);
void				create_line(long c, t_termcaps *tc);
void				clear_line(t_termcaps *tc);
void				print_line(t_termcaps *tc, t_ms *ms);
void				del_char(t_termcaps *tc);
int					prompt(t_ms *ms);

/*
*	history
*/
t_hist				*add_history(t_hist **begin, char *line);
void				up_history(t_termcaps *tc, t_ms *min);
void				down_history(t_termcaps *tc, t_ms *mini);
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
char	*ft_getenv(t_list **head_ref, char *elem);

void	init_ms(t_ms *ms, char **env);
void	tmp_line_to_cmd(t_ms *ms, char *line);

int		ft_echo(t_ms *ms, t_cmd *cmd);
int		ft_pwd(t_ms *ms, t_cmd *cmd);
int		ft_export(t_ms *ms, t_cmd *cmd);
int		ft_unset(t_ms *ms, t_cmd *cmd);
int		ft_env(t_ms *ms, t_cmd *cmd);
int		ft_cd(t_ms *ms, t_cmd *cmd);
int		ft_exit(t_ms *ms, t_cmd *cmd);
#endif
