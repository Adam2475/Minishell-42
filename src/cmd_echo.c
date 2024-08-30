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

/*
	TODO: 
	- Necessary tokenzation for "echo -n (...)"
	- For the moment just basic echo and trying for the dollar sign
*/

int	echo_doll(char *str, t_data **data)
{
	t_env_list	*node;
	int			len;
	
	len = ft_strlen(str);
	node = (*data)->env_list;
	if (str[0] == '\0' && !str)
		return (ft_printf("$\n"));
	while (strncmp(node->var, str, len) != 0 && node)
		node = node->next;
	return(ft_printf("%s\n", node->value));
}

int	echo_basic(char *str)
{
	if (str[0] == '\0' && !str)
		return (ft_printf("\n"));
	return (ft_printf("%s\n", str));
}

int	echo_cmd(t_data **data, char **cmd_args, t_token **tokens)
{
	t_token		*current;

	current = (*tokens);
	if (!cmd_args[1])
		return(ft_printf("\n"), 1);
	if (current->next->type == 8)
		echo_doll(cmd_args[2], data);
	else
		echo_basic(cmd_args[1]);
	return ((*data)->err_state = 0, 1);
}
