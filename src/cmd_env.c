/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_env.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mapichec <mapichec@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/18 21:08:15 by marco             #+#    #+#             */
/*   Updated: 2024/08/22 15:52:09 by mapichec         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

int	env_cmd(t_data **data)
{
	t_data *tmp;
	int	i;

	i = 0;
	// ft_printf(" \033[0;92m-------- %d --------\n", getpid());
	tmp = (*data);
	while (tmp->env_var[i] != NULL)
	{
		ft_printf("\033[0;91m%s\n", tmp->env_var[i]);
		i++;
	}
	ft_printf("\033[0;39m");
	return (1);
}