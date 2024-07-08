/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adapassa <adapassa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/06 14:14:27 by adapassa          #+#    #+#             */
/*   Updated: 2024/07/08 09:00:53 by adapassa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

int		lexer_control(t_data *data, int j)
{
	int	i;
	t_data *endpoint;

	endpoint = data;
	i = j;
	if (endpoint->input[i] == DOUBLE_QUOTES)
	{
		data->state = STATE_DOUBLE_QUOTES;
		ft_printf("Actual lexer state :%d\n", data->state);
	}
	else if (endpoint->input[i] == SINGLE_QUOTES)
	{
		data->state = STATE_DOUBLE_QUOTES;
		ft_printf("Actual lexer state :%d\n", data->state);
	}
}

void	tokenize_string(t_data *data)
{
	char				*buffer;
	char				*end;
	t_token				*tokens;
	int					i;

	i = 0;
	buffer = data->input;
	init_state(data, tokens);
	while (buffer[i])
	{
		if ((buffer[i]) == WHITESPACE)
		{
			i++;
			continue; // Skip the rest of the loop body
		}
		if (buffer[i] == REDIRECT_LEFT
			|| buffer[i] == REDIRECT_RIGHT
			|| buffer[i] == PIPE
			|| buffer[i] == SINGLE_QUOTES
			|| buffer[i] == DOUBLE_QUOTES)
		{
			i = i + special_cases_lexer(data, i);
			printf("i value: %d\n", i);
			continue;
		}
		end = buffer;
		while (*end && *end != WHITESPACE
				&& *end != REDIRECT_LEFT && *end != PIPE
				&& *end != REDIRECT_RIGHT)
				end++;

		ft_tokenadd_back(&tokens, ft_lstnewtoken(TOKEN_WORD, ft_strdup(buffer)));
		//ft_printf("%c", buffer[i]);
		i++;
	}
	while (tokens) {
		printf("%-15s %-20s\n", token_type_to_string(tokens->type), tokens->value ? tokens->value : "NULL");
		tokens = tokens->next;
	}
}

int	special_cases_lexer(t_data *data, int i)
{
	char 	*buffer;
	char	*end;

	buffer = data->input;
	ft_printf("index number passed: %d\n", i);
	printf("buffer value at index position: %c\n", buffer[i]);
	if (buffer[i] == REDIRECT_LEFT)
	{
		if (buffer[i + 1] == REDIRECT_LEFT)
		{
			//add_token('<<')
			ft_printf("case herdoc\n");
			return (2);
		}
		else
		{
			printf("case redirect left\n");
			return (1);
		}
	}
	if (buffer[i] == REDIRECT_RIGHT)
	{
		if (buffer[i + 1] == REDIRECT_RIGHT)
		{
			//add_token('>>')
			printf("case append\n");
			return (2);
		}
		else
		{
			//add_token('>');
			printf("case redirect right\n");
			return (1);
		}
	}
	if (buffer[i] == PIPE)
	{
		printf("case pipe\n");
		return (1);
	}
	return (1);
}