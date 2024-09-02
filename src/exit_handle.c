#include "../inc/minishell.h"

void	free_exit(t_data **data)
{
	free((*data)->input);
	close((*data)->fd);
}