/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adapassa <adapassa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/01 08:38:23 by adapassa          #+#    #+#             */
/*   Updated: 2024/09/05 18:03:56 by adapassa         ###   ########.fr       */
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

// void handle_heredoc(const char *delimiter)
// {
// 	char buffer[BUFFER_SIZE];
// 	ssize_t bytes_read;
// 	int tmp_fd;
// 	char *newline;

// 	// Create a temporary file for storing heredoc content
// 	tmp_fd = open("tmp_heredoc.txt", O_CREAT | O_WRONLY | O_TRUNC, 0600);
// 	if (tmp_fd < 0)
// 	{
// 		perror("open");
// 		exit(EXIT_FAILURE);
// 	}

// 	while (1)
// 	{
// 		// Prompt for input
// 		write(STDOUT_FILENO, "heredoc> ", 9);

// 		// Read input from stdin
// 		bytes_read = read(STDIN_FILENO, buffer, BUFFER_SIZE - 1);
// 		if (bytes_read < 0)
// 		{
// 			perror("read");
// 			close(tmp_fd);
// 			unlink("tmp_heredoc.txt");
// 			exit(EXIT_FAILURE);
// 		}

// 		// Null-terminate the string
// 		buffer[bytes_read] = '\0';

// 		// Remove the newline character
// 		newline = strchr(buffer, '\n');
// 		if (newline)
// 		{
// 			*newline = '\0';
// 		}

// 		// Check if the input matches the delimiter
// 		if (strcmp(buffer, delimiter) == 0)
// 		{
// 			break;
// 		}

// 		// Write the input to the temporary file
// 		write(tmp_fd, buffer, strlen(buffer));
// 		write(tmp_fd, "\n", 1);
// 	}

// 	// Close the temporary file
// 	close(tmp_fd);

// 	// Fork and execute a command using the heredoc as input
// 	pid_t pid = fork();
// 	if (pid == 0)
// 	{
// 		// Child process
// 		tmp_fd = open("tmp_heredoc.txt", O_RDONLY);
// 		if (tmp_fd < 0)
// 		{
// 			perror("open");
// 			exit(EXIT_FAILURE);
// 		}

// 		// Redirect stdin to the temporary file
// 		dup2(tmp_fd, STDIN_FILENO);
// 		close(tmp_fd);

// 		// Replace with the actual command you want to execute
// 		execlp("cat", "cat", NULL);
// 		perror("execlp");
// 		exit(EXIT_FAILURE);
// 	}
// 	else if (pid > 0)
// 	{
// 		// Parent process
// 		wait(NULL);
// 	} 
// 	else
// 	{
// 		perror("fork");
// 		exit(EXIT_FAILURE);
// 	}

// 	// Cleanup: remove the temporary file
// 	unlink("tmp_heredoc.txt");
// }

// void handle_heredoc(char *delimiter, t_data **data)
// {
//     char *line;
//     int heredoc_fd;
//     char *tempfile = "/tmp/minishell_heredoc.tmp"; // Temporary file for storing heredoc input

//     // Open temporary file for heredoc input
//     heredoc_fd = open(tempfile, O_CREAT | O_RDWR | O_TRUNC, 0600);
//     if (heredoc_fd < 0)
//     {
//         perror("Failed to open heredoc temporary file");
//         return;
//     }

//     while (1)
//     {
//         // Prompt the user and read input
//         line = readline("> ");
//         if (!line || strcmp(line, delimiter) == 0)
//         {
//             free(line);
//             break;
//         }

//         // Write the line to the heredoc file
//         write(heredoc_fd, line, strlen(line));
//         write(heredoc_fd, "\n", 1);
//         free(line);
//     }

//     // Reset file descriptor to the beginning of the file
//     lseek(heredoc_fd, 0, SEEK_SET);

//     // Store the heredoc file descriptor in `data`
//     (*data)->fd = heredoc_fd;
// }

void handle_heredoc(char *delimiter, t_data **data)
{
	char *line;
	int heredoc_fd;
	char *tempfile = "/tmp/minishell_heredoc.tmp"; // Temporary file for storing heredoc input

	// Open temporary file for heredoc input
	heredoc_fd = open(tempfile, O_CREAT | O_RDWR | O_TRUNC, 0600);
	if (heredoc_fd < 0)
	{
		perror("Failed to open heredoc temporary file");
		return;
	}

	while (1)
	{
		// Prompt the user and read input
		line = readline("> ");
		//printf("Readline input: '%s', Delimiter: '%s'\n", line, delimiter);
		if (!line || strcmp(line, delimiter) == 0)
		{
			free(line);
			break;
		}

		// Write the line to the heredoc file
		write(heredoc_fd, line, strlen(line));
		write(heredoc_fd, "\n", 1);
		free(line);
	}

	// Reset file descriptor to the beginning of the file
	lseek(heredoc_fd, 0, SEEK_SET);

	// Store the heredoc file descriptor in `data`
	(*data)->fd = heredoc_fd;

	if (dup2((*data)->fd, STDIN_FILENO) < 0)
		exit(printf("heredoc redirection error!\n"));
}