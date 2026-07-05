#include "minishell.h"
#define EQUAL 0


static t_list	*search_node(t_env *env, char *key)
{
	t_list *vals;
	size_t key_len;

	vals = env->vals;
	key_len = ft_strlen(key);
	if (vals)
		while (vals)
		{
			if (ft_strncmp(((t_env_content *)vals->content)->key, key,
					key_len) == EQUAL)
				return (vals);
			vals = vals->next;
		}
	return (NULL);
}

void	env_delete(t_env *env, char *key)
{
	t_list *node;

	node = search_node(env, key);
	if (node != NULL)
	{
		// TODO delete and relink
	}
}
