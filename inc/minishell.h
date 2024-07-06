#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdio.h>
# include <stdlib.h>
# include <readline/readline.h>
# include "../libft/libft.h"

# define TRUE 1
# define FALSE 0

# define SINGLE_QUOTES "\'"
# define DOUBLE_QUOTES "\""

typedef enum state
{
	NORMAL,
	STATE_DOUBLE_QUOTES,
	STATE_SINGLE_QUOTES
}	t_state;

typedef enum type
{
	CMD,
	OPTION,
	ARGUMENT
}	e_token_type;

typedef struct s_data
{
	char			*input;
	struct s_token	*token_string;
	e_token_type	state;
}	t_data;

typedef struct s_token
{
	char			*token;
	e_token_type	type;
	struct s_token	*next;
}	t_token;

void	free_exit(t_data *data);
int		parse_input(t_data *data);
void	tokenize_string(t_data *data);
int		lexer_control(t_data *data, int j);
void	init_state(t_data *data);

#endif