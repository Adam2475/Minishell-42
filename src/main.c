/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adapassa <adapassa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/05 15:01:08 by adapassa          #+#    #+#             */
/*   Updated: 2024/07/08 11:22:39 by adapassa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

int main(int argc, char **argv, char **envp)
{
	t_data		data;
	t_token		*tokens;

	data.input = readline("$ ");
	data.state = NORMAL;

	if (!data.input)
		exit(1);
	
	tokens = tokenize_string(&data);
	// Debug
	while (tokens)
	{
		printf("%d : %s\n", tokens->type, tokens->value);
		tokens = tokens->next;
	}
	parse_input(&data);
	// Free and exit program
	free_exit(&data);
	return (0);
}
