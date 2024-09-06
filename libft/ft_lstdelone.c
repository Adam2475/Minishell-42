/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstdelone.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adapassa <adapassa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/03 20:35:19 by adapassa          #+#    #+#             */
/*   Updated: 2024/01/03 20:35:22 by adapassa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_lstdelone(t_list *lst, void (*del)(void*))
{
	(*del)(lst->content);
	free(lst);
}

// lst : is the node to clear
// del : is the address of the function used to delete the content 

void	tkn_delone(t_token **tokens, int num)
{
	t_token	*current;
	t_token	*tmp;

	current = (*tokens);
	tmp = NULL;
	while (current->type != 7)
	{
		if ((int)current->next->type == num)
		{
			free(current->next->value);
			tmp = current;
			current->next = tmp->next;
			free(tmp);
			return ;
		}
		current = current->next;
	}
}
