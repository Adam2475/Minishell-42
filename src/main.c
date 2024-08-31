/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adapassa <adapassa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/05 15:01:08 by adapassa          #+#    #+#             */
/*   Updated: 2024/08/31 12:47:41 by adapassa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

void	free_exit(t_data **data)
{
	free((*data)->input);
	close((*data)->fd);
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
	tmp = NULL;
	while (1)
	{
		(data)->input = NULL;
		(data)->input = readline("myprompt$ ");
		(data)->fd = -1;
	
		if (!(data)->input)
			exit(1);
		tokens = tokenize_string(&data);
		token_reformatting(&tokens);
		env_parser(&data, envp);

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
		//print_tokens_state(tokens);
		tmp = copy_token_list(tokens);

		if (piper(&tokens) == 0)
			token_parser(&tokens, &data, envp);
		else
		{
			// print_tokens(tmp);
			// errors to fix in the split
			// cmd: grep int src/main.c | wc -w > outfile
			token_list = split_tokens_by_pipe(tmp);
			//print_tokens(tokens);
			pipe_case(&tokens, &data, envp, &token_list);
		}
		free_exit(&data);
	}
	return (0);
}

// Gdb process follow mode:
// set follow-fork-mode [parent|child]