/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clorin <clorin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/17 12:28:05 by clorin            #+#    #+#             */
/*   Updated: 2021/04/22 11:48:20 by bahaas           ###   ########.fr       */
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
# define END_CMD 5
# define LEFT 6
# define RIGHT 7
# define DRIGHT 8

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
	struct s_bltn		*bltn;
	t_cmd				*cmd;
	char				*line;
	char				**arr_env;
	int					exit;
	char				*pwd;
	char				*old_pwd;
	char				*sep_set[6];
	int					last_ret;
	int					pipin;
	int					pipout;
	int					recursive;
	int					in;
	int					out;
	int					fdin;
	int					fdout;
	int					flag;
	int					redir_status;
	int					signal;
	int					pid;
	int					escaped_tokens;
	int					is_env;
	t_tokens			*tokens;
}						t_ms;

t_ms					*g_ms;

typedef struct			s_bltn
{
	char				*bltn_name[8];
	int					(*bltn_cmd[8])(t_ms *ms, t_cmd *cmd);
}						t_bltn;

/*
**	Signal
*/

void					sig_int(int code);
void					sig_quit(int code);
void					ctr_c(t_termcaps *tc, t_ms *ms);

/*
**	Termcaps
*/
void					get_cursor_position(int *col, int *rows);
void					set_cursor_position(t_termcaps *tc, int col, int row);
void					keys_tree(long c, t_termcaps *tc, t_ms *mini);
void					window_size(t_termcaps *tc);
int						get_line(t_ms *mini);
int						tc_putchar(int c);
void					create_line(long c, t_termcaps *tc);
void					clear_line(void);
void					cls(t_termcaps *tc);
void					print_line(t_termcaps *tc, t_ms *ms);
int						prompt(t_ms *ms);
void					left(t_termcaps *tc);
void					right(t_termcaps *tc);
void					left_word(t_termcaps *tc);
void					right_word(t_termcaps *tc);
void					free_termcaps(t_termcaps *tc);
void					beep();

/*
**	history
*/

t_hist					*add_history(t_hist **begin, char *line);
void					up_history(t_termcaps *tc, t_ms *min);
void					down_history(t_termcaps *tc, t_ms *mini);
void					free_history(t_hist **begin);
void					init_bltn(t_ms *ms);
int						execute(t_ms *ms, t_cmd *cmd);
void					print_env(t_var *env, int mod);
void					init_ms(t_ms *ms, char **env);
void					ft_lstswap(t_list *prev, t_list *next);
void					line_to_cmd(t_ms *ms, char *line, t_cmd *cmd);
t_tokens				*create_token(t_tokens **tokens);

/*
** BUiltins folder
*/

int						ft_echo(t_ms *ms, t_cmd *cmd);
int						ft_pwd(t_ms *ms, t_cmd *cmd);
int						ft_export(t_ms *ms, t_cmd *cmd);
int						ft_unset(t_ms *ms, t_cmd *cmd);
int						ft_env(t_ms *ms, t_cmd *cmd);
int						ft_cd(t_ms *ms, t_cmd *cmd);
int						ft_exit(t_ms *ms, t_cmd *cmd);
void					free_cmd(t_cmd *cmd);
void					init_bltn(t_ms *ms);
int						get_bltn(t_ms *ms, char *cmd);
int						launch_bltn(t_ms *ms, t_cmd *cmd);
void					ft_list_sort(t_list **begin_list);
void					*list_sort(t_list **dest, t_list *list);

/*
** Exec folder
*/

void					error_file(t_ms *ms, t_cmd *cmd);
void					search_prog(t_ms *ms, t_cmd *cmd);

/*
** print.c
*/

void					print_tokens(t_tokens *tokens);
void					print_cmd(t_cmd *cmd);
void					print_action(t_cmd *cmd);
void					print_action_exec_condition(t_cmd *cmd,
						int pipe, t_ms *ms);

/*
** tokens.c
*/

void					free_tokens(t_tokens *tokens);

/*
** utils.c
*/

void					free_split(char ***split);
void					free_env(void *env);
char					*ft_getenv(t_list **head_ref, char *elem, int i);
t_var					*ft_get_t_var(t_list **head_ref, char *elem);
int						valid_name(char *name);
int						is_type(t_cmd *cmd, int type);
int						has_pipe(t_cmd *cmd);

void					setup_execution(t_ms *ms, t_cmd *cmd);
int						valid_quotes(const char *str, int len);
void					parse(char *str, t_ms *ms);
int						set_token_type(char *word_list);
void					close_fd(int fd);
int						choose_action(t_ms *ms, t_cmd *cmd);
void					valid_file(t_cmd *cmd);

void					launch_cmd(t_ms *ms, t_cmd *cmd, int pipe);
void					launch_redirection(t_ms *ms, t_cmd *cmd,
						int redirection_type);
int						select_action(t_ms *ms, t_cmd *cmd, int i);

/*
**	expansion
*/
char					*value(t_ms *mini, char *str, int *i);
char					*get_name_var(char *str);
int						is_spec_car(char c);

/*
**	parser
*/
int						back_slash(t_ms *ms, char *str, char **word, int i);
int						special(t_ms *ms, char *str, char **word, int i);
int						quote(char *str, char **word, int i, int q);
void					new_token(t_ms *ms, t_tokens **tokens, char **word);
t_list					*parse_bloc(char *str);

/*
** init_env
*/

char					**lst_to_arr(t_list *env);
void					init_lstenv(t_ms *ms, t_list **lst_env, char **env);
t_list					*lstnew_var(t_var *content);
t_var					*init_envvar(char *env_var);
#endif
