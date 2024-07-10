/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adapassa <adapassa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/10 14:04:42 by adapassa          #+#    #+#             */
/*   Updated: 2024/07/10 16:38:50 by adapassa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

void	token_parser(t_data *data, t_token **tokens, char **envp)
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
			}
			else
			{
				data->state = STATE_DOUBLE_QUOTES;
				printf("state switched to double quotes!\n");
				current = current->next;	
			}
		}
		if (current->type == TOKEN_SINGLE_QUOTES && data->state != STATE_DOUBLE_QUOTES)
		{
			if (data->state == STATE_SINGLE_QUOTES)
			{
				data->state = NORMAL;
				printf("single quote finished, state restored!\n");
			}
			else
			{
				data->state = STATE_SINGLE_QUOTES;
				printf("state switched to single quotes!\n");
				current = current->next;
			}
		}
		if (current->type == TOKEN_DOLLAR && data->state != STATE_SINGLE_QUOTES)
		{
			printf("Ready to variable expansion!\n");
			expand_variable(&current, envp);
			current = current->next;
		}
		current = current->next;
	}
	if (data->state != NORMAL)
	{
		printf("parse error, unclosed quotes, exiting!\n");
		//exit(1);
	}
}

char	*expand_variable(t_token **tokens, char **envp)
{
	t_token	*current;
	current = *tokens;
	char	*result;
	printf("expanding the variable\n");

	//printf("%s\n", current->value);
	if (current->value[0] == DOLLAR_SIGN)
	{
		printf("found dollar sign!\n");
	}
	return (result);
}