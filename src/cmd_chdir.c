/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_chdir.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mapichec <mapichec@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/18 16:29:32 by marco             #+#    #+#             */
/*   Updated: 2024/08/23 19:03:22 by mapichec         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

/*
/ * TODO: 
/ *		find the way to rewrite pwd and oldpwd but 
		problems with the function
/ *
*/
// char	*special_manage(t_data **data, char *new_path)
// {
	
// }

void	chpwd_env(t_data **data, char *new_path, int path_len)
{
	t_env_list	*node;
	t_env_list	*node_old;
	char		*tmp_str;

	node = (*data)->env_list;
	while (ft_strncmp(node->var, "PWD=", 4) != 0)
		node = node->next;
	node_old = node;
	while (ft_strncmp(node->var, "OLDPWD=", 7) != 0)
		node_old = node_old->next;
	// if (new_path[0] != '~' || new_path[0] != '/' || new_path[0] != '.'
	// 	|| new_path[0] != '\0')
	// 	tmp_str = special_manage(data, new_path);
	// else
	// 	tmp_str = ft_strjoin(node->value, new_path);
	free(node_old->value);
	node_old->value = ft_strndup(node->value, ft_strlen(node->value));
	// free(node->value);
	// node->value = tmp_str;
	free(node->value);
	node->value = getcwd(new_path, path_len);
	ft_printf("\033[0;91mPWD %s && OLDPWD %s\033[0;39m\n", node->value, node_old->value);
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
		return (close(fd),
			ft_printf("bash: cd: %s: too many arguments\n", cmd_args[1]));
	if (chdir(cmd_args[1]) != 0)
	{
		if (errno == ENOENT)
			return (close(fd),
			ft_printf("bash: cd: %s: No such file or directory\n", cmd_args[1]));
		else if (errno == ENOTDIR)
			return (close(fd),
			ft_printf("bash: cd: %s: Not a directory\n", cmd_args[1]));
	}
	// ft_printf("\033[0;91mCD_CMD\033[0;39m\n");
	// tmp = getcwd(cmd_args[1], MAX_SIZE_T);
	// path_len = ft_strlen(tmp);
	// chpwd_env(data, cmd_args[1], path_len);
	// exit(0);
	return(close(fd), free(tmp), 1);
}
