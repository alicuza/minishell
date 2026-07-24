#include "minishell.h"

static t_list	*get_new_list_node(char *key, char *value)
{
	t_list *new_node;
	t_env_content *content;
	content = malloc(sizeof(t_env_content));
	if (!content)
		return (NULL);
	content->key = key;
	content->val = value;
	new_node = ft_lstnew(content);
	if (!new_node)
	{
		free(content);
		return (NULL);
	}
	return (new_node);
}

int	env_add(t_env *env, char *key, char *value)
{
	t_list **vals;
	t_list *new_node;

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
