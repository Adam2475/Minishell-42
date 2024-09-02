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
int	ft_strsearch(char *str, int c)
{
	int i;

	i = 0;
	if (!str)
		return (1);
	while (str[i] != '\0')
	{
		if (str[i] == c)
			return (1);
		i++;
	}
	return (0);
}

int	add_to_env(char *arg, t_data **data)
{
	t_env_list	*node;
	t_env_list	*head;

	node = new_node_env(arg);
	head = (*data)->env_list;
	add_back_env(&head, node);
	return (1);
}

int	export_cmd(char **args, t_data **data)
{
	t_env_list	*node;
	t_token		*current;
	char		*tmp;
	int			size;
	int			i;
	
	node = (*data)->env_list;
	current = (*data)->tokens;
	size = ft_lstsize_token(current);
	i = 1;
	/* EXPANDER ??*/
	// if ((*data)->tokens->next->type == TOKEN_DOLLAR)
	while (args[i] && i < size)
	{
		if (args[i] && !((args[i][0] >= 'a' && args[i][0] <= 'z')
				|| (args[i][0] >= 'A' && args[i][0] <= 'Z')))
			return (ft_printf("bash: export: `%s': not a valid identifier\n", args[i]));
		if (args[i] && ft_strsearch(args[i], '='))
			add_to_env(args[i], data);
		i++;
	}
	while (node->next)
	{
		ft_printf("declare -x %s\"%s\"\n", node->var, node->value);
		node = node->next;
	}
	ft_printf("declare -x %s\"%s\"\n", node->var, node->value);
	return(0);
}
