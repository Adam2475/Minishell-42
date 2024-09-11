/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils3.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adapassa <adapassa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/28 17:17:28 by adapassa          #+#    #+#             */
/*   Updated: 2024/08/31 10:53:57 by adapassa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

t_token *flatten_token_list(t_token_list *token_list)
{
	t_token *result = NULL;
	t_token_list *current_list = token_list;

	while (current_list)
	{
		t_token *current_token = current_list->head;
		while (current_token)
		{
			append_token(&result, create_token(current_token->type, current_token->value));
			current_token = current_token->next;
		}
		current_list = current_list->next;
	}
	return result;
}

t_token *extract_command_and_appendices(t_token *tokens)
{
	t_token *result = NULL;
	t_token *current = tokens;
	int command_found = 0;

	while (current)
	{
		if (current->type == TOKEN_COMMAND)
		{
			command_found = 1;
			append_token(&result, create_token(current->type, current->value));
		}
		else if (command_found && (current->type == TOKEN_APPENDICE || current->type == TOKEN_OPTION))
		{
			append_token(&result, create_token(current->type, current->value));
		}
		else if (command_found)
		{
			break;
		}
		current = current->next;
	}
	return result;
}

int	piper(t_token **tokens)
{
	t_token *current = *tokens;
	
	while (current->type != TOKEN_EOF)
	{
		if (current->type == TOKEN_PIPE)
			return (1);
		current = current->next;
	}
	return (0);
}

char	*retrieve_line(char **envp)
{
	int	i;

	i = 0;
	while (envp[i] != NULL)
	{
		if (ft_strnstr(envp[i], "PATH=", 5))
			return (ft_strdup(envp[i]));
		i++;
	}
	return (NULL);
}

t_token_list	*split_tokens_by_pipe(t_token *tokens)
{
	t_token_list	*result = NULL;
	t_token			*current = tokens;
	t_token			*segment_start = tokens;
	t_token			*prev = NULL;

	while (current)
	{
		if (current->type == TOKEN_PIPE)
		{
			if (prev)
				prev->next = NULL;

			append_token_list(&result, segment_start);

			segment_start = current;
		}
		prev = current;
		current = current->next;
	}

	if (segment_start)
		append_token_list(&result, segment_start);

	return (result);
}