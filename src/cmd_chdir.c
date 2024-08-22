/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_chdir.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mapichec <mapichec@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/18 16:29:32 by marco             #+#    #+#             */
/*   Updated: 2024/08/22 18:47:44 by mapichec         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

/*
/ * TODO: 
/ *		chdir need to change even the path inside the env var PWD e OLDPWD
/ *
*/

// void	chpwd_env(t_data **data, char *new_path)
// {
// 	t_env_list	*node;

// 	node = (*data)->env_list;
// 	while (strncmp(node->content, "PWD=", 4))
// 		node = node->next;
	
// }

int cd_cmd(char **cmd_args, t_data **data)
{
	int	fd;

	fd = open(cmd_args[1], O_RDONLY, O_WRONLY, O_RDONLY);
	if (data)
		ft_printf("\033[0;91mCD_CMD\033[0;39m\n");
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
	// chpwd_env(data, cmd_args[1]);
	return(close(fd), 1);
}
