/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils5.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adapassa <adapassa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/30 08:32:03 by adapassa          #+#    #+#             */
/*   Updated: 2024/08/30 08:44:20 by adapassa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

int	check_unclosed_quotes(t_token *token)
{
	int	single_quote_count;
	int	double_quote_count;

	single_quote_count = 0;
	double_quote_count = 0;
	while (*token->value)
	{
		if (*token->value == '\'')
			single_quote_count++;
		else if (*token->value == '"')
			double_quote_count++;
		token->value++;
	}
	if (single_quote_count % 2 != 0 || double_quote_count % 2 != 0)
		return (1);
	return (0);
}

int	check_quotes(t_token *tokens)
{
	t_token	*current = tokens;

	while (current)
	{
		if (current->state > STATE_NORMAL)
		{
			if (check_unclosed_quotes(current))
				return (1);
		}
		current = current->next;
	}
	return (0);
}

void	print_tokens_state(t_token *tokens)
{
	t_token	*temp = tokens;

	while (temp)
	{
		ft_printf("State: %d Type: %d, Value: %s\n", temp->state, temp->type, temp->value);
		temp = temp->next;
	}
}