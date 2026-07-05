
#include "env.h"
#include "minishell.h"
#define FAILED -1

static bool	is_empty(char *str)
{
	return (str == NULL || str[0] == '\0');
}

static int	cd_path(t_ctx *c, char *curpath)
{
	int		result;
	char	*oldpwd;

	result = chdir(curpath);
	if (result == 0)
	{
		oldpwd = env_get(&c->env, PWD);
		if (env_update(&c->env, OLDPWD, oldpwd) == FAILED || env_update(&c->env,
				PWD, curpath) == FAILED)
		{
			result = chdir(oldpwd);
			result = -1;
		}
		else
			result = 0;
		free(oldpwd);
		return (result);
	}
	else
	{
		printf("cd: Error during chdir\n");
		return (-1);
	}
}

static int	cd_home(t_ctx *c)
{
	char	*home;
	int		result;

	result = 0;
	home = env_get(&c->env, HOME);
	if (is_empty(home))
	{
		printf("cd: HOME not set\n");
		result = -1;
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
		printf("cd: OLDPWD not set\n");
		return (-1);
	}
	else
	{
		result = cd_path(c, old_path);
		if (result == 0)
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
		result = -1;
	else
	{
		canonical_form = get_path_canonical_form(curpath, ft_strlen(curpath));
		if (canonical_form == NULL)
		{
			printf("cd: %s: No such file or directory\n", dir);
			result = -1;
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
		printf("cd: too many arguments\n");
		return (-1);
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
