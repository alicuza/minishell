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
			if (ft_strncmp(((t_env_content *)vals->content)->key, key,
					key_size) == EQUAL)
				return (vals);
			vals = vals->next;
		}
	return (NULL);
}

int	env_update_with_copy(t_env *env, char *key, char *value)
{
	char *key_loc;
	char *value_loc;

	key_loc = ft_strdup(key);
	value_loc = ft_strdup(value);
	if (key_loc == NULL || value_loc == NULL)
	{
		free(key_loc);
		free(value_loc);
		return (exit_mem_issue());
	}
	return (env_update(env, key_loc, value_loc));
}

int	env_update(t_env *env, char *key, char *value)
{
	t_list *node;

	node = search_node(env, key);
	if (node == NULL)
		return (env_add(env, key, value));
	if (value != NULL)
	{
		free(((t_env_content *)node->content)->key);
		free(((t_env_content *)node->content)->val);
		((t_env_content *)node->content)->key = key;
		((t_env_content *)node->content)->val = value;
	}
	return (0);
}
