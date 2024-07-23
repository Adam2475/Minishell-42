/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adapassa <adapassa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/10 14:04:42 by adapassa          #+#    #+#             */
/*   Updated: 2024/07/23 17:51:19 by adapassa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"
//echo lyc < ciao | cat -e > merda

// static t_token resolve_quotes(char *str)
// {
// 	int	i;
// 	char *tmp;

// 	i = 0;
// 	tmp = ft_calloc(, ft_strlen(str - 1));
// 	// a command will be the first string token
// 	// not preceded by a special character
// 	while (i < ft_strlen(str))
// 	{
		
// 	}
// }

// static void	parse_quote_token(char *str, t_data *data, t_token **tokens)
// {
// 	t_token	tmp;

// 	tmp = NULL;
// 	tmp = resolve_quotes(str, tokens);
// }

// static	int	quote_check(char *str)
// {
// 	int	i;

// 	i = 0;
// 	// Debug
// 	//ft_printf("%s\n", str);
// 	while (i < ft_strlen(str) - 1)
// 	{
// 		if (str[i] == DOUBLE_QUOTES)
// 		{
// 			return (1);
// 		}
// 		i++;
// 	}
// 	return (0);
//  }

// void	token_parser(t_data *data, t_token **tokens, char **envp)
// {
// 	t_token *current;

// 	current = *tokens;
// 	ft_printf("\nStarting Input Parsing: \n--------------------------------------\n");
// 	while (current->next != NULL)
// 	{
// 		if (quote_check(current->value) > 0)
// 		{
// 			ft_printf("Found DQUOTES State, Parsing:\n");
// 			parse_quote_token(current, data, tokens);
// 		}
// 		current = current->next;
// 	}
// 	ft_printf("\nPrinting token lists: \n--------------------------------------\n");
// }

// char	*expand_variable(t_token **tokens, char **envp)
// {
// 	t_token	*current;
// 	current = *tokens;
// 	char	*result;
// 	ft_printf("expanding the variable\n");

// 	//printf("%s\n", current->value);
// 	if (current->value[0] == DOLLAR_SIGN)
// 	{
// 		ft_printf("found dollar sign!\n");
// 	}
// 	return (result);
// }

char	*find_cmd(char *cmd, t_data *data)
{
	int		i;
	char	*tmp;
	char	*holder;

	i = 0;
	while (data->my_paths[i])
	{
		tmp = ft_strjoin(data->my_paths[i], "/");
		holder = ft_strjoin(tmp, cmd);
		if (access(holder, X_OK) == 0)
			return (free(tmp), holder);
		free(tmp);
		free(holder);
		i++;
	}
	write(2, "command not found : ", 20);
	write(2, cmd, ft_strlen(cmd));
	write(2, "\n", 1);
	return (NULL);
}

static void	execute_command(char *command[], t_data *data, char **envp)
{
	char *cmd;
	char **cmd_args;
	char *tmp;

	cmd = NULL;
	cmd = find_cmd(command[0], data);
	tmp = ft_strjoin_gnl(command[0], " ");
	if (command[1] != NULL)
		tmp = ft_strjoin_gnl(tmp, command[1]);
	cmd_args = ft_split(tmp, 32);
	// Debug
	 //printf("%s\n", cmd);
	 //printf("%s\n", cmd_args[0]);
	execve(cmd, cmd_args, envp);
	//exit(1);
	return ;
}

void	token_parser(t_token **tokens, t_data *data, char **envp)
{
	t_token		*current;
	char		*command[2];
	// int *pipe;

	printf("starting parser: ------------------------->\n");
	current = *tokens;
	while (current->type != 7)
	{
		if (current->type == 12)
		{
			printf("command execuion started\n");
			command[0] = current->value;
			command[1] = current->next->value;
			//printf("%s\n", command[0]);
			//printf("%s\n", command[1]);
			// handle pipes and split the token list (?)
			// handle redirection before executing the command;
			execute_command(command, data, envp);
			return ;
			current = current->next->next;
			continue;
		}
		if (current->type == TOKEN_REDIRECT_IN || current->type == TOKEN_REDIRECT_OUT)
		{
			printf("ititializing redirection\n");
			command[0] = current->value;
			command[1] = current->next->value;
			printf("%s\n", command[0]);
			printf("%s\n", command[1]);
			current = current->next->next;
			continue;
		}
		current = current->next;
	}
}