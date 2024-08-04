/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adapassa <adapassa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/10 14:04:42 by adapassa          #+#    #+#             */
/*   Updated: 2024/08/04 18:03:46 by adapassa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"
//echo lyc < ciao | cat -e > merda

// static t_token resolve_quotes(char *str)
// {
// 	int	i;
// 	char *tmp;

// 	i = 0;
// 	tmp = ft_calloc(, ft_strlen(str - 1));
// 	// a command will be the first string token
// 	// not preceded by a special character
// 	while (i < ft_strlen(str))
// 	{
		
// 	}
// }

// static void	parse_quote_token(char *str, t_data *data, t_token **tokens)
// {
// 	t_token	tmp;

// 	tmp = NULL;
// 	tmp = resolve_quotes(str, tokens);
// }

// static	int	quote_check(char *str)
// {
// 	int	i;

// 	i = 0;
// 	// Debug
// 	//ft_printf("%s\n", str);
// 	while (i < ft_strlen(str) - 1)
// 	{
// 		if (str[i] == DOUBLE_QUOTES)
// 		{
// 			return (1);
// 		}
// 		i++;
// 	}
// 	return (0);
//  }

// void	token_parser(t_data *data, t_token **tokens, char **envp)
// {
// 	t_token *current;

// 	current = *tokens;
// 	ft_printf("\nStarting Input Parsing: \n--------------------------------------\n");
// 	while (current->next != NULL)
// 	{
// 		if (quote_check(current->value) > 0)
// 		{
// 			ft_printf("Found DQUOTES State, Parsing:\n");
// 			parse_quote_token(current, data, tokens);
// 		}
// 		current = current->next;
// 	}
// 	ft_printf("\nPrinting token lists: \n--------------------------------------\n");
// }

// char	*expand_variable(t_token **tokens, char **envp)
// {
// 	t_token	*current;
// 	current = *tokens;
// 	char	*result;
// 	ft_printf("expanding the variable\n");

// 	//printf("%s\n", current->value);
// 	if (current->value[0] == DOLLAR_SIGN)
// 	{
// 		ft_printf("found dollar sign!\n");
// 	}
// 	return (result);
// }

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
	if (dup2(data->fd, STDOUT_FILENO) < 0)
		return (-1);
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

	//printf("hello motherfucker");

	//if (pipe(end) < 0)
	//	exit(ft_printf("pipe init error!"));
	
	cmd = NULL;
	cmd = find_cmd(command[0], data);
	tmp = ft_strjoin_gnl(command[0], " ");

	// ft_printf("%s\n", cmd);
	// ft_printf("%s\n", tmp);
	//ft_printf("%s\n", command[0]);
	//ft_printf("%s\n", command[1]);
	//ft_printf("%s\n", command[2]);
	//ft_printf("%s\n", command[3]);
	
	//exit(1);
	int i = 1;
	while (command[i] != NULL)
	{
		//ft_printf("%s\n", command[i]);
		tmp = ft_strjoin_gnl(tmp, command[i]);
		i++;
	}
	cmd_args = ft_split(tmp, 32);
	// Debug
	 //printf("%s\n", cmd);
	 //printf("%s\n", cmd_args[0]);

	parent = fork();

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

   		while (current != NULL)
		{
        // Check if the current node's data is '>'
			if (current->type == 4)
			{
				printf("%s\n%s\n", current->value,"Found '>' character in the linked list.\n");
				current = current->next;
				printf("%s\n%d\n", current->value, current->type);
				if (current->type == TOKEN_APPENDICE)
				{
					printf("ciao cane\n");
					data->fd = open(current->value, O_CREAT | O_RDWR | O_TRUNC, 0644);
					printf("%d\n", data->fd);
					// if (dup2(fd, STDOUT_FILENO) < 0)
					// 	exit(printf("dup2 error!\n"));
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
				///Debug
				//printf("%s\n", command[i]);
				i++;
			}

			
			// handle pipes and split the token list (?)
			// handle redirection before executing the command;
			execute_command(command, data, envp);
			close(data->fd);
			return ;
			//current = current->next->next;
			//continue;
		}
		if (current->type == TOKEN_REDIRECT_IN || current->type == TOKEN_REDIRECT_OUT)
		{
			printf("ititializing redirection\n");
			command[0] = current->value;
			command[1] = current->next->value;
			printf("%s\n", command[0]);
			printf("%s\n", command[1]);
			current = current->next->next;
			continue;
		}
		current = current->next;
	}
}