#include "minishell.h"

static void	print_val(void *str)
{
	printf("%s\n", envp[i]);
}

void	env(t_ctx *c)
{
#ifdef DEBUG
	printf("\nExecuting env:\n");
#endif


	t_list env = c->env.vals;
	if (env)
		ft_lstiter(list, &print_val);
}
