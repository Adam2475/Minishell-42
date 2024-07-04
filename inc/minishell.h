#include <stdio.h>
#include <stdlib.h>
#include <readline/readline.h>

#define TRUE 1
#define FALSE 0

typedef struct s_data
{
    char *input;
}   t_data;

void    free_exit(t_data *data);
int     parse_input(t_data *data);