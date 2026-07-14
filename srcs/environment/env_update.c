#include "minishell.h"
#define EQUAL 0

static t_list	*search_node(t_env *env, char *key)
{
	t_list *vals;
	size_t key_size;

	vals = env->vals;
	key_size = ft_strlen(key) + 1;
	if (vals)
		while (vals)
		{
			if (ft_strncmp(((t_env_content*) vals->content)->key, key,
					key_size) == EQUAL)
				return (vals);
			vals = vals->next;
		}
	return (NULL);
}

int	env_update(t_env *env, char *key, char *value)
{
	t_list *node;
	char* value_copy;

	node = search_node(env, key);
	if(node == NULL)
		return(add(env, ft_strdup(key), ft_strdup(value)));
	value_copy = ft_strdup(value);
	if(value_copy == NULL)
		return (EXIT_FAILURE);
	free(((t_env_content*) node->content)->val);
	((t_env_content*) node->content)->val = value_copy;
	return (0);
}
