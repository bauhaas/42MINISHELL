/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clorin <clorin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/17 12:28:05 by clorin            #+#    #+#             */
/*   Updated: 2021/05/05 15:52:08 by bahaas           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdlib.h>
# include <stdio.h>
# include <unistd.h>
# include <term.h>
# include <sys/ioctl.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <sys/stat.h>
# include <signal.h>
# include <limits.h>
# include <stdbool.h>
# include <fcntl.h>
# include <errno.h>
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
# define CTR_LEFT 74995417045787
# define CTR_RIGHT 73895905418011
# define HOME 4741915
# define END 4610843
# define DEL 2117294875

# define ENV 0
# define EXPORT 1
# define FALSE 0
# define TRUE 1

# define QUOTE '\''
# define DQUOTE '\"'
# define BSLASH '\\'
# define CMD_ARGS 1
# define REDIR 3
# define PIPES 4
# define LEFT 6
# define RIGHT 7
# define DRIGHT 8

int			DEBUG;
# define FREE_DEBUG 10

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
}						t_termcaps;

typedef struct			s_hist
{
	char				*line;
	struct s_hist		*prev;
	struct s_hist		*next;
}						t_hist;

typedef	struct			s_var
{
	char				*name;
	char				*value;
}						t_var;

typedef struct			s_tokens
{
	char				*content;
	int					type_content;
	int					is_env;
	struct s_tokens		*prev;
	struct s_tokens		*next;
}						t_tokens;

typedef struct			s_cmd
{
	char				**content;
	int					ret_value;
	int					type_link;
	int					is_env;
	struct s_cmd		*prev;
	struct s_cmd		*next;
}						t_cmd;

typedef struct			s_ms
{
	t_list				*env;
	t_hist				*history;
	t_hist				*cur_histo;
	t_list				*head_bloc;
	struct s_bltn		*bltn;
	t_cmd				*cmd;
	char				*line;
	char				**arr_env;
	int					exit;
	char				*pwd;
	char				*old_pwd;
	char				*sep_set[6];
	int					last_ret;
	int					in;
	int					out;
	int					fdin;
	int					fdout;
	int					signal;
	int					pid;
	int					escaped_tokens;
	int					echo;
	int					is_env;
	t_tokens			*head_tokens;
	int					ret;
	int					forked;
	int					total_consecutive_pipes;
}						t_ms;

t_ms					*g_ms;

typedef struct			s_bltn
{
	char				*bltn_name[8];
	int					(*bltn_cmd[8])(t_ms *ms, t_cmd *cmd);
}						t_bltn;

/*
**	termcaps folder
*/

/*
** cursor.c
*/

void					get_cursor_position(int *col, int *rows);
void					set_cursor_position(t_termcaps *tc, int col, int row);
void					beep();

/*
** cursor_utils.c
*/

void					left(t_termcaps *tc);
void					right(t_termcaps *tc);
void					left_word(t_termcaps *tc);
void					right_word(t_termcaps *tc);

/*
** history.c
*/

t_hist					*add_history(t_hist **begin, char *line);
void					up_history(t_termcaps *tc, t_ms *min);
void					down_history(t_termcaps *tc, t_ms *mini);
void					free_history(t_hist **begin);

/*
** keyboard.c
*/

void					keys_tree(long c, t_termcaps *tc, t_ms *mini);
void					window_size(t_termcaps *tc);

/*
** print_utils.c
*/

void					create_line(long c, t_termcaps *tc);
void					print_line(t_termcaps *tc, t_ms *ms);

/*
** prompt.c
*/

int						prompt(t_ms *ms);
/*
** termcaps.c
*/

int						get_line(t_ms *mini);

/*
** termcaps_utils.c
*/

void					clear_line(void);
void					cls(t_termcaps *tc);
void					free_termcaps(t_termcaps *tc);
int						tc_putchar(int c);

/*
**	parsing folder
*/

/*
**	expansion.c
*/

char					*value(t_ms *mini, char *str, int *i);

/*
**	parse_bloc.c
*/

int						nb_semicolon(char *str);
t_list					*parse_bloc(char *str);

/*
**	parser.c
*/

void					parse(char *str, t_ms *ms);

/*
**	parser_utils.c
*/

int						escaped(char *str, int pos);
int						back_slash(t_ms *ms, char *str, char **word, int i);
int						special(t_ms *ms, char *str, char **word, int i);
int						quote(char *str, char **word, int i, int q);

