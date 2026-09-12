#include "env.h"
#include "minishell.h"

static int	cd_path(t_ctx *c, char *curpath)
{
	int		result;
	char	*oldpwd;

	result = chdir(curpath);
	if (result == EXIT_SUCCESS)
	{
		oldpwd = env_get(&c->env, PWD);
		if (oldpwd)
		{
			if (env_update_with_copy(&c->env, OLDPWD, oldpwd) == EXIT_FAILURE
				|| env_update_with_copy(&c->env, PWD, curpath) == EXIT_FAILURE)
			{
				chdir(oldpwd);
				result = EXIT_FAILURE;
			}
			else
				result = EXIT_SUCCESS;
			free(oldpwd);
		}
		else
		{
			if (env_update_with_copy(&c->env, PWD, curpath) == EXIT_FAILURE)
				result = EXIT_FAILURE;
			else
				result = EXIT_SUCCESS;
		}
		return (result);
	}
	else
	{
		msh_error("cd", curpath, strerror(errno));
		return (EXIT_FAILURE);
	}
}

static int	cd_home(t_ctx *c)
{
	char	*home;
	int		result;

	result = EXIT_SUCCESS;
	home = env_get(&c->env, HOME);
	if (is_empty_str(home))
	{
		msh_error("cd", NULL, "HOME not set");
		result = EXIT_FAILURE;
	}
	else
	{
		result = cd_path(c, home);
	}
	free(home);
	return (result);
}

int	cd_oldpwd(t_ctx *c)
{
	char	*old_path;
	int		result;

	old_path = env_get(&c->env, OLDPWD);
	if (is_empty_str(old_path))
	{
		msh_error("cd", NULL, "OLDPWD not set");
		result = EXIT_FAILURE;
	}
	else
	{
		result = cd_path(c, old_path);
		if (result == EXIT_SUCCESS)
			ft_putendl_fd(old_path, STDOUT_FILENO);
	}
	free(old_path);
	return (result);
}

char	*add_pwd_prefix(t_ctx *c, const char *dir)
{
	char	*tmp;
	char	*tmp1;

	tmp = get_pwd(c);
	if (!tmp)
		return (NULL);
	tmp1 = ft_strjoin(tmp, "/");
	free(tmp);
	if (!tmp1)
		return (NULL);
	tmp = ft_strjoin(tmp1, dir);
	free(tmp1);
	return (tmp);
}

static int	cd_dir(t_ctx *c, const char *dir)
{
	char	*curpath;
	char	*canonical_form;
	int		result;
	t_error	e;

	canonical_form = NULL;
	errno = 0;
	if (dir[0] != '/')
		curpath = add_pwd_prefix(c, dir);
	else
		curpath = ft_strdup(dir);
	if (curpath == NULL)
	{
		if (errno == ENOMEM)
		{
			e = (t_error){NULL, strerror(ENOMEM), 1};
			msh_exit(c, NULL, &e, NULL);
		}
		msh_error("cd", (char *)dir, strerror(errno));
		return (EXIT_FAILURE);
	}
	else
	{
		errno = 0;
		canonical_form = get_path_canonical_form(curpath, ft_strlen(curpath));
		if (canonical_form == NULL)
		{
			if (errno == ENOMEM)
			{
				e = (t_error){NULL, strerror(ENOMEM), 1};
				free(curpath);
				msh_exit(c, NULL, &e, NULL);
			}
			msh_error("cd", (char *)dir, "No such file or directory");
			result = EXIT_FAILURE;
		}
		else
			result = cd_path(c, canonical_form);
	}
	free(curpath);
	free(canonical_form);
	return (result);
}

int	cd(t_ctx *c, t_command_ctx *command_ctx)
{
	char	*dir;

	if (command_ctx->argc > 2)
	{
		msh_error("cd", NULL, "too many arguments");
		return (2);
	}
	dir = command_ctx->argv[1];
	if (is_empty_str(dir))
		return (cd_home(c));
	else
	{
		if (dir[0] == '-' && dir[1] == '\0')
			return (cd_oldpwd(c));
		else
			return (cd_dir(c, dir));
	}
}
