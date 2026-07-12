#include "minishell.h"
#define EQUAL 0

void	env_delete(t_env *env, char *key)
{
	t_list *prev_node;
	t_list *vals;
	size_t key_size;

	prev_node = NULL;
	vals = env->vals;
	key_size = ft_strlen(key) + 1;
	while (vals)
	{
		if(ft_strncmp(((t_env_content *)vals->content)->key, key, key_size) == EQUAL)
		{
			if(prev_node)
				prev_node->next=vals->next;
			else
				env->vals = NULL;
			ft_lstdelone(vals, &free_env_content);
			return;
		}
		prev_node = vals;
		vals = vals->next;
	}
}