/*
**	utils.c
*/

int						is_spec_car(char c);
int						valid_quotes(const char *str, int len);
int						valid_name(char *name);
char					*get_name_var(char *str);

/*
** tokens.c
*/

void					free_tokens(t_tokens *tokens);
void					create_token(t_ms *ms, char **word);
void					fill_token(t_tokens **new, t_ms *ms, char **word);
int						set_token_type(char *word_list);

void					print_tokens(t_tokens *tokens);
void					print_cmd(t_cmd *cmd);

/*
** builtins folder
*/

/*
** builtins.c
*/

void					init_bltn(t_ms *ms);
int						get_bltn(t_ms *ms, char *cmd);
int						launch_bltn(t_ms *ms, t_cmd *cmd);

/*
** echo.c
*/

int						ft_echo(t_ms *ms, t_cmd *cmd);

/*
** env.c
*/

void					free_env(void *env);
void					print_env(t_var *env, int mod);
void					ft_lstswap(t_list *prev, t_list *next);
int						ft_env(t_ms *ms, t_cmd *cmd);

/*
** pwd.c
*/

int						ft_pwd(t_ms *ms, t_cmd *cmd);

/*
** unset.c
*/

int						ft_unset(t_ms *ms, t_cmd *cmd);

/*
** export.c
*/

int						ft_export(t_ms *ms, t_cmd *cmd);

/*
** export_utils.c
*/

void					*list_sort(t_list **dest, t_list *list);

/*
** cd.c
*/

int						ft_cd(t_ms *ms, t_cmd *cmd);

/*
** cd_error.c
*/

int						error_no_file(char *str);
void					error_getcwd(t_ms *ms, char *new_loc);

/*
** exit.c
*/

int						ft_exit(t_ms *ms, t_cmd *cmd);
void					free_cmd(t_cmd *cmd);
void					free_list(void *list);

/*
** exec folder
*/

/*
** execution.c
*/

void					pipeline(t_cmd *cmd, t_ms *ms);

/*
** file.c
*/

int						search_prog(t_ms *ms, t_cmd *cmd);

/*
**	fork.c
*/

void					parent_execution(int *fdd, int *fd);
void					child_execution(t_ms *ms, t_cmd **cmd, int fdd,
						int *fd);
void					select_execution(t_ms *ms, t_cmd *cmd,
						int exit_in_pipeline);
void					fork_error(void);

/*
**	redirection.c
*/

void					set_redirection(t_ms *ms, t_cmd *cmd);
void					select_redirection(t_ms *ms, t_cmd *redir_cmd,
						t_cmd *file_to_redirect);

/*
**	fd.c
*/

void					ft_close(int fd);
void					reset_fd(t_ms *ms);

/*
**	signal.c
*/

void					sig_int(int code);
void					sig_quit(int code);
void					ctr_c(t_termcaps *tc, t_ms *ms);

/*
**	types.c
*/

int						is_redir(t_cmd *cmd);
int						is_pipe(t_cmd *cmd);
int						is_type(t_cmd *cmd, int type);


/*
** tokens.c
*/

void					free_tokens(t_tokens *tokens);
void					create_token(t_ms *ms, char **word);
void					fill_token(t_tokens **new, t_ms *ms, char **word);
int						set_token_type(char *word_list);


/*
**	cmd folder
*/

/*
**	command.c
*/

void					line_to_cmd(t_ms *ms, char *line);

/*
**	tokens_to_cmd.c
*/

void					tokens_to_cmd(t_ms *ms, t_tokens **tokens);

/*
**	fill_command.c
*/

void					fill_arg_cmd(t_cmd **new, t_tokens **tokens);
void					fill_sep_cmd(t_cmd **new, t_tokens **tokens);

/*
** main folder
*/

/*
** init.c
*/

void					init_ms(t_ms *ms, char **env);

/*
** init_env.c
*/

char					**lst_to_arr(t_list *env);
void					init_lstenv(t_ms *ms, t_list **lst_env, char **env);
void					free_arrstr(char **arr_env);

/*
** utils.c
*/

void					free_split(char ***split);
char					*ft_getenv(t_list **head_ref, char *elem, int i);
t_var					*ft_get_t_var(t_list **head_ref, char *elem);

#endif
