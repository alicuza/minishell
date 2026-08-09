#include "env.h"
#include "minishell.h"

static bool	is_valid_name(char *name)
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

int	init_env(t_env *env, char **envp)
{
	char **tmp;
	int i;

	i = 0;
	if (envp)
		while (envp[i])
		{
			tmp = ft_split_key_value(envp[i], '=');
			if (tmp == NULL)
				return (EXIT_FAILURE);
			if (is_valid_name(tmp[0]) == false)
			{
				free_str_arr(tmp);
				i++;
				continue ;
			}
			if (env_add(env, tmp[0], tmp[1]))
			{
				free(tmp);
				return (EXIT_FAILURE);
			}
			free(tmp);
			i++;
		}
	add_env_defaults(env);
	return (EXIT_SUCCESS);
}
