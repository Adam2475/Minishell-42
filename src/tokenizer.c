/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mapichec <mapichec@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/06 14:14:27 by adapassa          #+#    #+#             */
/*   Updated: 2024/08/28 18:56:22 by mapichec         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

t_token	*tokenize_string(t_data **data)
{
	char				*buffer;
	char				*end;
	t_token				*tokens;

	buffer = (*data)->input;
	init_state(data, &tokens);
	while (*buffer)
	{
		if ((*buffer) == WHITESPACE)
		{
			buffer++;
			//ft_tokenadd_back(&tokens, ft_lstnewtoken(TOKEN_WHITESPACE, " "));
			continue; // Skip the rest of the loop body
		}
		if (*buffer == REDIRECT_LEFT
			|| *buffer == REDIRECT_RIGHT
			|| *buffer == PIPE
			//|| *buffer == SINGLE_QUOTES
			//|| *buffer == DOUBLE_QUOTES
			|| *buffer == DOLLAR_SIGN)
		{
			buffer = buffer + special_cases_lexer(data, buffer, &tokens);
			continue;
		}
		end = buffer;
		while (*end && *end != WHITESPACE
				&& *end != REDIRECT_LEFT && *end != PIPE
				&& *end != REDIRECT_RIGHT)
				// && *end != DOUBLE_QUOTES
				// && *end != SINGLE_QUOTES)
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
	return (tokens);
}

int	special_cases_lexer(t_data **data, char *buffer, t_token **tokens)
{
	// char	*end;

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
	// // State must wait for it's closure
	if (*buffer == DOLLAR_SIGN)
	{
		ft_tokenadd_back(tokens, ft_lstnewtoken(TOKEN_DOLLAR, ft_strndup(buffer, 1)));
		return (1);
	}
	// if (*buffer == SINGLE_QUOTES)
	// {
	// 	ft_tokenadd_back(tokens, ft_lstnewtoken(TOKEN_SINGLE_QUOTES, ft_strndup(buffer, 1)));
	// 	return (1);
	// }
	// if (*buffer == DOUBLE_QUOTES)
	// {
	// 	ft_tokenadd_back(tokens, ft_lstnewtoken(TOKEN_DOUBLE_QUOTES, ft_strndup(buffer, 1)));
	// 	return (1);
	// }
	return (0);
}

void	*token_reformatting(t_token **tokens)
{
	t_token		*head;
	t_token		*current;
	// char	*buffer;

	head = *tokens;
	current = *tokens;
	while (current->type != TOKEN_EOF)
	{
		if (current->type == TOKEN_EOF)
			return (NULL);
		if (current->type == TOKEN_PIPE)
		{
			current = current->next;
			if (current->type == TOKEN_WORD)
				current->type = TOKEN_COMMAND;
			current = current->next;
			if (current->type == TOKEN_WORD)
				current->type = TOKEN_APPENDICE;
			if (current->next)
				current = current->next;
			continue;
		}
		if (current->type != TOKEN_WORD && current->type != TOKEN_OPTION)
		{
			current = current->next;
			if (current->type == 0)
				current->type = TOKEN_APPENDICE;
			current = current->next;
			if (current->type == 0)
				current->type = TOKEN_COMMAND;
			continue;
		}
		if (current->type == TOKEN_WORD)
		{
			current->type = TOKEN_COMMAND;
			current = current->next;
			while ((current->type == 0 && current->type != 7) || current->type == TOKEN_OPTION)
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
