#include "env.h"
#include "minishell.h"

static int	add_default(t_env *env)
{
	char *pwd;
	pwd = env_get(env, PWD);
	if (pwd == NULL)
		return (env_add(env, ft_strdup(PWD), getcwd(NULL, 0)));
	free(pwd);
	return (EXIT_SUCCESS);
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
	int result_code;

	i = 0;
	result_code = 0;
	if (envp)
		while (envp[i])
		{
			tmp = ft_split_key_value(envp[i], '=');
			if (tmp == NULL)
				return (EXIT_FAILURE);
			result_code = env_add(env, tmp[0], tmp[1]);
			if (result_code)
			{
				free(tmp);
				return (result_code);
			}
			free(tmp);
			i++;
		}
	add_default(env);
	return (0);
}
