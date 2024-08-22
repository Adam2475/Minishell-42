/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adapassa <adapassa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/22 18:17:57 by adapassa          #+#    #+#             */
/*   Updated: 2024/08/22 18:56:49 by adapassa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

void print_token_lists(t_token_list *list)
{
	while (list)
	{
		t_token *current = list->head;
		printf("Token List:\n");
		while (current)
		{
			printf("Type: %d, Value: %s\n", current->type, current->value);
			current = current->next;
		}
		list = list->next;
		printf("----\n");
	}
}

void print_tokens(t_token *tokens)
{
	t_token *temp = tokens;
	while (temp)
	{
		printf("Type: %d, Value: %s\n", temp->type, temp->value);
		temp = temp->next;
	}
}

void	append_token_list(t_token_list **list, t_token *head)
{
	t_token_list *new_node = create_token_list_node(head);
	if (!new_node)
		return;

	if (*list == NULL) {
		*list = new_node;
	} else {
		t_token_list *current = *list;
		while (current->next)
			current = current->next;
		current->next = new_node;
	}
}