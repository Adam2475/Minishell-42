/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adapassa <adapassa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/05 15:01:08 by adapassa          #+#    #+#             */
/*   Updated: 2024/09/01 12:43:10 by adapassa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

void free_exit(t_data **data)
{
	clear_history();
	free((*data)->input);
	close((*data)->fd);
}

int main(int argc, char **argv, char **envp)
{
	t_data *data;
	t_token *tokens;
	t_token_list *token_list;
	t_token *tmp;

	int flags = fcntl(STDIN_FILENO, F_GETFL, 0);
	fcntl(STDIN_FILENO, F_SETFL, flags | O_NONBLOCK);

	argc = 0;
	argv = NULL;
	data = malloc(sizeof(t_data) * 1);
	tmp = NULL;
	while (1)
	{
		(data)->input = NULL;
		(data)->input = readline("myprompt$ ");
		(data)->fd = -1;
		add_history(data->input);
		/**quando devi pulire memoria usa clear_history() */
		// input_parser(&data);
		// printf("%s\n", (data)->input);
		// exit(1);
		if (!(data)->input)
			exit(1);
		tokens = tokenize_string(&data);
		token_reformatting(&tokens);
		env_parser(&data, envp);
		set_token_state(&tokens);
		if (check_quotes(tokens) != 0)
			exit(printf("unclosed quotes found!!\n"));

		/////////////////////////////
		// To Do
		// Implement Expander
		// single quotes treated as literal have no difference
		// if state is $ of "" then go to expander

		/////////////////////////////
		// Debug
		// print_tokens_state(tokens);
		tmp = copy_token_list(tokens);
		if (piper(&tokens) == 0)
			token_parser(&tokens, &data, envp);
		else
		{
			token_list = split_tokens_by_pipe(tmp);
			pipe_case(&tokens, &data, envp, &token_list);
		}
	}
	free_exit(&data);
	return (0);
}

// Gdb process follow mode:
// set follow-fork-mode [parent|child]

// ?? = NOT WORKING
// !? = SOME ISSUES

// Tests:
// grep int src/main.c | wc -w > outfile
// echo ciao ??