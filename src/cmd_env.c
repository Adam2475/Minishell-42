/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_env.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mapichec <mapichec@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/18 21:08:15 by marco             #+#    #+#             */
/*   Updated: 2024/08/24 16:46:21 by mapichec         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

int	env_cmd(t_data **data)
{
	t_env_list	*node;

	node = (*data)->env_list;
	ft_printf("\033[0;91mENV_CMD\033[0;39m\n");
	while (node != NULL)
	{
		ft_printf("%s\n", node->content);
		node = node->next;
	}
	return ((*data)->err_state = 0, 1);
}
