#include "env.h"
#include "minishell.h"


static int	add_default(t_env *env)
{
	char *pwd;
	char *key;
	char *value;
	pwd = env_get(env, PWD);
	if (pwd == NULL)
	{
		key = ft_strdup(PWD);
		value = getcwd(NULL, 0);
		if (key == NULL || value == NULL)
		{
			free(key);
			free(value);
			return (exit_mem_issue());
		}
		return (env_add(env, key, value));
	}
	free(pwd);
	return (EXIT_SUCCESS);
}

static void	free_2d_arr(char **val)
{
	int i;

	i = 0;
	while (val[i] != NULL)
	{
		free(val[i]);
		i++;
	}
	free(val);
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

/*
//TODO nik: set mininal required enviroment var
default ones
PWD=/home/nikit/42/minishell
PATH=/home/nikit/.local/funcheck/host:/home/nikit/.local/funcheck/host:/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin
_=/usr/bin/env*/
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
				free_2d_arr(tmp);
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
	add_default(env);
	return (EXIT_SUCCESS);
}
