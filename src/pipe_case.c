/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_case.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adapassa <adapassa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/26 10:12:13 by adapassa          #+#    #+#             */
/*   Updated: 2024/08/28 16:05:20 by adapassa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

static int child_process_pipe(char *command, char **envp, t_data *data, t_token *tokens, int *end, int i)
{
	char *holder;
	t_token *new_tokens;
	t_token *tmp;

	new_tokens = extract_command_and_appendices(tokens);
	holder = token_to_command(new_tokens);

	t_token *current = tokens;

	if (!(data->fd < 0))
	{
		if (data->redirect_state == 1)
		{
			if (dup2(data->fd, STDOUT_FILENO) < 0)
				return (-1);
		}
		if (data->redirect_state == 0)
		{
			if (dup2(data->fd, STDIN_FILENO) < 0)
				return (-1);
		}
	}
	
	if (dup2(data->end[i], STDOUT_FILENO) < 0)
		return (-1);
	
	execute_command(holder, data, envp);
	return (EXIT_SUCCESS);
}

static int parent_process_pipe(char *command, t_token *tokens, char **envp, t_data *data, int *end, int i)
{
	int status;

	waitpid(-1, &status, 0);
	if (dup2(end[i + 1], STDIN_FILENO) < 0)
		return (-1);
	return (status);
}

static void set_redirection(t_token *tokens, t_data *data)
{
	t_token *current = tokens;

	while (current != NULL)
	{
		if (current->type == TOKEN_REDIRECT_OUT)
		{
			current = current->next;
			data->redirect_state = 1;
			if (current->type == TOKEN_APPENDICE)
			{
				data->fd = open(current->value, O_CREAT | O_RDWR | O_TRUNC, 0644);
			}
		}
		else if (current->type == TOKEN_REDIRECT_IN)
		{
			current = current->next;
			data->redirect_state = 0;
			if (current->type == TOKEN_APPENDICE)
			{
				data->fd = open(current->value, O_RDONLY);
				if (data->fd < 0)
				{
					printf("input file not found\n");
					exit(1);
				}
			}
		}
		current = current->next;
	}
}

void pipe_case(t_token **tokens, t_data *data, char **envp, t_token_list **token_list)
{
	int pipes = count_pipes(*tokens);
	int end[pipes + 1];
	char **commands;
	int i = 0;
	int j = 0;
	int status;
	pid_t parent[pipes * 2];
	t_token_list *current = *token_list;

	while (j < pipes)
	{
		if (pipe(end + (j * 2)) < 0)
		{
			perror("pipe error!");
			exit(EXIT_FAILURE);
		}
		j++;
	}
	int x = 0;
	commands = ft_split(data->input, '|');
	while (i < (pipes + 1))
	{
		set_redirection(current->head, data);
		parent[i] = fork();
		if (!parent[i])
			child_process_pipe(commands[i], envp, data, current->head, end, x);
		else
			parent_process_pipe(commands[i], current->head, envp, data, end, x);
		i++;
		x = i * 2;
		current = current->next;
	}
}
