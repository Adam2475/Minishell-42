/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adapassa <adapassa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/10 14:04:42 by adapassa          #+#    #+#             */
/*   Updated: 2024/08/22 16:19:02 by adapassa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

char	*find_cmd(char *cmd, t_data *data)
{
	int		i;
	char	*tmp;
	char	*holder;

	i = 0;
	while (data->my_paths[i])
	{
		tmp = ft_strjoin(data->my_paths[i], "/");
		holder = ft_strjoin(tmp, cmd);
		if (access(holder, X_OK) == 0)
			return (free(tmp), holder);
		free(tmp);
		free(holder);
		i++;
	}
	write(2, "command not found : ", 20);
	write(2, cmd, ft_strlen(cmd));
	write(2, "\n", 1);
	return (NULL);
}

static int child_process(char *cmd, char **cmd_args, t_data *data, char **envp)
{
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

	// Debug
	// int i = 0;
	// printf("%s\n", cmd);
	// while (cmd_args[i])
	// {
	// 	printf("%s\n", cmd_args[i]);
	// 	i++;
	// }
	// exit(1);

	//ft_printf("proceding to execve: \n");
	execve(cmd, cmd_args, envp);
	return (EXIT_SUCCESS);
}

static int parent_process(char *cmd, char **cmd_args, t_data *data, char **envp)
{
	int status;
	waitpid(-1, &status, 0);
	return (status);
}


static void	execute_command(char **command, t_data *data, char **envp)
{
	char *cmd;
	char **cmd_args;
	char *tmp;
	int	 end[2];
	pid_t parent;
	int status;

	cmd = NULL;
	cmd = find_cmd(command[0], data);
	tmp = ft_strjoin_gnl(command[0], " ");
	int i = 1;
	while (command[i])
	{
		//ft_printf("%s\n", command[i]);
		tmp = ft_strjoin_gnl(tmp, command[i]);
		i++;
	}
	cmd_args = ft_split(tmp, 32);
	// Debug
	//printf("%s\n", cmd);
	//printf("%s\n", cmd_args[0]);

	//printf("------------------- due\n");
	//printf("%s\n", cmd);
	// i = 0;
	// while (cmd_args[i])
	// {
	// 	printf("%s\n", cmd_args[i]);
	// 	i++;
	// }

	parent = fork();
	//ft_printf("%d\n", parent);
	if (parent < 0)
		exit(ft_printf("error with the fork"));
	//ft_printf("%d\n", status);
	if (!parent)
		child_process(cmd, cmd_args, data, envp);
	else
		status = parent_process(cmd, cmd_args, data, envp);
	//ft_printf("%d\n", status);
	//exit(1);
	return ;
}

static int find_redirect(t_token* head)
{
    t_token	*current = head;

    // Traverse the linked list
    while (current != NULL) {
        // Check if the current node's data is '>'
        if (current->type == 4) {
            printf("Found '>' character in the linked list.\n");
            // Uncomment below if you want to stop after finding the first '>'
            return (1);
        }
        // Move to the next node
        current = current->next;
    }
    printf("Finished searching.\n");
	return (0);
}

void	token_parser(t_token **tokens, t_data *data, char **envp)
{
	t_token		*current;
	t_token		*head;
	char		**command;
	int i = 0;
	// int *pipe;

	command = (char **)malloc(sizeof(char *) * 3);
	printf("starting parser: ------------------------->\n");
	current = *tokens;
	head = *tokens;
	while (current->type != TOKEN_EOF)
	{

		// Case for handling redirections
		while (current != NULL)
		{
			if (current->type == TOKEN_REDIRECT_OUT)
			{
				//printf("%s\n%s\n", current->value,"Found '>' character in the linked list.\n");
				current = current->next;
				printf("%s\n%d\n", current->value, current->type);
				data->redirect_state = 1;
				if (current->type == TOKEN_APPENDICE)
				{
					data->fd = open(current->value, O_CREAT | O_RDWR | O_TRUNC, 0644);
					printf("%d\n", data->fd);
				}
			}
			else if (current->type == TOKEN_REDIRECT_IN)
			{
				//printf("%s\n%s\n", current->value,"Found '<' character in the linked list.\n");
				current = current->next;
				printf("%s\n%d\n", current->value, current->type);
				data->redirect_state = 0;
				if (current->type == TOKEN_APPENDICE)
				{
					data->fd = open(current->value, O_RDONLY);
					printf("%d\n", data->fd);
				}
			}
			current = current->next;
		}

		current = head;
		
		if (current->type == 12)
		{
			command[i] = ft_strdup(current->value);
			i++;
			current = current->next;
			while (current->type == 13 || current->type == 1)
			{
				command[i] = ft_strdup(current->value);
				current = current->next;
				////////
				//Debug
				//printf("%s\n", command[i]);
				i++;
			}		
			// handle pipes and split the token list (?)
			// handle redirection before executing the command;
			ft_printf("executing command: ----------------------->\n");
			execute_command(command, data, envp);
			close(data->fd);
			return ;
			//current = current->next->next;
			//continue;
		}
		current = current->next;
	}
}