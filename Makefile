# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: clorin <clorin@student.42.fr>              +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2021/03/17 12:01:33 by clorin            #+#    #+#              #
#    Updated: 2021/05/06 14:44:20 by bahaas           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME	= minishell

SRC		= srcs/main/main.c				\
		srcs/main/init.c				\
		srcs/main/init_env.c			\
		srcs/main/utils.c				\
		srcs/main/free.c 				\
		srcs/termcaps/cursor.c			\
		srcs/termcaps/termcaps.c		\
		srcs/termcaps/keyboard.c		\
		srcs/termcaps/termcaps_utils.c	\
		srcs/termcaps/print_utils.c		\
		srcs/termcaps/prompt.c			\
		srcs/termcaps/history.c			\
		srcs/termcaps/cursor_utils.c	\
		srcs/builtins/pwd.c				\
		srcs/builtins/echo.c			\
		srcs/builtins/export.c			\
		srcs/builtins/env.c				\
		srcs/builtins/export_utils.c	\
		srcs/builtins/unset.c			\
		srcs/builtins/exit.c			\
		srcs/builtins/cd.c				\
		srcs/builtins/cd_error.c		\
		srcs/builtins/builtins.c		\
		srcs/parsing/expansion.c		\
		srcs/parsing/tokens.c			\
		srcs/parsing/utils.c			\
		srcs/parsing/parser.c			\
		srcs/parsing/parser_utils.c		\
		srcs/parsing/parse_bloc.c		\
		srcs/exec/file.c				\
		srcs/exec/file_utils.c			\
		srcs/exec/signal.c				\
		srcs/exec/redirection.c			\
		srcs/exec/execution.c			\
		srcs/exec/fork.c				\
		srcs/exec/fd.c					\
		srcs/exec/types.c				\
		srcs/cmd/tokens_to_cmd.c		\
		srcs/cmd/command.c				\
		srcs/cmd/fill_command.c			\

CC		= clang

FLAGS	= -Wall -Werror -Wextra

HEADER	= -I includes/

OBJ		= $(SRC:.c=.o)

all :		$(NAME)

.c.o :	
		@printf "\033[0;33mGenerating Minishell objects... %-33.33s\r" $@
		@$(CC) $(FLAGS) $(HEADER) -c $< -o $(<:.c=.o)

$(NAME) :	$(OBJ)
		@echo "\033[31m██   ██ ██████  \033[32m ███    ███ ██ ███    ██ ██ ███████ ██   ██ ███████ ██      ██\033[0m" 
		@echo "\033[31m██   ██      ██ \033[32m ████  ████ ██ ████   ██ ██ ██      ██   ██ ██      ██      ██\033[0m"   
		@echo "\033[31m███████  █████  \033[32m ██ ████ ██ ██ ██ ██  ██ ██ ███████ ███████ █████   ██      ██\033[0m"
		@echo "\033[31m     ██ ██      \033[32m ██  ██  ██ ██ ██  ██ ██ ██      ██ ██   ██ ██      ██      ██\033[0m"
		@echo "\033[31m     ██ ███████ \033[32m ██      ██ ██ ██   ████ ██ ███████ ██   ██ ███████ ███████ ███████\033[0m"
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

run:	$(NAME)
		./minishell

.PHOMY :	all clean fclean re
	
