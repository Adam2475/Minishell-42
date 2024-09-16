/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adapassa <adapassa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/06 14:14:27 by adapassa          #+#    #+#             */
/*   Updated: 2024/09/13 18:31:21 by adapassa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

// if you move the pointer around then the free wont work
// that's why using counters is safer

t_token	*tokenize_string(t_data **data)
{
	char		*tmp;
	char		*buffer;
	char		*end;
	int			flag_ws;
	t_token		*tokens;

	tmp = ft_strndup((*data)->input, ft_strlen((*data)->input));
	if (!tmp)
		free_exit(data);
	buffer = tmp;
	init_state(data, &tokens);
	while (*buffer)
	{
		if ((*buffer) == WHITESPACE)
		{
			ft_tokenadd_back(&tokens, ft_lstnewtoken(TOKEN_WHITESPACE, ft_strndup(buffer, 1)));
			buffer++;
			continue;
		}
		if (*buffer == REDIRECT_LEFT
			|| *buffer == REDIRECT_RIGHT
			|| *buffer == PIPE || *buffer == '$'
			|| *buffer == SINGLE_QUOTES
			|| *buffer == DOUBLE_QUOTES)
		{
			buffer = buffer + special_cases_lexer(data, buffer, &tokens);
			continue;
		}
		end = buffer;
		while (*end && *end != WHITESPACE
				&& *end != REDIRECT_LEFT && *end != PIPE
				&& *end != REDIRECT_RIGHT && *end != '$'
				&& *end != DOUBLE_QUOTES
				&& *end != SINGLE_QUOTES)
		{
			end++;
		}
		if (*buffer == '-')
			ft_tokenadd_back(&tokens, ft_lstnewtoken(TOKEN_OPTION, ft_strndup(buffer, end - buffer)));
		else
			ft_tokenadd_back(&tokens, ft_lstnewtoken(TOKEN_WORD, ft_strndup(buffer, end - buffer)));
		buffer = end;
	}
	ft_tokenadd_back(&tokens, ft_lstnewtoken(TOKEN_EOF, NULL));
	free(tmp);
	return (tokens);
}

int	special_cases_lexer(t_data **data, char *buffer, t_token **tokens)
{
	char *end = NULL;
	if (*buffer == REDIRECT_LEFT && data)
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
	if (*buffer == DOLLAR_SIGN)
	{
		int i = 1;
		end = buffer;
		while (*++end && *end != WHITESPACE
				&& *end != REDIRECT_LEFT && *end != PIPE
				&& *end != REDIRECT_RIGHT && *end != '$'
				&& *end != DOUBLE_QUOTES
				&& *end != SINGLE_QUOTES)
			i++;
		ft_tokenadd_back(tokens, ft_lstnewtoken(TOKEN_DOLLAR, ft_strndup(buffer, end - buffer)));
		return (i);
	}
	if (*buffer == SINGLE_QUOTES)
	{
		ft_tokenadd_back(tokens, ft_lstnewtoken(TOKEN_SINGLE_QUOTES, ft_strndup(buffer, 1)));
		return (1);
	}
	if (*buffer == DOUBLE_QUOTES)
	{
		ft_tokenadd_back(tokens, ft_lstnewtoken(TOKEN_DOUBLE_QUOTES, ft_strndup(buffer, 1)));
		return (1);
	}
	return (0);
}

void	*token_reformatting(t_token **tokens)
{
	t_token		*head;
	t_token		*current;

	head = *tokens;
	current = *tokens;
	while (current && current->type != TOKEN_EOF)
	{
		if (current && current->type == TOKEN_EOF)
			return (NULL);
		if (current && current->type == TOKEN_PIPE)
		{
			current = current->next;
			if (current && current->type == TOKEN_WORD)
				current->type = TOKEN_COMMAND;
			current = current->next;
			if (current && current->type == TOKEN_WORD)
				current->type = TOKEN_APPENDICE;
			if (current && current->next)
				current = current->next;
			continue;
		}
		if (current && current->type != TOKEN_WORD
			&& current->type != TOKEN_OPTION
			&& current->type != TOKEN_DOLLAR)
		{
			current = current->next;
			if (current && current->type == 0)
				current->type = TOKEN_APPENDICE;
			current = current->next;
			if (current && current->type == 0)
				current->type = TOKEN_COMMAND;
			continue;
		}
		if (current && current->type == TOKEN_WORD)
		{
			current->type = TOKEN_COMMAND;
			current = current->next;
			while (current && (current->type == 0 && current->type != 7) || current->type == TOKEN_OPTION)
			{
				current->type = TOKEN_APPENDICE;
				current = current->next;
			}
			continue;
		}
		break;
	}
	return NULL;
}
