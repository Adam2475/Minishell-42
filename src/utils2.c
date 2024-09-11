/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adapassa <adapassa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/26 10:39:34 by adapassa          #+#    #+#             */
/*   Updated: 2024/08/31 10:40:16 by adapassa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

t_token *copy_token(t_token *token)
{
	if (!token)
		return NULL;

	t_token *new_token = (t_token *)malloc(sizeof(t_token));
	if (!new_token)
		return NULL;

	new_token->type = token->type;
	new_token->value = strdup(token->value);
	new_token->next = NULL;

	return new_token;
}

int count_pipes(t_token* head)
{
	int count = 0;
	t_token* current = head;

	while (current != NULL)
	{
		if (current->type == TOKEN_PIPE)
		{
			count++;
		}
		current = current->next;
	}
	return (count);
}

size_t calculate_command_length(t_token *head)
{
	size_t total_length = 0;
	t_token *current = head;
	while (current)
	{
		total_length += strlen(current->value);
		// Add 1 for the space separator
		if (current->next != NULL)
			total_length += 1;
		current = current->next;
	}
	return total_length;
}

char	*token_to_command(t_token *head)
{
	if (!head)
		return NULL;

	size_t command_length = calculate_command_length(head);
	char *command = (char *)malloc(command_length + 1); // +1 for the null terminator

	if (!command)
	{
		perror("malloc");
		return NULL;
	}

	command[0] = '\0'; // Initialize the command string

	t_token *current = head;
	while (current)
	{
		strcat(command, current->value);
		if (current->next != NULL)
			strcat(command, " "); // Add space between tokens
		current = current->next;
	}

	return command;
}

int ft_lstsize_token(t_token *tokens)
{
	t_token	*node;
	int		i;
	
	i = 0;
	if (!tokens)
		return (ft_printf("ft_lstsize_token\n"));
	node = tokens;
	while (node->next)
	{
		node = node->next;
		i++;
	}
	return (i);
}

t_token *copy_token_list(t_token *tokens)
{
	if (!tokens)
		return NULL;

	int i = 0;
	t_token *new_list = NULL;
	t_token *last_copied = NULL;
	t_token *current = tokens;
	int total = ft_lstsize_token(tokens);

	while (current && i < (total))
	{
		t_token *new_token = copy_token(current);

		if (!new_list)
			new_list = new_token;
		else
			last_copied->next = new_token;

		last_copied = new_token;
		current = current->next;
		i++;
	}
	return new_list;
}