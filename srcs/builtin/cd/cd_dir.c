/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd_dir.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nribakov <nribakov@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/13 21:14:45 by nribakov          #+#    #+#             */
/*   Updated: 2026/09/13 21:41:05 by nribakov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "env.h"
#include "minishell.h"

static char	*add_pwd_prefix(t_ctx *c, const char *dir)
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

static int	handle_failed_curpath(t_ctx *c, t_command_ctx *command_ctx,
		const char *dir)
{
	if (errno == ENOMEM)
		handle_mem_error(c, command_ctx, NULL);
	msh_error("cd", (char *)dir, strerror(errno));
	return (EXIT_FAILURE);
}

static int	handle_curpath(t_ctx *c, t_command_ctx *command_ctx,
		const char *dir, char *curpath)
{
	char	*canonical_form;
	int		result;
	t_error	e;

	errno = 0;
	canonical_form = get_path_canonical_form(curpath, ft_strlen(curpath));
	if (canonical_form == NULL)
	{
		if (errno == ENOMEM)
		{
			e = (t_error){NULL, strerror(ENOMEM), 1};
			free(curpath);
			msh_exit(c, command_ctx, &e, NULL);
		}
		msh_error("cd", (char *)dir, "No such file or directory");
		return (EXIT_FAILURE);
	}
	else
	{
		result = cd_path(c, canonical_form, dir);
		free(canonical_form);
	}
	return (result);
}

int	cd_dir(t_ctx *c, t_command_ctx *command_ctx, const char *dir)
{
	char	*curpath;
	int		result;

	errno = 0;
	if (dir[0] != '/')
		curpath = add_pwd_prefix(c, dir);
	else
		curpath = ft_strdup(dir);
	if (curpath == NULL)
		return (handle_failed_curpath(c, command_ctx, dir));
	else
	{
		result = handle_curpath(c, command_ctx, dir, curpath);
		free(curpath);
	}
	return (result);
}
