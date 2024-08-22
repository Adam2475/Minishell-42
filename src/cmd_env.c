/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_env.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mapichec <mapichec@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/18 21:08:15 by marco             #+#    #+#             */
/*   Updated: 2024/08/22 18:06:24 by mapichec         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

int	env_cmd(t_data **data)
{
	t_env_list	*node;

	node = (*data)->env_list;
	while (node != NULL)
	{
		ft_printf("\033[0;93m%s\n", node->content);
		node = node->next;
	}
	ft_printf("\033[0;39m");
	return (1);
}
