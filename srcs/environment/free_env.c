#include "minishell.h"

void	free_env_content(void *content_void_p)
{
	t_env_content *content;

	content = (t_env_content *)content_void_p;
	free(content->key);
	free(content->val);
	free(content);
}

void	free_env(t_env *env)
{
	ft_lstclear(&env->vals, &free_env_content);
}