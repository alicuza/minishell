#include "minishell.h"
#define EQUAL 0

int env_get(t_env *env, char *key, char** result) //TODO now check if key alweys existis
{
	t_list *vals;
	size_t key_size;

	vals = env->vals;
	key_size = ft_strlen(key) + 1;
	if (vals)
		while (vals)
		{
			if (ft_strncmp(((t_env_content*) vals->content)->key, key, key_size) == EQUAL)
			{
				if(((t_env_content*) vals->content)->val != NULL)
				{
					*result = ft_strdup(((t_env_content*) vals->content)->val);
					if(*result == NULL)
						return (EXIT_FAILURE);
					return (EXIT_SUCCESS);
				}
				return (EXIT_SUCCESS);
			}
			vals = vals->next;
		}
	return (EXIT_SUCCESS);
}
