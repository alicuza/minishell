static bool	is_empty(char *str)
{
	return (str == NULL || str[0] == '\0');
}

static int search_in_paths(char **paths, t_command_ctx *cmd_ctx)
{
	char *tmp;
	char *first_found;
	int i;

	first_found = NULL;
	i = 0;
	while (paths[i] != NULL)
	{
		if(paths[i][0] == '\0')
			tmp = ft_strjoin("./", name);
		else
			tmp = ft_strjoin(paths[i], name);
		if(!tmp)
			return (EXIT_FAILURE);
		if(access(tmp, X_OK) == 0)
		{
			free(cmd_ctx->pathname);
			cmd_ctx->pathname = tmp;
			return (EXIT_SUCCESS);
		}
		else if(first_found == NULL && access(tmp, F_OK) == 0)
		{
			first_found = tmp;
		}
		i++;
	}
	free(cmd_ctx->pathname);
	cmd_ctx->pathname = first_found;
	return (EXIT_SUCCESS);
}

int get_pathname(t_ctx *c, t_command_ctx *cmd_ctx)
{
	char	*path;
	char	**paths;
	char	*tmp;

	path = env_get(&c->env, PATH);
	if(is_empty(path))
	{
		tmp = ft_strjoin("./", cmd_ctx->pathname);
		if(!tmp)
			return (free(path), EXIT_FAILURE); //TODO cleanup and exit
		if(access(tmp, X_OK) == 0)
		{
			free(cmd_ctx->pathname);
			cmd_ctx->pathname = tmp;
			return (free(path), EXIT_SUCCESS);
		}
	}
	else
	{
		paths = ft_split_with_empty(path, ':');
		if(!paths)
			return (free(path), EXIT_FAILURE);
		search_in_paths(paths, cmd_ctx); //todo save and return after cleanup the status 
		//TODO clean path 
	}
	return (NULL);
}
