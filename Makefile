# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: clorin <clorin@student.42.fr>              +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2021/03/17 12:01:33 by clorin            #+#    #+#              #
#    Updated: 2021/03/21 14:55:56 by bahaas           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME		= minishell

SRC		= srcs/main.c srcs/pwd.c srcs/echo.c srcs/tokens.c srcs/execute.c \
		  srcs/export.c	srcs/env.c srcs/unset.c srcs/exit.c srcs/cd.c srcs/test_builtin.c
CC		= clang

#FLAGS		= -Wall -Werror -Wextra
FLAGS		=

HEADER		= -I includes/

OBJ		= $(SRC:.c=.o)

all :		$(NAME)

.c.o :	
		@$(CC) $(FLAGS) $(HEADER) -c $< -o $(<:.c=.o)
		@echo "compilation : "$< "\033[32mOK\033[0m"

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
	
