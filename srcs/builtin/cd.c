
#include "env.h"
#include "minishell.h"

static bool	is_empty(char *str)
{
	return (str == NULL || str[0] == "\0");
}

static int	cd_path(void)
{
	int		result;

	result = chdir(curpath);
	if(result == 0)
	{
		//set OLDPWD
		//set PWD
		rerutn 0;
	}
	else
	{
		// print error
	}
}

static int	cd_home(t_ctx *c)
{
	char	*home;

	home = search(&c->env, HOME);
	if (is_empty(home))
	{
		printf("cd: HOME not set");
		return (1);
	}
	else
		return (cd_path(home));
}

int	cd_oldpwd(t_ctx *c)
{
	char	*old_path;
	int		result;

	old_path = search(&c->env, OLDPWD);
	if (is_empty(old_path))
	{
		printf("cd: OLDPWD not set");
		return (1);
	}
	else
	{
		result = cd_path(old_path);
		if (result == 0)
			printf("%s\n", old_path);
		return (result);
	}
}
static char*	make_canonical_form(char *curpath)
{
	char *canonical_form;
	size_t size;
	size_t	i;

	size = ft_strlen(curpath) + 1;
	canonical_form = malloc(size);
	if(canonical_form == NULL)
		return;
	i = 0;
	while (i < size - 1 && curpath[i])
	{
		canonical_form[i] = curpath[i];
		i++;
	}
	canonical_form[i] = '\0';
	return (canonical_form);
	// Dot components and any<slash> characters that separate them from the next component shall be deleted.
	// 	For each dot -
	// 	dot component,
	// 	if there is a preceding component and it is neither root nor dot - dot,
	// 	then :
	// 	If the preceding component does not refer(in the context of pathname resolution with symbolic links followed) to a directory,
	// 	then the cd utility shall display an appropriate error message and
	// 		no further steps shall be taken.
	// 		The preceding component,
	// 	all<slash> characters separating the preceding component from dot - dot,
	// 	dot - dot,
	// 	and all<slash> characters separating dot -
	// 		dot from the following component(if any) shall be deleted.
	// 		An implementation may further simplify curpath by removing any trailing<slash>
	// 			characters that are not also leading<slash> characters,
	// 	replacing multiple non
	// 		- leading consecutive<slash> characters with a single<slash>,
	// 	and replacing three or more leading<slash> characters with a single<slash>.If,
	// 	as a result of this canonicalization,
	// 	the curpath variable is null,
	// 	no further steps shall be taken.
}

static int cd_dir(t_ctx *c, const char *dir)
{
	char *curpath;
	char *canonical_form;
	int		result;

	if (dir[0] != '/')
	{
		curpath = ft_strjoin(ft_strjoin(get_pwd(), "/"), dir);
		if(curpath == NULL)
			return 1;
	}
	else
	{
		curpath = ft_strdup(dir);
	}
	canonical_form = make_canonical_form(curpath);
	if(canonical_form == NULL)
		result = 1;
	else 
		result = cd_path(canonical_form);
	free(curpath);
	free(canonical_form);
	return;
}

int	cd(t_ctx *c, t_command_ctx *command_ctx)
{
	const char *dir;
	
	if (command_ctx->argc > 2)
		printf("cd: too many arguments");
	dir = command_ctx->argc[1];
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


