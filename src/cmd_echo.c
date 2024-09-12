/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_echo.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mapichec <mapichec@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/24 17:32:27 by mapichec          #+#    #+#             */
/*   Updated: 2024/08/29 13:46:40 by mapichec         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

// static void	print_doll(char *value, t_data **data)
// {
// 	t_env_list	*node;
// 	char		*str;

// 	node = (*data)->env_list;
// 	str = ft_strtrim()
// }

static void	modify_type(t_token *current, t_token_type num)	
{
	current = current->next;
	while (current->type != TOKEN_EOF && current->type != num)
	{
		current->type = TOKEN_WORD;
		current = current->next;
	}
}

static t_token	*print_in_qt(t_data **data, t_token *current, t_token **tokens)
{
	char	*tmp;
	int		flag;

	tmp = (*data)->input;
	flag = 0;
	while (*tmp != '\0')
	{
		if (*tmp == '\'' && flag == 0)
		{
			flag = 1;
			tmp++;
			while ((*tmp != '\0' || *tmp == '\'') && flag == 1)
			{
				if (current->type == TOKEN_SINGLE_QUOTES)
					modify_type(current, TOKEN_SINGLE_QUOTES);
				current = current->next;
				while (*tmp != '\0' && *tmp == 32)
				{
					ft_putchar(' ');
					tmp++;
				}
				if (current->type == TOKEN_EOF)
					return (current);
				if (current->type != TOKEN_SINGLE_QUOTES)
					ft_printf("%s", current->value);
				while (*tmp != '\0' && *tmp != 32 && *tmp != '\'')
					tmp++;
				if (*tmp == '\'')
					flag = 0;
			}
		}
		if (*tmp == '\"' && flag == 0)
		{
			flag = 1;
			tmp++;
			while ((*tmp != '\0' || *tmp == '\"') && flag == 1)
			{
				if (current->type == TOKEN_DOUBLE_QUOTES)
					modify_type(current, TOKEN_DOUBLE_QUOTES);
				current = current->next;
				while (*tmp != '\0' && *tmp == 32)
				{
					ft_putchar(' ');
					tmp++;
				}
				if (current->type == TOKEN_EOF)
					return (current);
				if (current->type != TOKEN_DOUBLE_QUOTES)
					ft_printf("%s", current->value);
				while (*tmp != '\0' && *tmp != 32 && *tmp != '\"')
					tmp++;
				if (*tmp == '\"')
					flag = 0;
			}
		}
		tmp++;
	}
	return (current->next);	
}

int	echo_cmd(t_data **data, t_token **tokens)
{
	t_token		*current;

	current = (*tokens);
	current = current->next;
	while (current->type != TOKEN_EOF)
	{
		if (current->type == TOKEN_DOUBLE_QUOTES || current->type == TOKEN_SINGLE_QUOTES)
			current = print_in_qt(data, current, tokens);
		else
			ft_printf("%s", current->value);
		current = current->next;
	}
	ft_putchar('\n');
	return ((*data)->err_state = 0, 1);
}
