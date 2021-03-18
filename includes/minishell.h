/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clorin <clorin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/17 12:28:05 by clorin            #+#    #+#             */
/*   Updated: 2021/03/17 14:14:09 by bahaas           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdlib.h>
# include <stdio.h>
# include <unistd.h>
# include <term.h>
# include <sys/ioctl.h>
# include "../libft/libft.h"

# define STDIN 0
# define STDOUT 1
# define STDERR 2

# define EOF_KEY 4
# define BACKSPACE 127
# define LEFT_ARROW 4479771
# define RIGHT_ARROW 4414235

typedef struct		s_termcaps
{
	struct termios	term;
	struct termios	old_termcaps;
	char			*cm;
	char			*ce;
	char			*dl;
	char			*line;
	//int				len_line;
	int				col;
	int				row;
	int				cur_pos;
	int				start_col;

}					t_termcaps;

void				get_cursor_position(int *col, int *rows);
void				set_cursor_position(t_termcaps *tc, int col, int row);
void				keys_tree(long c, t_termcaps *tc);

void				init_termcaps(t_termcaps *tc);
char				*get_line(t_termcaps *tc);
int					tc_putchar(int c);
void				create_line(long c, t_termcaps *tc);
void				clear_line(t_termcaps *tc);
void				print_line(t_termcaps *tc);
void				del_char(t_termcaps *tc);
void				prompt(void);

#endif
