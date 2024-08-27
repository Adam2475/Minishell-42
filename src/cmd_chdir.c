/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_chdir.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mapichec <mapichec@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/18 16:29:32 by marco             #+#    #+#             */
/*   Updated: 2024/08/27 16:07:28 by mapichec         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

/*
	TODO:
		- need to understand why if i echo $PWD it gves me 
			back the same path even though I did a chdir
*/

void	chpwd_env(t_data **data, char *new_path, int path_len)
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
	node_old->value = ft_strndup(node->value, ft_strlen(node->value));
	free(node->value);
	node->value = getcwd(new_path, (path_len + 1));
	(*data)->env_list = head;
	return ;
}

int cd_cmd(char **cmd_args, t_data **data)
{
	int		fd;
	int		path_len;
	char	*tmp;

	fd = open(cmd_args[1], O_RDONLY, O_WRONLY, O_RDONLY);
	if (cmd_args[2] && (cmd_args[2][1] != '|' || cmd_args[2][1] != '>'
		|| cmd_args[2][1] != '<'))
		return (close(fd), (*data)->err_state = 1,
			ft_printf("bash: cd: %s: too many arguments\n", cmd_args[1]));
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
	tmp = getcwd(cmd_args[1], UINT_MAX);
	path_len = ft_strlen(tmp);
	chpwd_env(data, cmd_args[1], path_len);
	
	// t_env_list *node = (*data)->env_list;
	// while (node && ft_strncmp(node->var, "PWD=", 4) != 0)
	// {
	// 	node = node->next;
	// }
	// ft_printf("\033[0;91mPWD %s\033[0;39m\n", node->value);

	/* PROVA LISTA ENV*/
	t_env_list *node = (*data)->env_list;
	while (node && ft_strncmp(node->var, "PWD=", 4) != 0)
	{
		node = node->next;
	}
		ft_printf("\033[0;91mPWD %s\033[0;39m\n", node->value);
	return(close(fd), free(tmp), (*data)->err_state = 0, 1);
}
