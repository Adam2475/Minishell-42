/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_export.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mapichec <mapichec@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/28 15:06:36 by mapichec          #+#    #+#             */
/*   Updated: 2024/08/30 12:15:12 by mapichec         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

// declare -x SSH_AGENT_PID="1140490"

int	cmd_export(char **args, t_data **data)
{
	t_env_list	*node;
	char		*tmp;
	
	node = (*data)->env_list;
	tmp = NULL;
	if (args[1])
		return (1);
		// return (add_var_env(args, data));
	while (node->next)
	{
		if (!tmp)
		{
			tmp = node->var;
			ft_printf("declare -x %s\"%s\"", node->var, node->value);
		}
		else if (tmp)
		{
			
			ft_printf("declare -x %s\"%s\"", node->var, node->value);
		}
		node = node->next;
	}
	return(0);
	// ft_printf("\033[0;91mPWD %s\033[0;39m\n", node->value);
}
