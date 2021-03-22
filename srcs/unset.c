/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bahaas <bahaas@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/21 00:41:56 by bahaas            #+#    #+#             */
/*   Updated: 2021/03/21 14:24:00 by bahaas           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"


void del_one(t_list **head_ref, char *key)
{
    t_list *temp = *head_ref, *prev;
	t_var *var;

	var = (t_var *)temp->content;
    // If head node itself holds the key to be deleted
    if (temp != NULL && var->name == key) {
        *head_ref = temp->next; // Changed head
        free(temp); // free old head
        return;
    }
    // Search for the key to be deleted, keep track of the
    // previous node as we need to change 'prev->next'
    while (temp != NULL && var->name != key) {
        prev = temp;
        temp = temp->next;
    }
    // If key was not present in linked list
    if (temp == NULL)
        return;
    // Unlink the node from linked list
    prev->next = temp->next;
    free(temp); // Free memory
}

int	ft_unset(t_ms *ms, t_cmd *cmd)
{
	int i = 0;
	char *var;
	while(ms->env)
	{
		var = ft_getenv(ms->env, cmd->content[1]);
		i++;
		if(var)
		{
			del_one(&ms->env, var);
		}
		ms->env = ms->env->next;
	}
	printf("test i %d\n", i);
	return (0);
}
