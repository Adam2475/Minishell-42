/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adapassa <adapassa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/05 15:01:08 by adapassa          #+#    #+#             */
/*   Updated: 2024/08/05 16:52:03 by adapassa         ###   ########.fr       */
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
			exit(1);
			// pipe_token_parser(&data);
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
