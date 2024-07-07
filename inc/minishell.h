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

typedef enum state
{
	NORMAL,
	STATE_DOUBLE_QUOTES,
	STATE_SINGLE_QUOTES
}	t_state;

typedef enum type
{
	TOKEN_WORD,
	TOKEN_PIPE,
	TOKEN_REDIRECT_IN,
	TOKEN_REDIRECT_OUT,
	TOKEN_APPEND,
	TOKEN_HEREDOC,
	TOKEN_EOF
}	t_token_type;

typedef struct s_data
{
	char			*input;
	struct s_token	*token_string;
	t_state			state;
}	t_data;

typedef struct s_token
{
	t_token_type	type;
	char			*value;
	struct s_token	*next;
}	t_token;

void	free_exit(t_data *data);
int		parse_input(t_data *data);
void	tokenize_string(t_data *data);
int		lexer_control(t_data *data, int j);
void	init_state(t_data *data, t_token_type *tokens);
int		special_cases_lexer(t_data *data, int i);

#endif