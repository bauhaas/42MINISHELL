# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: clorin <clorin@student.42.fr>              +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2021/03/17 12:01:33 by clorin            #+#    #+#              #
#    Updated: 2021/03/24 11:02:53 by bahaas           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME		= minishell

SRC		= srcs/main.c srcs/init.c srcs/command.c srcs/tokens.c srcs/execute.c srcs/expansion.c \
		srcs/termcaps/cursor.c srcs/termcaps/termcaps.c srcs/termcaps/keyboard.c \
		srcs/termcaps/print_utils.c srcs/termcaps/prompt.c srcs/termcaps/history.c \
		srcs/builtins/pwd.c srcs/builtins/echo.c srcs/builtins/export.c	srcs/builtins/env.c \
		srcs/builtins/unset.c srcs/builtins/exit.c srcs/builtins/cd.c srcs/utils.c

CC		= clang

#FLAGS		= -Wall -Werror -Wextra
FLAGS		=

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

re :		fclean all

.PHOMY :	all clean fclean re
	
