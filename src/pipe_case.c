/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_case.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adapassa <adapassa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/26 10:12:13 by adapassa          #+#    #+#             */
/*   Updated: 2024/09/11 15:50:05 by adapassa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

static int child_process_pipe(char **envp, t_data **data, t_token *tokens)
{
	char		*holder;
	t_token		*new_tokens;
	t_token		*tmp;

	tmp = NULL;
	new_tokens = extract_command_and_appendices(tokens);
	holder = token_to_command(new_tokens);

	if (!((*data)->fd < 0))
	{
		if ((*data)->redirect_state == 1)
		{
			if (dup2((*data)->fd, STDOUT_FILENO) < 0)
				return (-1);
		}
		if ((*data)->redirect_state == 0)
		{
			if (dup2((*data)->fd, STDIN_FILENO) < 0)
				return (-1);
		}
	}
	execute_command(holder, data, envp);
	return (EXIT_SUCCESS);
}

void pipe_case(t_token **tokens, t_data **data, char **envp, t_token_list **token_list)
{
	int pipes = count_pipes(*tokens);
	int end[pipes + 1];
	int i = 0;
	int j = 0;
	pid_t parent;
	t_token_list *current = *token_list;
	int z = 0;
	int prev_fd = 0;
	int status = 0;

	while (j < pipes)
	{
		if (pipe(end + (j * 2)) < 0)
		{
			perror("pipe error!");
			exit(EXIT_FAILURE);
		}
		j++;
	}
	while (i <= (pipes))
	{
		parent = fork();
		if (parent == -1)
			exit(ft_printf("fork error!\n"));
		if (parent == 0)
		{
			// Child Process
			if (i > 0)
			{
				// If not the first command redirect the previous pipe to the stdin
				if (dup2(prev_fd, STDIN_FILENO) < 0)
				{
					perror("error while duplicating fd!\n");
					exit(EXIT_FAILURE);
				}
				close(prev_fd);
			}
			// If not the last command redirect stdout to the current pipe's input
			if (i < pipes)
			{
				// Redirect stdout to the current pipe's input
				if (dup2(end[i * 2 + 1], STDOUT_FILENO) < 0)
				{
					perror("error while duplicating fd!\n");
					exit(EXIT_FAILURE);
				}
			}
			// Closing all the pipes
			while (z < (2 * pipes))
			{
				close(end[z]);
				z++;
			}
			// Setting Redirections
			if (set_redirection(current->head, data) > 0)
				return ;
			//if ((*data)->heredoc_flag)
			//	write(end[1], (*data)->heredoc_content, strlen((*data)->heredoc_content));
			// Command Execution
			status = child_process_pipe(envp, data, current->head);
		}
		else
		{
			// Parent process
			if (i > 0)
			{
				close(prev_fd);
			}
			// Close the current pipe end
			if (i < pipes)
			{
				close(end[i * 2 + 1]);
				prev_fd = end[i * 2];
			}
			waitpid(parent, &status, 0);
		}
		i++;
		current = current->next;
	}
}
