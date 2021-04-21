# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: clorin <clorin@student.42.fr>              +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2021/03/17 12:01:33 by clorin            #+#    #+#              #
#    Updated: 2021/04/21 15:46:32 by bahaas           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME		= minishell

VERSION		= 0.7

SRC		= srcs/main.c srcs/init.c srcs/command.c srcs/init_env.c\
		srcs/termcaps/cursor.c srcs/termcaps/termcaps.c srcs/termcaps/keyboard.c srcs/termcaps/termcaps_utils.c\
		srcs/termcaps/print_utils.c srcs/termcaps/prompt.c srcs/termcaps/history.c srcs/termcaps/cursor_utils.c\
		srcs/builtins/pwd.c srcs/builtins/echo.c srcs/builtins/export.c	srcs/builtins/env.c srcs/builtins/export_utils.c\
		srcs/builtins/unset.c srcs/builtins/exit.c srcs/builtins/cd.c srcs/builtins/builtins.c srcs/utils.c \
		srcs/parsing/expansion.c  srcs/parsing/tokens.c srcs/parsing/utils.c srcs/parsing/parser.c \
		srcs/parsing/parser_utils.c srcs/parsing/parse_bloc.c\
		srcs/print.c \
		srcs/exec/file.c srcs/exec/setup_execution.c srcs/exec/execute.c srcs/exec/signal.c srcs/exec/launch.c

CC		= clang

FLAGS		= -Wall -Werror -Wextra -g -D VERSION=$(VERSION)
#FLAGS		= -g -D VERSION=$(VERSION)

HEADER		= -I includes/

OBJ		= $(SRC:.c=.o)

all :		$(NAME)

.c.o :	
		@printf "\033[0;33mGenerating Minishell objects... %-33.33s\r" $@
		@$(CC) $(FLAGS) $(HEADER) -c $< -o $(<:.c=.o)

$(NAME) :	$(OBJ)
		@make -C libft/
		@$(CC) $(FLAGS) $(HEADER) $(OBJ) -o $(NAME) -L libft/ -lft -lncurses
		@echo "Building minishell : \033[32mOK\033[0m"

clean :		
		@rm -rf $(OBJ)
		@echo "Delete OBJ files : \033[32mOK\033[0m"

fclean :	clean
		@make fclean -C libft/
		@rm -rf $(NAME)
		@echo "Delete minishell : \033[32mOK\033[0m"

re :	fclean all

test:	$(NAME)
		./minishell -c test

echo:	$(NAME)
		./minishell -c echo_test

run:	$(NAME)
		./minishell

.PHOMY :	all clean fclean re
	
