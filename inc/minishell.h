#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdio.h>
# include <stdlib.h>
# include <limits.h>
# include <errno.h>
# include <ctype.h> 
# include <fcntl.h>
# include <readline/readline.h>
# include "../libft/libft.h"
# include <sys/types.h>
# include <sys/wait.h>
# include <sys/stat.h>
# include <readline/readline.h>
# include <readline/history.h>

# define TRUE 1
# define FALSE 0
# define WHITESPACE 32

# define SINGLE_QUOTES '\''
# define DOUBLE_QUOTES '\"'
# define REDIRECT_LEFT '<'
# define REDIRECT_RIGHT '>'
# define PIPE '|'
# define DOLLAR_SIGN '$'

#define MAX_HISTORY 100
#define MAX_COMMAND_LENGTH 1024

typedef struct s_token_list
{
	t_token		*head;
	struct s_token_list *next;
}	t_token_list;

typedef enum cmd
{
	NONE,
	CH_DIR,
	ECHO,
	EXPORT,
	UNSET,
	ENV,
	EXIT,
	PWD
}	t_cmd;

typedef struct s_env_list
{
	char				*var;
	char				*value;
	char				*content;
	struct s_env_list	*pre;
	struct s_env_list	*next;
}	t_env_list;

typedef struct s_data
{
	char			*input;
	int				fd;
	pid_t			parent;
	t_cmd			cmd;
	int				redirect_state;
	/////////////
	// Commands splitted by pipe
	t_env_list		*env_list;
	t_token			*tokens;
	int				err_state;
	struct s_command *commands;
	/////////////
	// From Pipex
	char *my_line;
	char *path_from_envp;
	char **my_paths;
	/////////////
	int	heredoc_flag;
	char *heredoc_content;
}	t_data;

void			free_exit(t_data **data);
t_token			*tokenize_string(t_data **data);
void			init_state(t_data **data, t_token **tokens);
int				special_cases_lexer(t_data **data, char *buffer, t_token **tokens);
void			token_parser(t_token **tokens,t_data **data, char **envp);
int				expand_var(t_token **tkn_lst, t_data **data);
void			expand_doll(t_token **current, t_data **data);
void			env_parser(t_data **data, char **envp);
void			*token_reformatting(t_token **tokens);
char			*find_cmd(char *cmd, t_data **data);
t_env_list		*lstlast_env(t_env_list *lst);
void			gen_list_env(t_data **data, char **envp);
void			add_back_env(t_env_list **lst, t_env_list *new);
t_env_list		*new_node_env(char *content);
void			split_var_env(t_env_list **node, int len);
int				cd_cmd(char **cmd_args, t_data **data);
void			chpwd_env(t_data **data, char *new_path);
int				env_cmd(t_data **data);
int				pwd_cmd(t_data **data);
int				echo_cmd(t_data **data, t_token **tokens);
void			free_exit(t_data **data);
int				parse_input(t_data *data);
int				lexer_control(t_data *data, int j);
void			init_state(t_data **data, t_token **tokens);
char			*expand_variable(t_token **current, char **envp);
void			*token_reformatting(t_token **tokens);
int				ft_lstsize_token(t_token *tokens);
void			print_token_lists(t_token_list *list);
char			*reformat_command(char *command, t_token_list *token_list);
t_token			*flatten_token_list(t_token_list *token_list);
t_token			*extract_command_and_appendices(t_token *tokens);
void			print_tokens(t_token *tokens);
void			append_token_list(t_token_list **list, t_token *head);
t_token_list	*create_token_list_node(t_token *head);
void 			pipe_case(t_token **tokens, t_data **data, char **envp, t_token_list **token_list);
int				execute_command(char *command, t_data **data, char **envp);
char			*trim_whitespace(char *str);
int				count_pipes(t_token* head);
size_t			calculate_command_length(t_token *head);
char			*token_to_command(t_token *head);
t_token			*copy_token_list(t_token *tokens);
t_token			*copy_token(t_token *token);
t_token_list	*split_tokens_by_pipe(t_token *tokens);
char			*retrieve_line(char **envp);
int				piper(t_token **tokens);
t_token			*create_token(t_token_type type, char *value);
void			append_token(t_token **list, t_token *new_token);
int				set_token_state(t_token **tokens);
int				check_double_redirects(const char *str);
int				export_cmd(char **args, t_data **data);
int				add_to_env(char *arg, t_data **data);
int				ft_strsearch(char *str, int c);
void			clean_tokens_qt(t_token **tkn_lst);
/////////// DA ELIMINARE
void			print_env_pwd(t_data **data);
int				check_unclosed_quotes(t_token *token);
void			print_tokens_state(t_token *tokens);
int				check_quotes(t_token **tokens);
void			set_redirection(t_token *tokens, t_data **data);
void			handle_heredoc(char *delimiter, t_data **data);
int				unset_env(t_env_list **env, char *var_name);
void			cmd_exit(char **args, t_data *data);
#endif