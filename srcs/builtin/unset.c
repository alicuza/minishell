#include "minishell.h"

int	unset(t_ctx *c, t_command_ctx *command_ctx)
{
	uint64_t	i;

	i = 1;
	while (i < command_ctx->argc)
	{
		env_delete(&c->env, command_ctx->argv[i]);
		i++;
	}
	return 0;
}
