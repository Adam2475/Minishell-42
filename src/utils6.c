/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils6.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adapassa <adapassa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/30 08:32:03 by adapassa          #+#    #+#             */
/*   Updated: 2024/09/01 12:44:33 by adapassa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

void	set_data_zero(t_data *data)
{
	data->my_line = NULL;
	data->my_paths = NULL;
	data->path_from_envp = NULL;
	data->tokens = NULL;
	data->env_list = NULL;
}

void	free_exit(t_data **data)
{
	clear_history();
	if ((*data)->my_line)
		free((*data)->my_line);
	free((*data)->path_from_envp);
	free_char_array((*data)->my_paths);
	if ((*data)->tokens)
		free_list(&(*data)->tokens);
	free_list_env(&(*data)->env_list);
	free((*data));
	exit(0);
}

void	free_list_env(t_env_list **head)
{
	t_env_list	*temp;

	if (head == NULL || *head == NULL)
		return ;
	while (*head != NULL)
	{
		temp = *head;
		if (temp->next)
			*head = (*head)->next;
		else
		{
			free(temp->value);
			free(temp->var);
			free(temp->content);
			free(temp);
			break ;
		}
		free(temp->value);
		free(temp->var);
		free(temp->content);
		free(temp);
	}
	*head = NULL;
}

void	free_list(t_token **head)
{
	t_token	*temp;

	if (!head || !*head)
		return ;
	if ((*head)->type == TOKEN_EOF || (*head)->type == TOKEN_NEUTRO)
	{
		free((*head));
		*head = NULL;
		return ;
	}
	while (*head != NULL)
	{
		temp = *head;
		if (temp->next != NULL && temp->type != TOKEN_EOF)
			*head = (*head)->next;
		else if (temp)
		{
			free(temp->value);
			free(temp);
			break ;
		}
		free(temp->value);
		free(temp);
	}
	*head = NULL;
}
