/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adapassa <adapassa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/10 14:04:42 by adapassa          #+#    #+#             */
/*   Updated: 2024/09/15 17:08:25 by adapassa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

void free_char_array(char **array)
{
	int i;

	i = 0;
	if (array == NULL)
		return;
	while (array[i] != NULL)
	{
		free(array[i]);
		i++;
	}
	free(array);
}

char	*find_cmd(char *cmd, t_data **data)
{
	int		i;
	char	*tmp;
	char	*holder;

	i = 0;
	while ((*data)->my_paths[i])
	{
		tmp = ft_strjoin((*data)->my_paths[i], "/");
		holder = ft_strjoin(tmp, cmd);
		if (access(holder, X_OK) == 0)
			return (free(tmp), holder);
		free(holder);
		i++;
	}
	write(2, "command not found : ", 20);
	write(2, cmd, ft_strlen(cmd));
	write(2, "\n", 1);
	return (NULL);
}

int	conf_man_cmd(char *str)
{
	if (ft_strncmp(str, "cd", ft_strlen(str)) == 0)
		return (1);
	if (ft_strncmp(str, "echo", ft_strlen(str)) == 0)
		return (2);
	if (ft_strncmp(str, "export", ft_strlen(str)) == 0)
		return (3);
	if (ft_strncmp(str, "unset", ft_strlen(str)) == 0)
		return (4);
	if (ft_strncmp(str, "env", ft_strlen(str)) == 0)
		return (5);
	if (ft_strncmp(str, "exit", ft_strlen(str)) == 0)
		return (6);
	if (ft_strncmp(str, "pwd", ft_strlen(str)) == 0)
		return (7);
	else
		return (0);
}

int	manual_cmd(char **cmd_args, t_data **data)
{
	int		i;
	t_data	*tmp;

	tmp = (*data);
	i = 0;
	tmp->cmd = conf_man_cmd(cmd_args[0]);
	if (tmp->cmd == CH_DIR)
		return (cd_cmd(cmd_args, data));
	if (tmp->cmd == ECHO)
		return (echo_cmd(data, &tmp->tokens));
	if (tmp->cmd == EXPORT)
		return (export_cmd(cmd_args, data));
	if (tmp->cmd == UNSET)
		return (unset_env(&tmp->env_list, cmd_args[1]));
	if (tmp->cmd == ENV)
		return (env_cmd(data));
	if (tmp->cmd == EXIT)
		cmd_exit(cmd_args, *data);
	if (tmp->cmd == PWD)
		return (pwd_cmd(data));
	return (0);
}

static int child_process(char *cmd, char **cmd_args, t_data **data, char **envp)
{
	if (!((*data)->fd < 0))
	{
		if ((*data)->redirect_state == 1)
		{
			if (dup2((*data)->fd, STDOUT_FILENO) < 0)
				return (-1);
		}
		if ((*data)->redirect_state == 0)
		{
			if (dup2((*data)->fd, STDIN_FILENO) < 0)
				return (-1);
		}
	}
	execve(cmd, cmd_args, envp);
	return (EXIT_SUCCESS);
}

static int parent_process(void)
{
	int status;

	waitpid(-1, &status, 0);
	return (status);
}

static void	execute_command_single(char **command, t_data **data, char **envp)
{
	char	*cmd;
	pid_t	parent;
	char	*tmp;
	char	**cmd_args;
	int		status;
	char	*holder;

	cmd = NULL;
	tmp = ft_strjoin(command[0], " ");
	if (manual_cmd(command, data))
	{
		free(tmp);
		return;
	}
	cmd = find_cmd(command[0], data);

	int i = 1;
	holder = NULL;
	while (command[i])
	{
		holder = ft_strjoin_gnl(tmp, command[i++]);
		free(tmp);
		tmp = holder;
	}
	cmd_args = ft_split(tmp, 32);
	free(tmp);
	parent = fork();
	if (parent < 0)
	{
		free(cmd_args);
		exit(ft_printf("error with the fork"));
	}
	if (!parent)
	{
		child_process(cmd, command, data, envp);
		free(cmd_args);
		return;
	}
	else
		status = parent_process();
	free(cmd_args);
	return;
}

void	token_parser(t_token **tokens, t_data **data, char **envp)
{
	t_token		*current;
	t_token		*head;
	char		**command;
	int			i;

	i = 0;
	command = (char **)ft_calloc(4, sizeof(char *));
	if (!command)
		free_exit(data);
	command[3] = (char*)ft_calloc(1, 1);
	if (!command[3])
		free_exit(data);
	current = *tokens;
	head = *tokens;	
	while (current->type != TOKEN_EOF)
	{
		while (current != NULL)
		{
			if (current->type == TOKEN_REDIRECT_OUT)
			{
				current = current->next;
				(*data)->redirect_state = 1;
				if (current->type == TOKEN_APPENDICE)
					(*data)->fd = open(current->value, O_CREAT | O_RDWR | O_TRUNC, 0644);
			}
			else if (current->type == TOKEN_REDIRECT_IN)
			{
				current = current->next;
				(*data)->redirect_state = 0;
				if (current->type == TOKEN_APPENDICE)
					(*data)->fd = open(current->value, O_RDONLY);
			}
			else if (current->type == TOKEN_APPEND)
			{
				current = current->next;
				(*data)->redirect_state = 1;
				if (current->type == TOKEN_APPENDICE)
					(*data)->fd = open(current->value, O_WRONLY | O_APPEND | O_CREAT, 0644);
			}
			else if (current->type == TOKEN_HEREDOC)
			{
				current = current->next;
				handle_heredoc(current->value, data);
			}
			current = current->next;
		}
		current = head;
		if (current->type == 12)
		{
			command[i] = ft_strdup(current->value);
			i++;
			current = current->next;
			while (current->type == 13 || current->type == 1 || current->type == 8)
			{
				command[i] = ft_strdup(current->value);
				current = current->next;
				i++;
			}
			execute_command_single(command, data, envp);
			if ((*data)->fd >= 0)
				close((*data)->fd);
			free(command[3]);
			free_char_array(command);
			return ;
		}
		current = current->next;
	}
	free(command[3]);
	free_char_array(command);
}
