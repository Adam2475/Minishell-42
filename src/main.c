/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mapichec <mapichec@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/05 15:01:08 by adapassa          #+#    #+#             */
/*   Updated: 2024/08/24 15:58:12 by mapichec         ###   ########.fr       */
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
	gen_list_env(&data, envp);
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

static int child_process_pipe(char *data, char **envp)
{
	int i = 0;
	ft_printf("hello from %d\n iteration");
	return (0);
}

static void pipe_case(t_token **tokens, t_data *data, char **envp)
{
	int pipes = countPipes(*tokens);
	int end[pipes + 1];
	char **commands;
	int i = 0;

	commands = ft_split(data->input, '|');
	while (i < (pipes + 1))
	{
		child_process_pipe(commands[i++], envp);
	}
}

int main(int argc, char **argv, char **envp)
{
	t_data		data;
	t_token		*tokens;

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
		env_parser(&data, envp);
		if (piper(&tokens) == 0)
			token_parser(&tokens, &data, envp);
		else 
		{
			printf("found pipe case!\n");
			//pipe_splitter(&data, &tokens);
			pipe_case(&tokens, &data, envp);
			exit(1);
			// pipe_token_parser(&data);
		}
		// token_parser(&tokens, &data, envp);
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
