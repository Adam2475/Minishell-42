/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_export.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mapichec <mapichec@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/28 15:06:36 by mapichec          #+#    #+#             */
/*   Updated: 2024/08/28 15:11:41 by mapichec         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

int	cmd_export(char **args, t_data **data)
{
	t_env_list *node;
	
	node = (*data)->env_list;
	// if ()
	while (node && ft_strncmp(node->var, "PWD=", 4) != 0)
	{
		node = node->next;
	}
	return(0);
	// ft_printf("\033[0;91mPWD %s\033[0;39m\n", node->value);
}
