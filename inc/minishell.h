#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdio.h>
# include <stdlib.h>
# include <ctype.h> 
# include <readline/readline.h>
# include "../libft/libft.h"

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

typedef struct s_data
{
	char			*input;
	t_state			state;
	
	/////////////
	// From Pipex
	char *my_line;
	char *path_from_envp;
	char **my_paths;
}	t_data;

void		free_exit(t_data *data);
int			parse_input(t_data *data);
t_token		*tokenize_string(t_data *data);
int			lexer_control(t_data *data, int j);
void		init_state(t_data *data, t_token **tokens);
int			special_cases_lexer(t_data *data, char *buffer, t_token **tokens);
void		token_parser(t_token **tokens,t_data *data, char **envp);
char		*expand_variable(t_token **current, char **envp);
void		*token_reformatting(t_token **tokens);
//void		token_parser(t_token **tokens, t_data *data);

#endif