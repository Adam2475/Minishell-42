/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_chdir.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mapichec <mapichec@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/18 16:29:32 by marco             #+#    #+#             */
/*   Updated: 2024/08/21 17:01:09 by mapichec         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

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
	return(close(fd), 1);
}
