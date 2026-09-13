/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nribakov <nribakov@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/13 21:13:54 by nribakov          #+#    #+#             */
/*   Updated: 2026/09/13 21:29:13 by nribakov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "env.h"
#include "minishell.h"

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
		result = cd_path(c, home, NULL);
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
		result = cd_path(c, old_path, NULL);
		if (result == EXIT_SUCCESS)
			ft_putendl_fd(old_path, STDOUT_FILENO);
	}
	free(old_path);
	return (result);
}

int	cd(t_ctx *c, t_command_ctx *command_ctx)
{
	char	*dir;

	if (command_ctx->argc > 2)
	{
		msh_error("cd", NULL, "too many arguments");
		return (1);
	}
	dir = command_ctx->argv[1];
	if (is_empty_str(dir))
		return (cd_home(c));
	else
	{
		if (dir[0] == '-' && dir[1] == '\0')
			return (cd_oldpwd(c));
		else
			return (cd_dir(c, command_ctx, dir));
	}
}
