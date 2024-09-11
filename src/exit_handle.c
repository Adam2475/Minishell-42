#include "../inc/minishell.h"

void	free_exit(t_data **data)
{
	t_env_list	*node;

	node = (*data)->env_list;
	ft_printf("\033[0;91m free_exit print env \033[0;39m\n");
	while (node != NULL)
	{
		ft_printf("%s\n", node->content);
		node = node->next;
	}
	free((*data)->input);
	close((*data)->fd);
}