#include "env.h"
#include "minishell.h"

#define EQUAL 0

int	unset(t_ctx *c, t_command_ctx *command_ctx)
{
	uint64_t	i;

	i = 1;
	while (i < command_ctx->argc)
	{
		if (ft_strncmp(command_ctx->argv[i], SHLVL, 6) != EQUAL)
			env_delete(&c->env, command_ctx->argv[i]);
		i++;
	}
	return (0);
}
