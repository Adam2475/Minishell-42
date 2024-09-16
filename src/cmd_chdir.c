/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_chdir.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adapassa <adapassa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/18 16:29:32 by marco             #+#    #+#             */
/*   Updated: 2024/09/15 17:55:06 by adapassa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

void	chpwd_env(t_data **data, char *new_path)
{
	t_env_list	*node;
	t_env_list	*node_old;
	char		*tmp_str;

	node = (*data)->env_list;
	while (node && ft_strncmp(node->var, "PWD=", 4) != 0)
		node = node->next;
	node_old = (*data)->env_list;
	while (node_old && ft_strncmp(node_old->var, "OLDPWD=", 7) != 0)
		node_old = node_old->next;
	free(node_old->value);
	free(node_old->content);
	node_old->value = ft_strndup(node->value, ft_strlen(node->value));
	node_old->content = ft_strjoin(node_old->var, node->value);
	free(node->value);
	free(node->content);
	tmp_str = getcwd(new_path, UINT_MAX);
	node->value = ft_strndup(tmp_str, ft_strlen(tmp_str));
	node->content = ft_strjoin(node->var, node->value);
	return ;
}

int cd_cmd(char **cmd_args, t_data **data)
{
	t_env_list	*node;
	t_token		*token;
	
	token = (*data)->tokens->next;
	if (token->next->value && (token->next->value[1] != '|'
		|| token->next->value[1] != '>'
		|| token->next->value[1] != '<'))
		return (err_state = 1,	ft_printf("bash: cd: %s: too many arguments\n", token->value));
	node = (*data)->env_list;
	if (!token->value || token->value[0] == '~')
	{
		while (node && ft_strncmp(node->var, "HOME=", 5) != 0)
			node = node->next;
		token->value = ft_strndup(node->value, ft_strlen(node->value));
	}
	if (chdir(token->value) != 0)
	{
		if (errno == ENOENT)
			return (err_state = errno, ft_printf("bash: cd: %s: No such file or directory\n", token->value));
		else if (errno == ENOTDIR)
			return (err_state = errno, ft_printf("bash: cd: %s: Not a directory\n", token->value));
	}
	chpwd_env(data, token->value);
	return(err_state = 0, 1);
}
