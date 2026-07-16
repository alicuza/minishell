#include "minishell.h"

static void	print_val(void *content_void_p)
{
	t_env_content	*content;

	content = (t_env_content *)content_void_p;
	if (content->val == NULL)
		printf("declare -x %s\n", (char *)content->key);
	else
		printf("declare -x %s=\"%s\"\n", (char *)content->key, content->val);
}

static bool	is_valid_name(char *name)
		// TODO accept the empty values; add to env init
{
	if (!ft_isalpha(*name) && *name != '_')
		return (false);
	name++;
	while (*name != '\0')
	{
		if (!ft_isalnum(*name) && *name != '_')
			return (false);
		name++;
	}
	return (true);
}

/*
All values undergo tilde expansion, parameter and variable expansion,
	command substitution, arithmetic expansion,
	and quote removal (see Shell Parameter Expansion).
	Word splitting and filename expansion are not performed.
*/
int	builtin_export(t_ctx *c, t_command_ctx *command_ctx)
{
	char		**tmp;
	uint64_t	i;
	int			result_code;

	i = 1;
	if (command_ctx->argc == 1)
	{
		ft_lstiter(c->env.vals, &print_val);
		return (EXIT_SUCCESS);
	}
	while (i < command_ctx->argc)
	{
		tmp = ft_split_key_value(command_ctx->argv[i], '=');
		if (tmp == NULL)
			return (exit_mem_issue());
		if (is_valid_name(tmp[0]) == false)
		{
			free(tmp[0]);
			free(tmp[1]);
			free(tmp);
			i++;
			continue ;
		}
		result_code = env_add(&c->env, tmp[0], tmp[1]);
		if (result_code)
		{
			free(tmp);
			return (result_code);
		}
		free(tmp);
		i++;
	}
	return (EXIT_SUCCESS);
}
