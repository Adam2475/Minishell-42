/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils4.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adapassa <adapassa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/28 17:23:17 by adapassa          #+#    #+#             */
/*   Updated: 2024/08/30 12:58:34 by adapassa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

t_token	*create_token(t_token_type type, char *value)
{
	t_token *new_token = (t_token *)malloc(sizeof(t_token));
	if (!new_token)
		return NULL;
	new_token->type = type;
	new_token->value = strdup(value);
	new_token->next = NULL;
	return new_token;
}

void	append_token(t_token **list, t_token *new_token)
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

t_token_list	*create_token_list_node(t_token *head)
{
	t_token_list *new_node = (t_token_list *)malloc(sizeof(t_token_list));
	if (!new_node)
		return NULL;
	new_node->head = head;
	new_node->next = NULL;
	return new_node;
}

void	env_parser(t_data **data, char **envp)
{
	static int	flag = 0;

	if (flag == 0)
	{
		flag = 1;
		gen_list_env(data, envp);
	}
	(*data)->my_line = retrieve_line(envp);
	if (!(*data)->my_line)
		exit(write(1, "PATH not found\n", 15));
	(*data)->path_from_envp = ft_substr((*data)->my_line, 5, 500);
	(*data)->my_paths = ft_split((*data)->path_from_envp, ':');
}

int	set_token_state(t_token **tokens)
{
	t_token 		*current = *tokens;
	int				i;
	t_token_state	current_state;
	current_state = STATE_NORMAL;
	while (current->next != NULL)
	{
		i = 0;
		while (current->value[i] != '\0')
		{
			if (current->value[i] == '$')
			{
				if (current_state == STATE_DOUBLE_QUOTES)
				{
					current_state = STATE_DOLLAR_DOUBLE_QUOTES;
				}
				else
				{
					current_state = STATE_DOLLAR;
				}
			}
			else if (current->value[i] == '\'')
				current_state = STATE_SINGLE_QUOTES;
			else if (current->value[i] == '\"' && current_state != STATE_DOLLAR_DOUBLE_QUOTES)
				current_state = STATE_DOUBLE_QUOTES;
			i++;
		}
		current->state = current_state;
		current = current->next;
		current_state = STATE_NORMAL;
	}
	return (0);
}
