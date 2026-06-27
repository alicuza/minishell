#include "minishell.h"

static void	print_val(void *content_void_p)
{
	t_env_content* content;

	content = (t_env_content*) content_void_p;
	printf("%s=%s\n", (char*) content->key, content->val);
}

int	env(t_ctx *c, char **argv)
{
#ifdef DEBUG
	printf("\nExecuting env:\n");
#endif
	(void) argv;
	t_list *env = c->env.vals;
	if (env)
	{
		ft_lstiter(env, &print_val);
		return 0;
	}
	else
		return 2;
}
