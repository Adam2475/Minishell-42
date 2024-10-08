/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adapassa <adapassa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/11 16:07:52 by adapassa          #+#    #+#             */
/*   Updated: 2024/09/11 18:18:31 by adapassa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

static	int	is_numeric(char *str)
{
	int i = 0;

	if (!str || !str[0])
		return 0;
	if (str[0] == '-')
		i++;
	while (str[i])
	{
		if (!isdigit(str[i]))
			return (0);
		i++;
	}
	return (1);
}

int	unset_env(t_env_list **env, char *var_name)
{
	t_env_list *current = *env;

	var_name = ft_strjoin(var_name, "=");
	while (current && ft_strncmp(current->var, var_name, ft_strlen(var_name)) != 0)
	{
		if (current->next)
			current = current->next;
		else
			break ;
	}
	if (ft_strncmp(current->var, var_name, ft_strlen(var_name)) == 0)
	{
		if (current->pre)
			current->pre->next = current->next;
		else
			*env = current->next;
		if (current->next)
			current->next->pre = current->pre;
		free(current->var);
		free(current->value);
		free(current->content);
		free(current);
		return (1);
	}
	return (0);
}

void cmd_exit(char **args, t_data *data)
{
	int exit_code = 0;

	if (args[1])
	{
		if (!is_numeric(args[1]))
		{
			printf("exit: %s: numeric argument required\n", args[1]);
			exit_code = 255;
		}
		else
		{
			exit_code = ft_atoi(args[1]);
		}
		if (args[2])
		{
			printf("exit: too many arguments\n");
			return;
		}
	}
	free_exit(&data);
	exit(exit_code);
}

