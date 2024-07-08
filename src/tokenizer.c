/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adapassa <adapassa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/06 14:14:27 by adapassa          #+#    #+#             */
/*   Updated: 2024/07/08 15:15:04 by adapassa         ###   ########.fr       */
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
	return (0);
}

t_token	*tokenize_string(t_data *data)
{
	char				*buffer;
	char				*end;
	t_token				*tokens;

	buffer = data->input;
	init_state(data, &tokens);
	while (*buffer)
	{
		if ((*buffer) == WHITESPACE)
		{
			buffer++;
			continue; // Skip the rest of the loop body
		}
		if (*buffer == REDIRECT_LEFT
			|| *buffer == REDIRECT_RIGHT
			|| *buffer == PIPE
			|| *buffer == SINGLE_QUOTES
			|| *buffer == DOUBLE_QUOTES
			|| *buffer == DOLLAR_SIGN)
		{
			buffer = buffer + special_cases_lexer(buffer, &tokens);
			continue;
		}
		end = buffer;
		while (*end && *end != WHITESPACE
				&& *end != REDIRECT_LEFT && *end != PIPE
				&& *end != REDIRECT_RIGHT)
		{
			end++;
		}
		if (*buffer == '-')
			ft_tokenadd_back(&tokens, ft_lstnewtoken(TOKEN_OPTION, ft_strndup(buffer, end - buffer)));
		else
			ft_tokenadd_back(&tokens, ft_lstnewtoken(TOKEN_COMMAND, ft_strndup(buffer, end - buffer)));
		buffer = end;
	}
	ft_tokenadd_back(&tokens, ft_lstnewtoken(TOKEN_EOF, NULL));
	return (tokens);
}

int	special_cases_lexer(t_data *data, char *buffer, t_token **tokens)
{
	char	*end;

	if (*buffer == REDIRECT_LEFT)
	{
		if (*(buffer + 1) == REDIRECT_LEFT)
		{
			ft_tokenadd_back(tokens, ft_lstnewtoken(TOKEN_HEREDOC, ft_strndup(buffer, 2)));
			return (2);
		}
		else
		{
			ft_tokenadd_back(tokens, ft_lstnewtoken(TOKEN_REDIRECT_IN, ft_strndup(buffer, 1)));
			return (1);
		}
	}
	if (*buffer  == REDIRECT_RIGHT)
	{
		if (*(buffer + 1)  == REDIRECT_RIGHT)
		{
			ft_tokenadd_back(tokens, ft_lstnewtoken(TOKEN_APPEND, ft_strndup(buffer, 2)));
			return (2);
		}
		else
		{
			ft_tokenadd_back(tokens, ft_lstnewtoken(TOKEN_REDIRECT_OUT, ft_strndup(buffer, 1)));
			return (1);
		}
	}
	if (*buffer == PIPE)
	{
		ft_tokenadd_back(tokens, ft_lstnewtoken(TOKEN_PIPE, ft_strndup(buffer, 1)));
		return (1);
	}

	// State must wait for it's closure

	if (*buffer == DOLLAR_SIGN)
	{
		ft_printf("Ready for variable expansion!");
		data->state = STATE_DOLLAR;
		ft_printf("State switched to dollar state!");
	}

	if (*buffer == SINGLE_QUOTES)
	{
		ft_printf("State switched to single quotes!");
		data->state = STATE_SINGLE_QUOTES;
	}

	if (*buffer == DOUBLE_QUOTES)
	{
		ft_printf("State switched to double quotes!");
		data->state = STATE_DOUBLE_QUOTES;
	}
	
	return (1);
}
