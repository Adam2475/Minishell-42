/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adapassa <adapassa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/10 14:04:42 by adapassa          #+#    #+#             */
/*   Updated: 2024/07/12 19:35:33 by adapassa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

static t_token resolve_quotes(char *str)
{
	int	i;
	char *tmp;

	i = 0;
	tmp = ft_calloc(, ft_strlen(str - 1));
	// a command will be the first string token
	// not preceded by a special character
	while (i < ft_strlen(str))
	{
		
	}
}

static void	parse_quote_token(char *str, t_data *data, t_token **tokens)
{
	t_token	tmp;

	tmp = NULL;
	tmp = resolve_quotes(str, tokens);
}

static	int	quote_check(char *str)
{
	int	i;

	i = 0;
	// Debug
	//ft_printf("%s\n", str);
	while (i < ft_strlen(str) - 1)
	{
		if (str[i] == DOUBLE_QUOTES)
		{
			return (1);
		}
		i++;
	}
	return (0);
}

void	token_parser(t_data *data, t_token **tokens, char **envp)
{
	t_token *current;

	current = *tokens;
	ft_printf("\nStarting Input Parsing: \n--------------------------------------\n");
	while (current->next != NULL)
	{
		if (quote_check(current->value) > 0)
		{
			ft_printf("Found DQUOTES State, Parsing:\n");
			parse_quote_token(current, data, tokens);
		}
		current = current->next;
	}
	ft_printf("\nPrinting token lists: \n--------------------------------------\n");
}

char	*expand_variable(t_token **tokens, char **envp)
{
	t_token	*current;
	current = *tokens;
	char	*result;
	ft_printf("expanding the variable\n");

	//printf("%s\n", current->value);
	if (current->value[0] == DOLLAR_SIGN)
	{
		ft_printf("found dollar sign!\n");
	}
	return (result);
}