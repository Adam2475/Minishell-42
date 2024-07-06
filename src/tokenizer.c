/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adapassa <adapassa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/06 14:14:27 by adapassa          #+#    #+#             */
/*   Updated: 2024/07/06 18:39:02 by adapassa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

int		lexer_control(t_data *data, int j)
{
	int	i;
	t_data *endpoint;

	endpoint = data;
	i = j;
	if (endpoint->input[i] == DOUBLE_QUOTES)
	{
		data->state = STATE_DOUBLE_QUOTES;
		ft_printf("Actual lexer state :%d\n", data->state);
	}
	else if (endpoint->input[i] == SINGLE_QUOTES)
	{
		data->state = STATE_DOUBLE_QUOTES;
		ft_printf("Actual lexer state :%d\n", data->state);
	}
}

void	tokenize_string(t_data *data)
{
	int		i;
	int		j;
	char	*buffer;

	i = 0;
	j = 0;
	init_state(data);
	while (data->input[i] != '\0')
	{

		// if (data->input[i] == '(' || data->input[i] == '|'
		// 		|| data->input[i] == '<' || data->input[i] == '>'
		// 		|| data->input[i] == '<<' || data->input[i] == '>>'
		// 		|| data->input[i] == '&&' || data->input[i] == '||')
		// 	special_cases_lexer(data);
		// funny behaviour

		lexer_control(data, i);
	
		ft_printf(&data->input[j]);
		ft_printf(&data->input[i]);
		j++;
		i++;
	}
}

void	special_cases_lexer(t_data *data)
{
	printf("found special case");
}