#include "env.h"
#include "minishell.h"

static void	ft_free_str_arr(char **arr)
{
	int	i;

	i = 0;
	while (arr[i] != NULL)
	{
		free(arr[i]);
		i++;
	}
	free(arr);
}

static bool	is_empty(char *str)
{
	return (str == NULL || str[0] == '\0');
}

char	*add_prefix(char *path, const char *pathname) //TODO test if it is fine if prefix already added in original string
{
	char	*tmp;
	char	*tmp1;

	tmp = NULL;
	tmp1 = NULL;
	tmp = ft_strjoin(path, "/");
	if(tmp != NULL)
	{
		tmp1 = ft_strjoin(tmp, pathname);
		free(tmp);
	}
	return (tmp1);
}

static int	search_in_paths(char **paths, t_command_ctx *cmd_ctx)
{
	char	*tmp;
	char	*first_found;
	int		i;

	first_found = NULL;
	i = 0;
	while (paths[i] != NULL)
	{
		if (paths[i][0] == '\0')
			tmp = ft_strjoin("./", cmd_ctx->pathname);
		else
			tmp = add_prefix(paths[i], cmd_ctx->pathname);
		if (!tmp)
			return (EXIT_FAILURE);
		if (access(tmp, X_OK) == 0)
		{
			free(cmd_ctx->pathname);
			cmd_ctx->pathname = tmp;
			return (EXIT_SUCCESS);
		}
		else if (first_found == NULL && access(tmp, F_OK) == 0)
		{
			first_found = tmp;
		}
		i++;
	}
	free(cmd_ctx->pathname);
	cmd_ctx->pathname = first_found;
	return (EXIT_SUCCESS);
}

static int get_from_current(t_command_ctx *cmd_ctx)
{
	char	*tmp;

	tmp = ft_strjoin("./", cmd_ctx->pathname);
	if (!tmp)
		return (EXIT_FAILURE);
	if (access(tmp, F_OK) == 0)
	{
		free(cmd_ctx->pathname);
		cmd_ctx->pathname = tmp;
		return (EXIT_SUCCESS);
	}
	free(cmd_ctx->pathname);
	cmd_ctx->pathname = NULL;
	return (EXIT_SUCCESS);
} 

int	get_pathname(t_ctx *c, t_command_ctx *cmd_ctx)
{
	char	*path;
	char	**paths;
	int		status;

	path = env_get(&c->env, PATH);
	if (is_empty(path))
	{
		status = get_from_current(cmd_ctx);
		free(path);
		return (status);
	}
	else
	{
		paths = ft_split_with_empty(path, ':');
		if (!paths)
			return (free(path), EXIT_FAILURE);
		status = search_in_paths(paths, cmd_ctx);
		ft_free_str_arr(paths);
		return (free(path), status);
	}
}
