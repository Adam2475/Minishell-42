#include "../inc/minishell.h"

void	clean_tokens_qt(t_token **tkn_lst)
{
	t_token	*node;

	node = (*tkn_lst);
	while (node->type != TOKEN_EOF)
	{
		if (node->next->type == TOKEN_DOUBLE_QUOTES)
			tkn_delone(&node, node->next);
		if (node->next->type == TOKEN_SINGLE_QUOTES)
			tkn_delone(&node, node->next);
		node = node->next;
	}
}

void	expand_doll(t_token **current, t_data **data)
{
	t_env_list	*node;
	char		*tmp;

	node = (*data)->env_list;
	tmp = ft_strdup((*current)->value);
	tmp = ft_strtrim(tmp, "$");
	tmp = ft_strjoin(tmp, "=");
	while (node && ft_strncmp(tmp, node->var, ft_strlen(tmp)) != 0)
	{
		if (node->next)
			node = node->next;
		else if (!node->next)
		{
			node = node->next;
			break ;
		}
	}
	if (!node)
	{
		free((*current)->value);
		(*current)->value = ft_strndup("", 1);
		return ;
	}
	free(tmp);
	(*current)->value = ft_strndup(node->value, ft_strlen(node->value));
}

int	expand_var(t_token **tkn_lst, t_data **data)
{
	t_token	*current;

	current = (*tkn_lst);
	// clean_tokens_qt(tkn_lst);
	while (current->type != TOKEN_EOF)
	{
		if (current->type == 8)
			expand_doll(&current, data);
		current = current->next;
	}
	print_tokens_state(*tkn_lst);
	return (0);
}
// TODO: va rimesso che azzera i token type