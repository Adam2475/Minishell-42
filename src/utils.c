/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adapassa <adapassa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/22 18:17:57 by adapassa          #+#    #+#             */
/*   Updated: 2024/09/11 15:45:51 by adapassa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

char	*trim_whitespace(char *str)
{
	char *end;

	while (*str == 32) str++;
	if (*str == 0)
		return str;
	end = str + strlen(str) - 1;
	while (end > str && *end == 32) end--;
	*(end + 1) = '\0';
	return (str);
}

int	execute_command(char *command, t_data **data, char **envp)
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
	return(execve(holder, cmd_args, envp));
}

void	print_token_lists(t_token_list *list)
{
	while (list)
	{
		t_token *current = list->head;
		printf("Token List:\n");
		while (current)
		{
			printf("Type: %d, Value: %s\n", current->type, current->value);
			current = current->next;
		}
		list = list->next;
		printf("----\n");
	}
}

void	print_tokens(t_token *tokens)
{
	t_token *temp = tokens;
	while (temp)
	{
		printf("Type: %d, Value: %s\n", temp->type, temp->value);
		temp = temp->next;
	}
}

void	append_token_list(t_token_list **list, t_token *head)
{
	t_token_list *new_node = create_token_list_node(head);

	if (!new_node)
		return;
	if (*list == NULL)
		*list = new_node;
	else
	{
		t_token_list *current = *list;
		while (current->next)
			current = current->next;
		current->next = new_node;
	}
}