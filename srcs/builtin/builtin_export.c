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

static bool	is_empty(char *str)
{
	return (str == NULL || str[0] == '\0');
}

static bool	is_valid_name(char *name)
{
	if (is_empty(name))
		return (true);
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

static void	free_2d_arr(char **val)
{
	int	i;

	i = 0;
	while (val[i] != NULL)
	{
		free(val[i]);
		i++;
	}
	free(val);
}

int	add_args_to_env(t_ctx *c, t_command_ctx *command_ctx)
{
	char		**tmp;
	uint64_t	i;

	i = 1;
	while (i < command_ctx->argc)
	{
		tmp = ft_split_key_value(command_ctx->argv[i], '=');
		if (tmp == NULL)
			return (exit_mem_issue());
		if (is_valid_name(tmp[0]) == false)
		{
			free_2d_arr(tmp);
			i++;
			continue ;
		}
		if (env_add(&c->env, tmp[0], tmp[1]))
		{
			free(tmp);
			return (EXIT_FAILURE);
		}
		free(tmp);
		i++;
	}
	return (EXIT_SUCCESS);
}

/*
All values undergo tilde expansion, parameter and variable expansion,
	command substitution, arithmetic expansion,
	and quote removal (see Shell Parameter Expansion).
	Word splitting and filename expansion are not performed.
*/
int	builtin_export(t_ctx *c, t_command_ctx *command_ctx)
{
	if (command_ctx->argc == 1)
	{
		ft_lstiter(c->env.vals, &print_val);
		return (EXIT_SUCCESS);
	}
	else
		return (add_args_to_env(c, command_ctx));
}
