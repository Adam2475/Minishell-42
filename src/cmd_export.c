/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_export.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adapassa <adapassa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/28 15:06:36 by mapichec          #+#    #+#             */
/*   Updated: 2024/09/15 16:34:52 by adapassa         ###   ########.fr       */
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

	head = (*data)->env_list;
	node = head;
	// TODO: check if it works
	while (node)
	{
		if (ft_strncmp(arg, node->var, ft_strlen_char(arg, '=')) == 0)
		{
			node->value = ft_strndup(arg + ft_strlen_char(arg, '='), ft_strlen_char(arg, '\0'));
			break ;
		}
		if (!node->next)
			break ;
		node = node->next;
	}
	node = new_node_env(arg);
	add_back_env(&head, node);
	return (0);
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
	// while (node->next)
	// {
	// 	ft_printf("declare -x %s\"%s\"\n", node->var, node->value);
	// 	node = node->next;
	// }
	// ft_printf("declare -x %s\"%s\"\n", node->var, node->value);
	return(1);
}
