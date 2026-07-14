#include "env.h"
#include "minishell.h"


static t_list	*get_new_list_node(char *key, char *value)
{
	t_list *new_node;
	t_env_content *content;
	content = malloc(sizeof(t_env_content));
	if (!content)
		return (NULL);
	content->key = key;
	content->val = value;
	new_node = ft_lstnew(content);
	if (!new_node)
	{
		free(content);
		return (NULL);
	}
	return (new_node);
}

void	free_env_content(void *content_void_p)
{
	t_env_content *content;

	content = (t_env_content *)content_void_p;
	free(content->key);
	free(content->val);
	free(content);
}

int	free_env(t_env *env)
{
	ft_lstclear(&env->vals, &free_env_content);
	return (EXIT_SUCCESS);
}

int	add_default(t_env *env)
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
