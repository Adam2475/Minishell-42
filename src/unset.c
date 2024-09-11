/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adapassa <adapassa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/11 16:07:52 by adapassa          #+#    #+#             */
/*   Updated: 2024/09/11 16:14:23 by adapassa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

void unset_env(t_env_list **env, const char *var_name)
{
	t_env_list *current = *env;

	while (current)
	{
		if (ft_strncmp(current->var, var_name, ft_strlen(var_name)) == 0)
		{
			// Unlink the node from the list
			if (current->pre)
				current->pre->next = current->next;
			else
				*env = current->next; // If it's the first node, update the head

			if (current->next)
				current->next->pre = current->pre;

			// Free the current node
			free(current->var);
			free(current->value);
			free(current->content);
			free(current);
			return;
		}
		current = current->next;
	}
}