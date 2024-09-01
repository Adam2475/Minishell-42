/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adapassa <adapassa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/01 12:47:06 by adapassa          #+#    #+#             */
/*   Updated: 2024/09/01 12:58:28 by adapassa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

void	set_redirection(t_token *tokens, t_data **data)
{
	t_token *current = tokens;

	while (current != NULL)
	{
		if (current->type == TOKEN_REDIRECT_OUT)
		{
			current = current->next;
			(*data)->redirect_state = 1;
			if (current->type == TOKEN_APPENDICE)
				(*data)->fd = open(current->value, O_CREAT | O_RDWR | O_TRUNC, 0644);
		}
		else if (current->type == TOKEN_REDIRECT_IN)
		{
			current = current->next;
			(*data)->redirect_state = 0;
			if (current->type == TOKEN_APPENDICE)
			{
				(*data)->fd = open(current->value, O_RDONLY);
				if ((*data)->fd < 0)
					exit(ft_printf("input file not found!\n"));
			}
		}
		else if (current->type == TOKEN_APPEND)
		{
			current = current->next;
			(*data)->redirect_state = 1;
			if (current->type == TOKEN_APPENDICE)
			{
				ft_printf("setting up the append!\n");
				(*data)->fd = open(current->value, O_WRONLY | O_APPEND | O_CREAT, 0644);
				ft_printf("%d\n", (*data)->fd);
			}
		}
		else if (current->type == TOKEN_HEREDOC)
			exit(printf("found heredoc in the command!\n"));
		current = current->next;
	}
}