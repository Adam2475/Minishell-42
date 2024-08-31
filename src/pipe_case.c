/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_case.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adapassa <adapassa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/26 10:12:13 by adapassa          #+#    #+#             */
/*   Updated: 2024/08/31 14:25:15 by adapassa         ###   ########.fr       */
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
	// if (command && data)
	// {
	// 	//for(int p=0; p < 2 ; p++)
	// 	ft_printf("end_child: %d\n", i);
	// 	ft_printf("end: %d\n", end[i]);
	// }
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

// int parent_process_pipe(char *command, t_token *tokens, char **envp, t_data **data, int end[], int i)
// {
// 	int status;

// 	status = 0;
// 	if (status != 0 && !data)
// 		ft_printf("parent_process_pipe %d\n", status);
// 	waitpid(-1, &status, 0);
// 	if (!command || !tokens || !envp)
// 		return (0);
// 	if (!end)
// 		exit(1);
// 	printf("end_parent: %d\n", i + 1);
// 	return (status);
// }

static void set_redirection(t_token *tokens, t_data **data)
{
	t_token *current = tokens;

	while (current != NULL)
	{
		if (current->type == TOKEN_REDIRECT_OUT)
		{
			current = current->next;
			(*data)->redirect_state = 1;
			if (current->type == TOKEN_APPENDICE)
			{
				(*data)->fd = open(current->value, O_CREAT | O_RDWR | O_TRUNC, 0644);
			}
		}
		else if (current->type == TOKEN_REDIRECT_IN)
		{
			current = current->next;
			(*data)->redirect_state = 0;
			if (current->type == TOKEN_APPENDICE)
			{
				(*data)->fd = open(current->value, O_RDONLY);
				if ((*data)->fd < 0)
				{
					printf("input file not found\n");
					exit(1);
				}
			}
		}
		else if (current->type == TOKEN_APPEND)
			exit(printf("found append in the command!\n"));
		else if (current->type == TOKEN_HEREDOC)
			exit(printf("found heredoc in the command!\n"));
		current = current->next;
	}
}

void pipe_case(t_token **tokens, t_data **data, char **envp, t_token_list **token_list)
{
	int pipes = count_pipes(*tokens);
	int end[pipes + 1];
	//char **commands;
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

	//int x = 0;
	//commands = ft_split((*data)->input, '|');
	j = 1;
	while (i <= (pipes))
	{
		//ft_printf("end: %d\n", end[i]);
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
				close(end[j]);
				z++;
			}
			// Setting Redirections
			set_redirection(current->head, data);
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
