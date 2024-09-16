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

int	add_to_env(t_token *arg, t_data **data)
{
	t_env_list	*node;
	t_env_list	*head;

	head = (*data)->env_list;
	node = head;
	// TODO: check if it works
	while (node)
	{
		if (ft_strncmp(arg->value, node->var, ft_strlen_char(arg->value, '=')) == 0)
		{
			node->value = ft_strndup(arg->value + ft_strlen_char(arg->value, '='), ft_strlen_char(arg->value, '\0'));
			break ;
		}
		if (!node->next)
			break ;
		node = node->next;
	}
	node = new_node_env(arg->value);
	add_back_env(&head, node);
	return (0);
}

int	export_cmd(char **args, t_data **data)
{
	t_env_list	*node;
	t_token		*current;
	// int			size;
	// int			i;
	int			flag;

	node = (*data)->env_list;
	current = (*data)->tokens;
	// size = ft_lstsize_token(current);
	// i = 1;
	flag = 0;
	while (current->value && current->type != TOKEN_EOF)
	{
		if (current->value && !((current->value[0] >= 'a' && current->value[0] <= 'z')
				|| (current->value[0] >= 'A' && current->value[0] <= 'Z')))
			return (ft_printf("bash: export: `%s': not a valid identifier\n", current->value));
		if (current->value && ft_strsearch(current->value, '='))
		{
			add_to_env(current, data);
			flag = 1;
		}
		current = current->next;
	}
	while (node->next && flag == 0)
	{
		ft_printf("declare -x %s\"%s\"\n", node->var, node->value);
		node = node->next;
	}
	ft_printf("declare -x %s\"%s\"\n", node->var, node->value);
	return(1);
}
