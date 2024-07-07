/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adapassa <adapassa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/06 14:14:27 by adapassa          #+#    #+#             */
/*   Updated: 2024/07/06 18:39:02 by adapassa         ###   ########.fr       */
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
	t_token_type		*tokens;
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
			i++;
			special_cases_lexer(data);
			continue;
		}
		ft_printf("%c", buffer[i]);
		i++;
	}
	ft_printf("\n");
}

void	special_cases_lexer(t_data *data)
{
	printf("found special case\n");
}