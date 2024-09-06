#include "../inc/minishell.h"

void	expand_s_qt(t_data **data, t_token **current)
{
	t_token	*node;
	int		i;

	i = 0;
	while (i < 2)
	{
		tkn_delone(&(*data)->tokens, 10);
		i++;
	}
	node = (*current);
	return ;
}

void	expand_db_qt(t_data **data, t_token **current)
{
	t_token	*node;
	int		i;

	i = 0;
	while (i < 2)
	{
		tkn_delone(&(*data)->tokens, 10);
		i++;
	}
	node = (*current);
	return ;
}

int	expand_var(t_token **tkn_lst, t_data **data)
{
	t_token	*current;

	current = (*tkn_lst);
	while (current->type != TOKEN_EOF)
	{
		if (current->type == 9)
			expand_db_qt(data, &current->next);
		else if (current->type == 10)
			expand_s_qt(data, &current->next);
		current = current->next;
	}
	print_tokens_state(*tkn_lst);
	return (0);
}