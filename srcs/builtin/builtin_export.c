#include "minishell.h"

int	builtin_export(t_ctx *c, t_command_ctx *command_ctx)
{
	char	**tmp;
	uint64_t	i;
	int result_code;

	i = 1;
	while (i < command_ctx->argc)
	{
		tmp = ft_split_key_value(command_ctx->argv[i], '=');
		if(tmp == NULL)
			return EXIT_FAILURE;
		//TODO add validation
		result_code = add(&c->env, tmp[0], tmp[1]);
		if(result_code)
			return result_code;
		free(tmp);
		i++;
	}
	return 0;
}
