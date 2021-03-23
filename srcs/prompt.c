/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clorin <clorin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/18 13:56:39 by clorin            #+#    #+#             */
/*   Updated: 2021/03/22 22:03:07 by bahaas           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int			prompt(t_ms *ms)
{
	int ret;

	ret = ft_strlen(ft_getenv(&ms->env, "PWD"));
	write (1, ft_getenv(&ms->env, "PWD"), ret);
	write (1, "$> ", 3);
	return (ret + 3);
}
