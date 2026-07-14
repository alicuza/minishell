
#include "env.h"
#include "minishell.h"

static bool	is_empty(char *str)
{
	return (str == NULL || str[0] == '\0');
}

static int	cd_path(t_ctx *c, char *curpath)
{
	int		result;
	char	*oldpwd;

	result = chdir(curpath);
	if (result == EXIT_SUCCESS)
	{
		oldpwd = env_get(&c->env, PWD);
		if (env_update(&c->env, OLDPWD, oldpwd) == EXIT_FAILURE || env_update(&c->env,
				PWD, curpath) == EXIT_FAILURE)
		{
			result = chdir(oldpwd);
			result = EXIT_FAILURE;
		}
		else
			result = EXIT_SUCCESS;
		free(oldpwd);
		return (result);
	}
	else
	{
		perror("cd: Error during chdir\n");
		return (errno);
	}
}

static int	cd_home(t_ctx *c)
{
	char	*home;
	int		result;

	result = EXIT_SUCCESS;
	home = env_get(&c->env, HOME);
	if (is_empty(home))
	{
		perror("cd: HOME not set\n"); //TODO do i need to set errno
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
	if (is_empty(old_path))
	{
		perror("cd: OLDPWD not set\n"); //TODO do i need to set errno
		return (EXIT_FAILURE);
	}
	else
	{
		result = cd_path(c, old_path);
		if (result == EXIT_SUCCESS)
			printf("%s\n", old_path);
	}
	free(old_path);
	return (result);
}

char	*add_pwd_prefix(t_ctx *c, const char *dir)
{
	char	*tmp;
	char	*tmp1;

	tmp = get_pwd(c);
	tmp1 = ft_strjoin(tmp, "/");
	free(tmp);
	tmp = ft_strjoin(tmp1, dir);
	free(tmp1);
	return (tmp);
}

static int	cd_dir(t_ctx *c, const char *dir)
{
	char	*curpath;
	char	*canonical_form;
	int		result;

	canonical_form = NULL;
	if (dir[0] != '/')
		curpath = add_pwd_prefix(c, dir);
	else
		curpath = ft_strdup(dir);
	if (curpath == NULL)
		result = EXIT_FAILURE;
	else
	{
		canonical_form = get_path_canonical_form(curpath, ft_strlen(curpath));
		if (canonical_form == NULL)
		{
			perror("cd: %s: No such file or directory\n", dir); //TODO do i need to set errno
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
		perror("cd: too many arguments\n"); //TODO do i need to set errno
		return (EXIT_FAILURE);
	}
	dir = command_ctx->argv[1];
	if (is_empty(dir))
		return (cd_home(c));
	else
	{
		if (dir[0] == '-' && dir[1] == '\0')
			return (cd_oldpwd(c));
		else
			return (cd_dir(c, dir));
	}
}
