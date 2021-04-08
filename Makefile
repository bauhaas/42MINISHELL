# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: clorin <clorin@student.42.fr>              +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2021/03/17 12:01:33 by clorin            #+#    #+#              #
#    Updated: 2021/04/06 15:35:49 by bahaas           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME		= minishell

VERSION		= 0.3

SRC		= srcs/main.c srcs/init.c srcs/command.c srcs/execute.c\
		srcs/termcaps/cursor.c srcs/termcaps/termcaps.c srcs/termcaps/keyboard.c \
		srcs/termcaps/print_utils.c srcs/termcaps/prompt.c srcs/termcaps/history.c \
		srcs/builtins/pwd.c srcs/builtins/echo.c srcs/builtins/export.c	srcs/builtins/env.c \
		srcs/builtins/unset.c srcs/builtins/exit.c srcs/builtins/cd.c srcs/builtins/builtins.c srcs/utils.c \
		srcs/parsing/expansion.c srcs/parsing/quotes.c srcs/parsing/tokens.c srcs/parsing/word.c srcs/parsing/utils.c srcs/parsing/sep_and_escaped.c \
		srcs/print.c \
		srcs/exec/file.c

CC		= clang

#FLAGS		= -Wall -Werror -Wextra -D VERSION=$(VERSION)
FLAGS		= -g -D VERSION=$(VERSION)

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

run:	$(NAME)
		./minishell

.PHOMY :	all clean fclean re
	
