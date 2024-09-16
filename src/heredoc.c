/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adapassa <adapassa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/01 08:38:23 by adapassa          #+#    #+#             */
/*   Updated: 2024/09/11 15:49:21 by adapassa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

// [CMD - optional] << DELIMITER
// Input lines
// DELIMITER

// The command above means “Take everything after “<< DELIMITER”
// until you find a line that only contains `DELIMITER`,
// and pass it as input to `[CMD]`”. In this case,
// ` Input Line` will be passed as input to `[CMD]`.

// line 1 ├── cat << EOF
// line 2 ├── Hello
// line 3 ├── World
// line 4 └── EOF

// Line 1: When you enter the first line into your terminal you get into the mode where heredoc expects your input streams.
// Lines 2 & 3: This mode takes multilines of inputs. In this case, Hello and then World, until the EOF is encountered.
// Line 4: Finally, the EOF that ends the subshell process created by the heredoc

void handle_heredoc(char *delimiter, t_data **data)
{
	char *line;
	int heredoc_fd;
	char *tempfile = "/tmp/minishell_heredoc.tmp";

	heredoc_fd = open(tempfile, O_CREAT | O_RDWR | O_TRUNC, 0600);
	if (heredoc_fd < 0)
	{
		perror("Failed to open heredoc temporary file");
		return;
	}

	while (1)
	{
		line = readline("> ");
		if (!line || ft_strncmp(line, delimiter, ft_strlen(line)) == 0)
		{
			free(line);
			break;
		}
		write(heredoc_fd, line, strlen(line));
		write(heredoc_fd, "\n", 1);
		free(line);
	}
	close(heredoc_fd);
	heredoc_fd = open(tempfile, O_RDONLY);
	if (heredoc_fd < 0)
	{
		perror("Failed to reopen heredoc temporary file");
		return;
	}
	(*data)->fd = heredoc_fd;
	if (dup2((*data)->fd, STDIN_FILENO) < 0)
	{
		perror("heredoc redirection error");
		exit(1);
	}
}
