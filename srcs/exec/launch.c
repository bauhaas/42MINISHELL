/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   launch.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bahaas <bahaas@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/20 14:40:36 by bahaas            #+#    #+#             */
/*   Updated: 2021/05/03 11:37:39 by bahaas           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void		empty_cmd_content(t_ms *ms, t_cmd *cmd)
{
	if (!cmd->is_env)
	{
		printf("\nLa commande  «  » n'a pas été trouvée,");
		printf(" mais peut être installée avec :\n\n");
		printf("sudo apt install libpam-mount\n");
		printf("sudo apt install openssh-server\n");
		printf("sudo apt install openvswitch-common\n");
		printf("sudo apt install openvswitch-switch\n");
		printf("sudo apt install php-common\n");
		printf("sudo apt install bpfcc-tools\n");
		printf("sudo apt install burp\n");
		printf("sudo apt install cryptomount\n");
		printf("sudo apt install dolphin-emu\n");
		printf("sudo apt install mailutils-mh\n");
		printf("sudo apt install mmh\n");
		printf("sudo apt install nmh\n");
		printf("\n");
		ms->last_ret = 127;
	}
}
