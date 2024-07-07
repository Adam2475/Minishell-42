/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adapassa <adapassa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/20 19:30:25 by adapassa          #+#    #+#             */
/*   Updated: 2023/12/20 19:30:54 by adapassa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/*
    The  strdup() function returns a pointer to a new string which is a du‐
    plicate of the string s.  Memory for the new string  is  obtained  with
    malloc(3), and can be freed with free(3).

    On  success,  the strdup() function returns a po
    inter to the duplicated
    string.  It returns NULL if insufficient memory 
    was available, with er‐
    rno set to indicate the cause of the error.
*/

char	*ft_strdup(const char *s)
{
	char	*pt1;
	int		l;

	l = ft_strlen(s);
	pt1 = malloc(sizeof(char) * (l + 1));
	if (!pt1)
		return (0);
	ft_memcpy(pt1, s, l);
	pt1[l] = '\0';
	return (pt1);
}
