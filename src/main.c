/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adapassa <adapassa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/05 15:01:08 by adapassa          #+#    #+#             */
/*   Updated: 2024/08/20 17:42:53 by adapassa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

static	char	*retrieve_line(char **envp)
{
	int	i;

	i = 0;
	while (envp[i] != NULL)
	{
		if (ft_strnstr(envp[i], "PATH=", 5))
			return (ft_strdup(envp[i]));
		i++;
	}
	return (NULL);
}

static void	env_parser(t_data *data, char **envp)
{
	data->my_line = retrieve_line(envp);
	if (!data->my_line)
		exit(write(1, "PATH not found\n", 15));
	data->path_from_envp = ft_substr(data->my_line, 5, 500);
	data->my_paths = ft_split(data->path_from_envp, ':');

	/////////
	// Debug
	// printf("%s\n", data->path_from_envp);
	// exit(0);
}

static int piper(t_token **tokens)
{
	t_token *current = *tokens;
	
	while (current->type != TOKEN_EOF)
	{
		if (current->type == TOKEN_PIPE)
			return (1);
		current = current->next;
	}
	return (0);
}


// Function to count occurrences of '|' in linked list nodes
static int countPipes(t_token* head)
{
    int count = 0;
    t_token* current = head;
    
    while (current != NULL) {
        if (current->type == TOKEN_PIPE) {
            count++;
        }
        current = current->next;
    }
    
    return count;
}

static char *trim_whitespace(char *str)
{
    char *end;

    // Trim leading spaces (ASCII 32)
    while (*str == 32) str++;

    // If all spaces or an empty string
    if (*str == 0)  
        return str;

    // Trim trailing spaces (ASCII 32)
    end = str + strlen(str) - 1;
    while (end > str && *end == 32) end--;

    // Write new null terminator character
    *(end + 1) = '\0';

    return str;
}


static int execute_command(char *command, t_data *data, char **envp)
{
	char *cmd;
	char **cmd_args;
	char *tmp;

	cmd_args = ft_split(command, 32);
	cmd = cmd_args[0];
	find_cmd(cmd, data);
	tmp = NULL;

	int i = 1;
	while (cmd_args[i])
	{
		tmp = ft_strjoin_gnl(tmp, trim_whitespace(cmd_args[i]));
		//printf("%s\n", tmp);
		i++;
	}

	free(cmd_args);
	cmd_args = ft_split(tmp, 32);
	/////////
	// Debug
	//printf("%s\n", cmd);
	//printf("%s\n", command);

	//printf("%s\n", cmd);
	i = 0;
	while (cmd_args[i])
	{
		printf("%s\n", cmd_args[i]);
		i++;
	}

	printf ("%d\n", execve(cmd, cmd_args, envp));
	return(EXIT_SUCCESS);
}

static int child_process_pipe(char *command, char **envp, t_data *data)
{
	static int i = 1;
	////////
	//Debug
	ft_printf("Process no: %d\n", i);
	i++;
	//ft_printf("my command: %s\n", command);
	execute_command(command, data, envp);
	return (EXIT_SUCCESS);
}

static int parent_process_pipe(char *command, char **envp, t_data *data)
{
	int status;
	waitpid(-1, &status, 0);
	return (status);
}

static void print_token_lists(t_token_list *list) {
    while (list) {
        t_token *current = list->head;
        printf("Token List:\n");
        while (current) {
            printf("Type: %d, Value: %s\n", current->type, current->value);
            current = current->next;
        }
        list = list->next;
        printf("----\n");
    }
}

static void pipe_case(t_token **tokens, t_data *data, char **envp, t_token_list **token_list)
{
	int pipes = countPipes(*tokens);
	int end[pipes + 1];
	char **commands;
	int i = 0;
	pid_t parent[pipes + 1];

	commands = ft_split(data->input, '|');
	while (i < (pipes + 1))
	{
		parent[i] = fork();
		//printf("%d\n", parent[i]);
		if (!parent[i])
			child_process_pipe(commands[i], envp, data);
		else
			parent_process_pipe(commands[i], envp, data);
		i++;																																																																										
	}
	//exit(1);
}

static t_token_list	*create_token_list_node(t_token *head)
{
	t_token_list *new_node = (t_token_list *)malloc(sizeof(t_token_list));
	if (!new_node)
		return NULL;
	new_node->head = head;
	new_node->next = NULL;
	return new_node;
}

static void	append_token_list(t_token_list **list, t_token *head)
{
	t_token_list *new_node = create_token_list_node(head);
	if (!new_node)
		return;

	if (*list == NULL) {
		*list = new_node;
	} else {
		t_token_list *current = *list;
		while (current->next)
			current = current->next;
		current->next = new_node;
	}
}


static t_token_list	*split_tokens_by_pipe(t_token *tokens)
{
	t_token_list	*result = NULL;
	t_token			*current = tokens;
	t_token			*segment_start = tokens;
	t_token			*prev = NULL;

	while (current) {
		if (current->type == TOKEN_PIPE) {
			if (prev)
				prev->next = NULL;  // End the current segment

			append_token_list(&result, segment_start);

			segment_start = current->next;  // Start of the new segment
		}
		prev = current;
		current = current->next;
	}

	// Append the last segment
	if (segment_start)
		append_token_list(&result, segment_start);

	return result;
}

static t_token *copy_token(t_token *token)
{
    if (!token)
        return NULL;

    t_token *new_token = (t_token *)malloc(sizeof(t_token));
    if (!new_token)
        return NULL;

    new_token->type = token->type;
    new_token->value = strdup(token->value);  // Copy the string value
    new_token->next = NULL;

    return new_token;
}


static t_token *copy_token_list(t_token *tokens)
{
    if (!tokens)
        return NULL;

	int i = 0;
    t_token *new_list = NULL;
    t_token *last_copied = NULL;
    t_token *current = tokens;
	int total = ft_lstsize_token(tokens);

    while (current && i < (total - 1)) {
        t_token *new_token = copy_token(current);

        if (!new_list) {
            new_list = new_token;
        } else {
            last_copied->next = new_token;
        }

        last_copied = new_token;
        current = current->next;
		i++;
    }

    return new_list;
}

int main(int argc, char **argv, char **envp)
{
	t_data		data;
	t_token		*tokens;
	t_token_list *token_list;

	// I need to create a deep copy first
	t_token *tmp;

	while (1)
	{
		data.input = NULL;
		data.input = readline("myprompt$ ");
		data.fd = -1;
		//data.state = NORMAL;
	
		if (!data.input)
			exit(1);
		
		tokens = tokenize_string(&data);
		token_reformatting(&tokens);
		tmp = copy_token_list(tokens);
		env_parser(&data, envp);
		if (piper(&tokens) == 0)
			token_parser(&tokens, &data, envp);
		else 
		{
			//printf("found pipe case!\n");
			token_list = split_tokens_by_pipe(tmp);
			//print_token_lists(token_list);
			//exit(1);
			pipe_case(&tokens, &data, envp, &token_list);
			//exit(1);
			// pipe_token_parser(&data);
			//return (printf("terminato correttamente"));
		}
		//token_parser(&data, &tokens, envp);
		//exit(1);
		printf("debug: -------------------------------->\n");
		t_token	*head = tokens;
		//Debug
		while (tokens)
		{
			printf("%d : %s\n", tokens->type, tokens->value);
			tokens = tokens->next;
		}
		// resets the list pointer to it's head
		tokens = head;
		// Free and exit program
		free_exit(&data);
	}
	return (0);
}
