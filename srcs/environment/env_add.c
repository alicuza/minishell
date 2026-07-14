#include "minishell.h"

int	env_add(t_env *env, char *key, char *value)
{
	t_list **vals;
	t_list *new_node;

	if (key == NULL || value == NULL)
		return (exit_mem_issue());
	vals = &env->vals;
	new_node = get_new_list_node(key, value);
	if (!new_node)
	{
		ft_lstclear(vals, &free_env_content);
		free(key);
		free(value);
		return (exit_mem_issue());
	}
	ft_lstadd_back(vals, new_node);
	return (EXIT_SUCCESS);
}
