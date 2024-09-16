/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adapassa <adapassa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/05 15:01:08 by adapassa          #+#    #+#             */
/*   Updated: 2024/09/15 18:28:11 by adapassa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

int	err_state;

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
	err_state = 0;
	data = malloc(sizeof(t_data) * 1);
	set_data_zero(data);
	tmp = NULL;
	set_signal();
	while (1)
	{
		(data)->input = NULL;
		(data)->input = readline("myprompt$ ");
		(data)->fd = -1;
		if (!(data)->input)
		{
			(data)->tokens = ft_calloc(sizeof(t_token), 1);
			data->tokens->type = 14;
			free_exit(&data);
		}
		add_history(data->input);
		if (data->path_from_envp == NULL)
			env_parser(&data, envp);
		tokens = tokenize_string(&data);
		token_reformatting(&tokens);
		data->tokens = tokens;
		set_token_state(&tokens);
		if (check_quotes(&tokens) != 0)
			exit(printf("unclosed quotes found!!\n"));
		expand_var(&tokens, &data);
		tmp = copy_token_list(tokens);
		if (piper(&tokens) == 0)
			token_parser(&tokens, &data, envp);
		else
		{
			token_list = split_tokens_by_pipe(tmp);
			pipe_case(&tokens, &data, envp, &token_list);
		}
		free_list(&tokens);
		free_list(&tmp);
		free(tokens);
		free(data->input);
	}
	return (0);
}

//////////////////////////////////////
// Gdb process follow mode:
// set follow-fork-mode [parent|child]
//////////////////////////////////////

// Legenda
// ?? = NOT WORKING
// !? = SOME ISSUES
// !! = OK

////////////////
// Tests:
// grep int src/main.c | wc -w > outfile
// echo ciao !!
// expansion di ?
// cmd not found = 127 (exit_status)
// echo -n

//////////////
// Valgrind:

// << !!
// echo hello'world' ?!
// ciao !! (invalid cmd)
// echo "aspas ->'" !?
// export HELLO="123 A-" !?
// echo ciao
// cat maek !!
// ls -l > outfile !!
// echo -n ciao
// echo "$PWD"
// grep int src/main.c | cat -e > outfile !!
// cat src/utils3.c | cat src/utils5.c !!

// added minishell tester,  use ./tester from the directory
