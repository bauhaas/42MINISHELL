/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clorin <clorin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/17 12:28:05 by clorin            #+#    #+#             */
/*   Updated: 2021/03/18 16:01:18 by bahaas           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdlib.h>
# include <stdio.h>
# include <unistd.h>
# include "../libft/libft.h"

typedef	struct	s_var
{
	char *name;
	char *value;
}				t_var;

typedef struct	s_ms
{
	t_list	*env;
	char	*pwd;
}				t_ms;

void	main_echo();
void	echo(char **line);

void	main_pwd(t_ms *ms);
void	pwd(t_ms *ms, char **line);
#endif
