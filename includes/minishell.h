/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clorin <clorin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/17 12:28:05 by clorin            #+#    #+#             */
/*   Updated: 2021/04/01 19:54:24 by bahaas           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdlib.h>
# include <stdio.h>
# include <unistd.h>
# include <term.h>
# include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
# include <signal.h>
# include <limits.h>
# include <stdbool.h>
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

# define ENV 0
# define EXPORT 1
# define FALSE 0
# define TRUE 1

# define QUOTE '\''
# define DQUOTE '\"'
# define BSLASH '\\'
# define CMD 0
# define ARGS 1
# define REDIR 3
# define PIPES 2
# define END_CMD 4

/*
typedef enum e_token_type
{
	COMMAND,
	ARGS,
	REDIRECTION,
	PIPES,
	END_COMMAND
}			t_token_type;
*/

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
	int					start_col;
	int					start_row;
	// int					end_row;
}						t_termcaps;

typedef struct			s_hist
{
	char				*line;
	struct s_hist		*prev;
	struct s_hist		*next;
}						t_hist;

typedef struct			s_tokens
{
	char *content;
	int type_content;
	int type_quote;
	struct s_tokens *prev;
	struct s_tokens *next;
}						t_tokens;

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
	char **arr_env;
	char	*pwd;
	char	*old_pwd;
	struct s_bltn	*bltn;
	struct s_cmd	*cmd;
	int i;
	char *sep_set[6];
	int		last_ret;
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
char				*substitute(char *str, t_ms *mini);

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
	int				is_pipe;
	struct s_cmd	*prev;
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
void	print_env(t_var *env, int mod);

void	init_ms(t_ms *ms, char **env);
void	ft_lstswap(t_list *prev, t_list *next);
void	line_to_cmd(t_ms *ms, char *line, t_cmd *cmd);

int		get_tokens(t_ms *ms, t_tokens **tokens, char *line);
t_tokens *create_token(t_tokens **tokens);

size_t word_len(char *line, size_t *len, char c);
void	fill_word(char *word, char *line, char c);

int		is_linked(t_ms *ms, char *line);
int		is_separator(char c);
int		is_space(char c);
int		is_escaped(char c, char *line, int i);

int		is_dquote_tok(t_ms *ms, t_tokens **tokens, char *line, size_t *i);
int		is_quote_tok(t_ms *ms, t_tokens **tokens, char *line, size_t *i);
int		is_str_tok(t_ms *ms, t_tokens **tokens, char *line, size_t *i);
int		is_sep_tok(t_ms *ms, t_tokens **tokens, char *line, size_t *i);
int		is_escaped_tok(t_ms *ms, t_tokens **tokens, char *line, size_t *i);

/*
** BUiltins folder
*/

int		ft_echo(t_ms *ms, t_cmd *cmd);
int		ft_pwd(t_ms *ms, t_cmd *cmd);
int		ft_export(t_ms *ms, t_cmd *cmd);
int		ft_unset(t_ms *ms, t_cmd *cmd);
int		ft_env(t_ms *ms, t_cmd *cmd);
int		ft_cd(t_ms *ms, t_cmd *cmd);
int		ft_exit(t_ms *ms, t_cmd *cmd);
void	free_cmd(t_cmd *cmd);

/*
** print.c
*/

void print_tokens(t_tokens *tokens);
void print_cmd(t_cmd *cmd);

/*
** tokens.c
*/

void	free_tokens(t_tokens *tokens);

/*
** utils.c
*/

void	free_split(char ***split);
char	*ft_getenv(t_list **head_ref, char *elem, int i);

#endif
