/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_echo.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mapichec <mapichec@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/24 17:32:27 by mapichec          #+#    #+#             */
/*   Updated: 2024/08/29 13:46:40 by mapichec         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

int	echo_cmd(t_data **data, t_token **tokens)
{
	t_token		*current;

	current = (*tokens);
	current = current->next;
	while (current->type != TOKEN_EOF)
	{
		ft_printf("%s ", current->value);
		current = current->next;
	}
	ft_putchar('\n');
	return ((*data)->err_state = 0, 1);
}
