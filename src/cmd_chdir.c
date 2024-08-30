/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_chdir.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mapichec <mapichec@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/18 16:29:32 by marco             #+#    #+#             */
/*   Updated: 2024/08/28 17:53:29 by mapichec         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

/*
	TODO:
		- need to understand why if i echo $PWD it gves me 
			back the same path even though I did a chdir
*/

void	chpwd_env(t_data **data, char *new_path)
{
	t_env_list	*head;
	t_env_list	*node;
	t_env_list	*node_old;
	char		*tmp_str;

	head = (*data)->env_list;
	node = (*data)->env_list;
	while (node && ft_strncmp(node->var, "PWD=", 4) != 0)
		node = node->next;
	(*data)->env_list = head;
	node_old = (*data)->env_list;
	while (node_old && ft_strncmp(node_old->var, "OLDPWD=", 7) != 0)
		node_old = node_old->next;
	free(node_old->value);
	node_old->value = ft_strdup(node->value);
	free(node->value);
	tmp_str = getcwd(new_path, UINT_MAX);
	node->value = ft_strndup(tmp_str, ft_strlen(tmp_str));
	free(tmp_str);
	(*data)->env_list = head;
	return ;
}

int cd_cmd(char **cmd_args, t_data **data)
{
	int			fd;
	t_env_list	*node;

	fd = open(cmd_args[1], O_RDONLY, O_WRONLY, O_RDONLY);
	if (cmd_args[2] && (cmd_args[2][1] != '|' || cmd_args[2][1] != '>'
		|| cmd_args[2][1] != '<'))
		return (close(fd), (*data)->err_state = 1,
			ft_printf("bash: cd: %s: too many arguments\n", cmd_args[1]));
	node = (*data)->env_list;
	if (!cmd_args[1] || cmd_args[1][0] == '~')
	{
		while (node && ft_strncmp(node->var, "HOME=", 5) != 0)
			node = node->next;
		cmd_args[1] = ft_strdup(node->value);
	}
	if (chdir(cmd_args[1]) != 0)
	{
		if (errno == ENOENT)
			return (close(fd), (*data)->err_state = errno,
			ft_printf("bash: cd: %s: No such file or directory\n", cmd_args[1]));
		else if (errno == ENOTDIR)
			return (close(fd), (*data)->err_state = errno,
			ft_printf("bash: cd: %s: Not a directory\n", cmd_args[1]));
	}
	ft_printf("\033[0;91mCD_CMD\033[0;39m\n");
	chpwd_env(data, cmd_args[1]);
	
	// t_env_list *node = (*data)->env_list;
	// while (node && ft_strncmp(node->var, "PWD=", 4) != 0)
	// {
	// 	node = node->next;
	// }
	// ft_printf("\033[0;91mPWD %s\033[0;39m\n", node->value);

	/* PROVA LISTA ENV*/
	node = (*data)->env_list;
	while (node && ft_strncmp(node->var, "PWD=", 4) != 0)
	{
		node = node->next;
	}
	ft_printf("\033[0;91mPWD %s\033[0;39m\n", node->value);
	return(close(fd), (*data)->err_state = 0, 1);
}
