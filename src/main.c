/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adapassa <adapassa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/05 15:01:08 by adapassa          #+#    #+#             */
/*   Updated: 2024/07/06 15:30:45 by adapassa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

int main(int argc, char **argv, char **envp)
{
	t_data data;

	data.input = readline("$ ");
	data.state = NORMAL;

	if (!data.input)
		exit(1);
	
	tokenize_string(&data);
	parse_input(&data);
	// Free and exit program
	free_exit(&data);
	return (0);
}
