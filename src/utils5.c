/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils5.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adapassa <adapassa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/30 08:32:03 by adapassa          #+#    #+#             */
/*   Updated: 2024/09/01 12:44:33 by adapassa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

void	remove_ws(t_token **token)
{
	t_token	*node;
	t_token	*tmp;

	node = (*token);
	tmp = NULL;
	while (node->type == TOKEN_WHITESPACE && node->type != TOKEN_EOF)
	{
		tmp = node;
		node = node->next;
		free(tmp->value);
		free(tmp);
		tmp = NULL;
	}
	while (node->type != TOKEN_EOF)
	{
		if (node->next->type == TOKEN_WHITESPACE)
			tkn_delone(&node, node->next);
		else
			node = node->next;
	}	
}

int check_quotes(t_token **tokens)
{
	t_token	*current;
	t_token	*current_2;

	current = (*tokens);
	current_2 = NULL;
	while ((int)current->type != TOKEN_EOF)
	{
		if ((int)current->type == 10)
		{
			current_2 = current->next;
			while ((int)current_2->type != 7 && (int)current_2->type != 10)
			{
				current_2->type = TOKEN_WORD;
				current_2 = current_2->next;
			}
			if ((int)current_2->type == 7)
				return (ft_printf("check_quotes\n"), 1);
			current = current_2;
		}
		if ((int)current->type == 9)
		{
			current_2 = current->next;
			while ((int)current_2->type != 7 && (int)current_2->type != 9)
			{
				if ((int)current_2->type != 8)
					current_2->type = TOKEN_WORD;
				else if ((int)current->type == 8 && (int)current->next->type == 13)
					current_2 = current->next;
				current_2 = current_2->next;
			}
			if ((int)current_2->type == 7)
				return (ft_printf("check_quotes\n"), 1);
			current = current_2;
		}
		current = current->next;
	}
	remove_ws(tokens);
	return (0);
}
