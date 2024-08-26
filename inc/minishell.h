#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdio.h>
# include <stdlib.h>
# include <ctype.h> 
# include <readline/readline.h>
# include "../libft/libft.h"
# include <sys/types.h>
# include <sys/wait.h>

# define TRUE 1
# define FALSE 0
# define WHITESPACE 32

# define SINGLE_QUOTES '\''
# define DOUBLE_QUOTES '\"'
# define REDIRECT_LEFT '<'
# define REDIRECT_RIGHT '>'
# define PIPE '|'
# define DOLLAR_SIGN '$'

typedef struct s_token_list {
	t_token		*head;
	struct s_token_list *next;
}	t_token_list;

typedef enum state
{
	NORMAL,
	STATE_DOUBLE_QUOTES,
	STATE_SINGLE_QUOTES,
	STATE_DOLLAR
}	t_state;

typedef struct s_data
{
	char			*input;
	int				fd;
	t_state			state;
	int				redirect_state;
	/////////////
	// Commands splitted by pipe
	struct s_command *commands;
	/////////////
	// From Pipex
	char *my_line;
	char *path_from_envp;
	char **my_paths;
	/////////////
	// 	For Pipe
	int *end;
}	t_data;

typedef struct s_command
{
	char *cmd1;
	char **args1;
	char redirect;
	char *cmd2;
	char *args2;
}	t_command;



void			free_exit(t_data *data);
int				parse_input(t_data *data);
t_token			*tokenize_string(t_data *data);
int				lexer_control(t_data *data, int j);
void			init_state(t_data *data, t_token **tokens);
int				special_cases_lexer(t_data *data, char *buffer, t_token **tokens);
void			token_parser(t_token **tokens,t_data *data, char **envp);
char			*expand_variable(t_token **current, char **envp);
void			*token_reformatting(t_token **tokens);
char			*find_cmd(char *cmd, t_data *data);
int				ft_lstsize_token(t_token *lst);
void			print_token_lists(t_token_list *list);
char			*reformat_command(char *command, t_token_list *token_list);
t_token			*flatten_token_list(t_token_list *token_list);
t_token			*extract_command_and_appendices(t_token *tokens);
void			print_tokens(t_token *tokens);
void			append_token_list(t_token_list **list, t_token *head);
t_token_list	*create_token_list_node(t_token *head);
void 			pipe_case(t_token **tokens, t_data *data, char **envp, t_token_list **token_list);
int				execute_command(char *command, t_data *data, char **envp);
char			*trim_whitespace(char *str);
int				count_pipes(t_token* head);
size_t			calculate_command_length(t_token *head);
char			*token_to_command(t_token *head);
t_token			*copy_token_list(t_token *tokens);
t_token			*copy_token(t_token *token);

#endif