/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_pwd.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adapassa <adapassa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/21 19:09:53 by mapichec          #+#    #+#             */
/*   Updated: 2024/09/15 16:33:32 by adapassa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

int	pwd_cmd(t_data **data)
{
	t_env_list	*node;

	node = (*data)->env_list;
	while (ft_strncmp(node->var, "PWD=", 4))
		node = node->next;
	ft_printf("%s\n", node->value);
	return (err_state = 0, 1);
}
