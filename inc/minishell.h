#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdio.h>
# include <stdlib.h>
# include <limits.h>
# include <errno.h>
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

typedef enum state
{
	NORMAL,
	STATE_DOUBLE_QUOTES,
	STATE_SINGLE_QUOTES,
	STATE_DOLLAR
}	t_state;


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
	t_state			state;
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
}	t_data;

typedef struct s_command
{
	char *cmd1;
	char **args1;
	char redirect;
	char *cmd2;
	char *args2;
}	t_command;



void		free_exit(t_data **data);
t_token		*tokenize_string(t_data **data);
// int			lexer_control(t_data **data, int j);
void		init_state(t_data **data, t_token **tokens);
int			special_cases_lexer(t_data **data, char *buffer, t_token **tokens);
void		token_parser(t_token **tokens,t_data **data, char **envp);
char		*expand_variable(t_token **current, char **envp);
void		env_parser(t_data **data, char **envp);
void		*token_reformatting(t_token **tokens);
char		*find_cmd(char *cmd, t_data **data);
//void		token_parser(t_token **tokens, t_data *data);

//// built_in ////
t_env_list	*lstlast_env(t_env_list *lst);
void		gen_list_env(t_data **data, char **envp);
void		add_back_env(t_env_list **lst, t_env_list *new);
t_env_list	*new_node_env(char *content);
void		split_var_env(t_env_list **node, int len);
int			cd_cmd(char **cmd_args, t_data **data);
void		chpwd_env(t_data **data, char *new_path);
int			env_cmd(t_data **data);
int			pwd_cmd(t_data **data);
int			echo_doll(char *str, t_data **data);
int			echo_basic(char *str);
int			echo_cmd(t_data **data, char **cmd_args, t_token **tokens);

#endif