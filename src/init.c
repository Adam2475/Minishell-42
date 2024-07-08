/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adapassa <adapassa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/06 18:39:05 by adapassa          #+#    #+#             */
/*   Updated: 2024/07/08 09:03:50 by adapassa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"
#include "../libft/libft.h"

void	init_state(t_data *data, t_token *tokens)
{
	data->state = NORMAL;
	tokens = NULL;
}