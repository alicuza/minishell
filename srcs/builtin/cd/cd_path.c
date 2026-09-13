/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd_path.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nribakov <nribakov@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/13 20:15:03 by nribakov          #+#    #+#             */
/*   Updated: 2026/09/13 20:27:03 by nribakov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "env.h"
#include "minishell.h"

static int	handle_error(char *curpath, const char *orig)
{
	if (orig != NULL)
		msh_error("cd", (char *)orig, strerror(errno));
	else
		msh_error("cd", curpath, strerror(errno));
	return (EXIT_FAILURE);
}

static int	update_oldpwd_and_pwd(t_ctx *c, char *curpath, char *oldpwd)
{
	int	result;

	result = EXIT_SUCCESS;
	if (env_update_with_copy(&c->env, OLDPWD, oldpwd) == EXIT_FAILURE
		|| env_update_with_copy(&c->env, PWD, curpath) == EXIT_FAILURE)
	{
		result = chdir(oldpwd);
		result = EXIT_FAILURE;
	}
	else
		result = EXIT_SUCCESS;
	return (result);
}

static int	update_pwd(t_ctx *c, char *curpath)
{
	int	result;

	result = EXIT_SUCCESS;
	if (env_update_with_copy(&c->env, PWD, curpath) == EXIT_FAILURE)
		result = EXIT_FAILURE;
	else
		result = EXIT_SUCCESS;
	return (result);
}

int	cd_path(t_ctx *c, char *curpath, const char *orig)
{
	int		result;
	char	*oldpwd;

	result = chdir(curpath);
	if (result == EXIT_SUCCESS)
	{
		oldpwd = env_get(&c->env, PWD);
		if (oldpwd)
		{
			result = update_oldpwd_and_pwd(c, curpath, oldpwd);
			free(oldpwd);
		}
		else
			result = update_pwd(c, curpath);
		return (result);
	}
	else
		return (handle_error(curpath, orig));
}
