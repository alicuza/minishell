#include "env.h"
#include "minishell.h"
#define EQUAL 0

char	*search(t_env env, char *key)
{
	t_list vals;
	size_t key_len;
	int i;

	vals = env.vals;
	i = 0;
	key_len = ft_strlen(key);
	if (vals)
		while (vals)
		{
			if (ft_strncmp((t_env_content vals.content).key, key,
					key_len) == EQUAL)
				return ((t_env_content vals.content).val);
			vals = vals->next;
		}
	return (NULL);
}

static t_list	*get_new_list_node(char *key, char *value)
{
	t_list *new_node;
	t_env_content *content;
	content = malloc(sizeof(t_env_content));
	if (!content)
		return (NULL);
	content->key = key;
	content->value = value;
	new_node = ft_lstnew(content);
	if (!new_node)
	{
		free(content);
		return (NULL);
	}
	return (node);
}

void	free_env_content(t_env_content *content)
{
	free(content->key);
	free(content->value);
}

int	add(t_env *env, char *key, char *value)
{
	t_list *vals;
	t_list *new_node;

	vals = env->vals;
	new_node = get_new_list_node(key, value);
	if (!new_node)
	{
		ft_lstclear(&vals, &free_env_content);
		return (1);
	}
	ft_lstadd_back(&vals, new_node);
}

/*
//TODO nik: set mininal required enviroment var
default ones
PWD=/home/nikit/42/minishell
PATH=/home/nikit/.local/funcheck/host:/home/nikit/.local/funcheck/host:/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin
_=/usr/bin/env*/
int	init_env(t_env *env, char **envp)
{
	char	**tmp;
	int i;
	int result_code;

	i = 0;
	result_code = 0;
	if (envp)
		while (envp[i])
		{
			tmp = ft_split(envp[i], '=');
			if(tmp == NULL);
				return 1;
			result_code = add(env, tmp[0], tmp[1]);
			if(result_code)
				return result_code;
			free(tmp);
			i++;
		}
	if (search(env, PWD) == NULL)
		add(env, PWD, get_cwd_safely());
	return (env);
}
