/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adapassa <adapassa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/10 14:04:42 by adapassa          #+#    #+#             */
/*   Updated: 2024/07/10 14:51:29 by adapassa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

void	token_parser(t_data *data, t_token **tokens)
{
	t_token *current;

	current = *tokens;
	while (current != NULL)
	{
		if (current->type == TOKEN_DOUBLE_QUOTES && data->state != STATE_SINGLE_QUOTES)
		{
			if (data->state == STATE_DOUBLE_QUOTES)
			{
				data->state = NORMAL;
				printf("double quote finished, state restored!\n");
				break;
			}
			data->state = STATE_DOUBLE_QUOTES;
			printf("state switched to double quotes!\n");
			current = current->next;
		}
		else if (current->type == TOKEN_SINGLE_QUOTES && data->state != STATE_DOUBLE_QUOTES)
		{
			if (data->state == STATE_SINGLE_QUOTES)
			{
				data->state = NORMAL;
				printf("single quote finished, state restored!\n");
				break;
			}
			data->state = STATE_SINGLE_QUOTES;
			printf("%d\n", data->state);
			printf("state switched to single quotes!\n");
			current = current->next;
		}
		else if (current->type == TOKEN_DOLLAR && data->state != STATE_SINGLE_QUOTES)
		{
			//data->state = STATE_DOLLAR;
			printf("Ready to variable expansion!\n");
			//printf("%d\n", data->state);
			current = current->next;
		}
		current = current->next;
	}
	if (data->state != NORMAL)
	{
		printf("parse error, unclosed quotes!\n");
		exit(1);
	}
}