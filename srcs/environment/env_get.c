#include "minishell.h"
#define EQUAL 0

char	*env_get(t_env *env, char *key)
{
	t_list *vals;
	size_t key_len;

	vals = env->vals;
	key_len = ft_strlen(key);
	if (vals)
		while (vals)
		{
			if (ft_strncmp(((t_env_content*) vals->content)->key, key,
					key_len) == EQUAL)
				return (ft_strdup(((t_env_content*) vals->content)->val));
			vals = vals->next;
		}
	return (NULL);
}