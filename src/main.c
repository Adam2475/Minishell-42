/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adapassa <adapassa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/05 15:01:08 by adapassa          #+#    #+#             */
/*   Updated: 2024/09/11 17:58:35 by adapassa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

int	err_state;

void free_exit(t_data **data)
{
	clear_history();
	free((*data)->input);
	close((*data)->fd);
}

int check_quotes(t_token **tokens)
{
	t_token	*current;
	t_token	*current_2;

	current = (*tokens);
	current_2 = NULL;
	while ((int)current->type != TOKEN_EOF)
	{
		if ((int)current->type == 10)
		{
			current_2 = current->next;
			while ((int)current_2->type != 7 && (int)current_2->type != 10)
			{
				current_2->type = TOKEN_WORD;
				current_2 = current_2->next;
			}
			if ((int)current_2->type == 7)
				return (ft_printf("check_quotes\n"), 1);
			current = current_2;
		}
		if ((int)current->type == 9)
		{
			current_2 = current->next;
			while ((int)current_2->type != 7 && (int)current_2->type != 9)
			{
				if ((int)current_2->type != 8)
					current_2->type = TOKEN_WORD;
				else if ((int)current->type == 8 && (int)current->next->type == 13)
					current_2 = current->next;
				current_2 = current_2->next;
			}
			if ((int)current_2->type == 7)
				return (ft_printf("check_quotes\n"), 1);
			current = current_2;
		}
		current = current->next;
	}
	return (0);
}

void	print_tokens_state(t_token *tokens)
{
	t_token *temp = tokens;
	while (temp)
	{
		printf("State: %d Type: %d, Value: %s\n", temp->state, temp->type, temp->value);
		//printf("State: %d Type: %d, Value: %s\n", temp->state, temp->type, temp->value);
		temp = temp->next;
	}
}

static void	sigint(void)
{
	rl_on_new_line();
	// rl_redisplay();
	ft_putstr_fd("\n", STDOUT_FILENO);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
}

static void	sigquit(void)
{
	rl_on_new_line();
	rl_redisplay();
	ft_putstr_fd("  \b\b", STDOUT_FILENO);
}

static void	handle_signal(int signo)
{
	int	pid;
	int	status;

	pid = waitpid(-1, &status, WNOHANG);
	if (signo == SIGINT)
	{
		err_state = 130;
		if (pid == -1)
			sigint();
		else
			ft_putstr_fd("\n", STDOUT_FILENO);
	}
	else if (signo == SIGQUIT)
	{
		if (pid == -1)
			sigquit();
		else
			ft_putstr_fd("Quit: 3\n", STDOUT_FILENO);
	}
}

void	set_signal(void)
{
	signal(SIGINT, handle_signal);
	signal(SIGQUIT, handle_signal);
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
	err_state = 0;
	data = malloc(sizeof(t_data) * 1);
	tmp = NULL;
	set_signal();
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
		env_parser(&data, envp);
		tokens = tokenize_string(&data);
		token_reformatting(&tokens);
		data->tokens = tokens;

		//print_tokens(tokens);
		//exit(1);

		set_token_state(&tokens);
		// print_tokens_state(tokens);
		if (check_quotes(&tokens) != 0)
			exit(printf("unclosed quotes found!!\n"));

		/////////////////////////////
		// To Do
		// Implement Expander
		// single quotes treated as literal have no difference
		// if state is $ of "" then go to expander
		/////////////////////////////
		expand_var(&tokens, &data);
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

		//ft_printf("MAIN pwd\n\n");
		//print_env_pwd(&data);
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
// echo ciao ?!
// expansion di ?
// cmd not found = 127 (exit_status)