/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_pwd.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mapichec <mapichec@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/21 19:09:53 by mapichec          #+#    #+#             */
/*   Updated: 2024/08/24 16:46:03 by mapichec         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

int	pwd_cmd(t_data **data)
{
	t_env_list	*node;

	node = (*data)->env_list;
	ft_printf("\033[0;91mPWD_CMD\033[0;39m\n");
	while (ft_strncmp(node->var, "PWD=", 4))
		node = node->next;
	ft_printf("%s\n", node->value);
	return ((*data)->err_state = 0, 1);
}
