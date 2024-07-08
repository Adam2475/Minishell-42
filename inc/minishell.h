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

typedef struct s_data
{
	char			*input;
	struct s_token	*token_string;
	t_state			state;
}	t_data;

void		free_exit(t_data *data);
int			parse_input(t_data *data);
t_token		*tokenize_string(t_data *data);
int			lexer_control(t_data *data, int j);
void		init_state(t_data *data, t_token **tokens);
int			special_cases_lexer(char *buffer, t_token **tokens);

#endif