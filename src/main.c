/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adapassa <adapassa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/05 15:01:08 by adapassa          #+#    #+#             */
/*   Updated: 2024/08/26 11:32:52 by adapassa         ###   ########.fr       */
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

char *reformat_command(char *command, t_token_list *token_list)
{
	//apply_redirection(token_list);
	//print_token_lists(*token_list);
	//printf("%s\n", command);
	printf("reformatting the tokens!\n");
	return (NULL);
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

t_token_list	*create_token_list_node(t_token *head)
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
			token_list = split_tokens_by_pipe(tmp);\
			pipe_case(&tokens, &data, envp, &token_list);
		}
		free_exit(&data);
	}
	return (0);
}
