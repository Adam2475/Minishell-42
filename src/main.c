/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adapassa <adapassa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/05 15:01:08 by adapassa          #+#    #+#             */
/*   Updated: 2024/08/30 06:36:15 by adapassa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

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
	t_token			*tmp;

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
		tokens = tokenize_string(&data);
		token_reformatting(&tokens);
		env_parser(&data, envp);

		//print_tokens(tokens);
		//exit(1);

		set_token_state(&tokens);

		print_tokens_state(tokens);
		//printf("ciao");
		//exit(1);
		
		if (piper(&tokens) == 0)
			token_parser(&tokens, &data, envp);
		else
		{
			token_list = split_tokens_by_pipe(tmp);
			pipe_case(&tokens, &data, envp, &token_list);
		}
		free_exit(&data);
	}
	return (0);
}
