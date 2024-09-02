/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mapichec <mapichec@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/05 15:01:08 by adapassa          #+#    #+#             */
/*   Updated: 2024/08/30 12:24:54 by mapichec         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

int check_double_redirects(const char *str)
{
    // Check if the string contains ">>"
    if (ft_strnstr(str, ">>", 2) != NULL)
    {
        return 1; // Found ">>"
    }

    // Check if the string contains "<<"
    if (ft_strnstr(str, "<<", 2) != NULL)
    {
        return 1; // Found "<<"
    }

    return 0; // Neither ">>" nor "<<" found
}

static int check_unclosed_quotes(t_token *token)
{
	int single_quote_count = 0;
	int double_quote_count = 0;
	int	i = 0;

	while (token->value[i])
	{
		if (token->value[i] == '\'')
			single_quote_count++;
		else if (token->value[i] == '"')
			double_quote_count++;
		i++;
	}

	if (single_quote_count % 2 != 0 || double_quote_count % 2 != 0)
		return 1; // Error: Unclosed quotes
	return 0; // No error
}

static int check_quotes(t_token *tokens)
{
	t_token *current = tokens;

	while (current)
	{
		if (current->state > STATE_NORMAL) // Only check if state is greater than 0
		{
			if (check_unclosed_quotes(current))
				return 1; // Error found
		}
		current = current->next;
	}
	return 0; // No errors found
}

static void	print_tokens_state(t_token *tokens)
{
	t_token *temp = tokens;
	while (temp)
	{
		printf("State: %d Type: %d, Value: %s\n", temp->state, temp->type, temp->value);
		//printf("State: %d Type: %d, Value: %s\n", temp->state, temp->type, temp->value);
		temp = temp->next;
	}
}

int main(int argc, char **argv, char **envp)
{
	t_data			*data;
	t_token			*tokens;
	t_token_list	*token_list;
	// t_token			*tmp;

	argc = 0;
	argv = NULL;
	data = malloc(sizeof(t_data) * 1);

	while (1)
	{
		(data)->input = NULL;
		(data)->input = readline("myprompt$ ");
		(data)->fd = -1;
		//(data)->state = NORMAL;
	
		if (!(data)->input)
			exit(1);
		env_parser(&data, envp);
		tokens = tokenize_string(&data);
		token_reformatting(&tokens);
		data->tokens = tokens;

		//print_tokens(tokens);
		//exit(1);

		set_token_state(&tokens);
		if (check_quotes(tokens) != 0 )
			exit(printf("unclosed quotes found!!\n"));

		/////////////////////////////
		// To Do
		// Implement Expander
		// single quotes treated as literal have no difference
		// if state is $ of "" then go to expander

		/////////////////////////////
		//Debug
		print_tokens_state(tokens);
		if (piper(&tokens) == 0)
			token_parser(&tokens, &data, envp);
		else
		{
			// token_list = split_tokens_by_pipe(tmp);
			pipe_case(&tokens, &data, envp, &token_list);
		}
		free_exit(&data);
	}
	return (0);
}
