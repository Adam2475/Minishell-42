/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mapichec <mapichec@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/06 18:39:05 by adapassa          #+#    #+#             */
/*   Updated: 2024/08/27 15:52:36 by mapichec         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"
#include "../libft/libft.h"

void	init_state(t_data **data, t_token **tokens)
{
	(*data)->state = NORMAL;
	*tokens = NULL;
}