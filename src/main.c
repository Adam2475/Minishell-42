/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adapassa <adapassa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/05 15:01:08 by adapassa          #+#    #+#             */
/*   Updated: 2024/08/22 18:57:46 by adapassa         ###   ########.fr       */
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

static int countPipes(t_token* head)
{
	int count = 0;
	t_token* current = head;

	while (current != NULL)
	{
		if (current->type == TOKEN_PIPE)
		{
			count++;
		}
		current = current->next;
	}
	return (count);
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
	char *holder;

	cmd_args = ft_split(command, 32);
	cmd = cmd_args[0];
	tmp = NULL;
	holder = find_cmd(cmd, data);

	int i = 1;
	while (cmd_args[i])
	{
		tmp = ft_strjoin_gnl(tmp, trim_whitespace(cmd_args[i]));
		i++;
	}
	execve(holder, cmd_args, envp);
	return(EXIT_SUCCESS);
}

static int child_process_pipe(char *command, char **envp, t_data *data, t_token *tokens)
{
	char *holder;
	t_token *new_tokens;
	t_token *tmp;

	//holder = reformat_command(command, token_list);
	//tmp = flatten_token_list(token_list);
	new_tokens = extract_command_and_appendices(tokens); 
	//print_token_lists(*token_list);
	//ft_printf("my command: %s\n", command);
	print_tokens(new_tokens);
	//print_token_lists(token_list);
	printf("prceding to execve: ---------------------------->\n");
	execute_command(command, data, envp);
	return (EXIT_SUCCESS);
}

static int parent_process_pipe(char *command, char **envp, t_data *data)
{
	int status;
	waitpid(-1, &status, 0);
	return (status);
}

char *reformat_command(char *command, t_token_list *token_list)
{
	//apply_redirection(token_list);
	//print_token_lists(*token_list);
	//printf("%s\n", command);
	printf("reformatting the tokens!\n");
	return (NULL);
}

static void pipe_case(t_token **tokens, t_data *data, char **envp, t_token_list **token_list)
{
	int pipes = countPipes(*tokens);
	int end[pipes + 1];
	char **commands;
	int i = 0;
	pid_t parent[pipes + 1];
	t_token_list *current = *token_list;

	commands = ft_split(data->input, '|');
	while (i < (pipes + 1))
	{
		//printf("%s\n", commands[i]);
		parent[i] = fork();
		//printf("%d\n", parent[i]);
		if (!parent[i])
			child_process_pipe(commands[i], envp, data, current->head);
		else
			parent_process_pipe(commands[i], envp, data);
		i++;
		current = current->next;
	}
}
static t_token *create_token(t_token_type type, char *value)
{
    t_token *new_token = (t_token *)malloc(sizeof(t_token));
    if (!new_token)
        return NULL;
    new_token->type = type;
    new_token->value = strdup(value);
    new_token->next = NULL;
    return new_token;
}

static void append_token(t_token **list, t_token *new_token)
{
    if (!*list)
    {
        *list = new_token;
        return;
    }
    t_token *temp = *list;
    while (temp->next)
        temp = temp->next;
    temp->next = new_token;
}

// Function to filter tokens and return the command and its appendices
t_token *extract_command_and_appendices(t_token *tokens)
{
    t_token *result = NULL;
    t_token *current = tokens;
    int command_found = 0;

    while (current)
    {
        if (current->type == TOKEN_COMMAND)
        {
            command_found = 1;
            append_token(&result, create_token(current->type, current->value));
        }
        else if (command_found && (current->type == TOKEN_APPENDICE || current->type == TOKEN_OPTION))
        {
            append_token(&result, create_token(current->type, current->value));
        }
        else if (command_found)
        {
            // Stop processing if command is found and current token is not an appendice
            break;
        }
        current = current->next;
    }

    return result;
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

t_token *flatten_token_list(t_token_list *token_list)
{
    t_token *result = NULL;
    t_token_list *current_list = token_list;

    while (current_list)
    {
        t_token *current_token = current_list->head;
        while (current_token)
        {
            append_token(&result, create_token(current_token->type, current_token->value));
            current_token = current_token->next;
        }
        current_list = current_list->next;
    }

    return result;
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
